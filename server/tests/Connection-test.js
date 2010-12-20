var io = require('socket.io'),
    http = require('http'),
    mongo = require('mongodb'),
    GMSEC = require('../src/build/default/gmsec'),
    libxml = require('libxmljs'),
    mongoose = require('mongoose').Mongoose;

/* Add a unique method to the array prototype. */
Array.prototype.unique = function() {
    var o = {}, i, l = this.length, r = [];
    for(i=0; i<l;i++) o[this[i]] = this[i];
    for(i in o) r.push(o[i]);
    return r;
};

/* Subscription model definition */
mongoose.model('Subscription', {
  properties : ['subject', 'clients'],
});

/*
 * Function to convert an XML string to and object in Javascript.
 * Note that this assumes a specific structure that is similar to
 * that of the GMSEC messages.
 *
 * TODO: Do validation check on data.
 */
function xmlToObject(xmlStr){
  
  /* Extract out XML structure from string. */
  var doc = libxml.parseXmlString(xmlStr);

  /* Create a new object to reformat the XML structure. */
  var msg = {};
  msg['Subject'] = doc.root().attr('SUBJECT').value();
  msg['Kind']    = doc.root().attr('KIND').value();
  msg['Fields']  = {};
  
  /* Extract out the field elements */
  for (var i=2;i<doc.root().childNodes().length;i=i+2){
    msg['Fields'][doc.root().child(i).attr('NAME').value()] = {Value: doc.root().child(i).text(),
                                                               Type:  doc.root().child(i).attr('TYPE').value()};
  };
  
  return msg;
};

/*
 * Function to convert a Javascript object to an XML string.
 * This function should be an inverse operation to that of
 * xmlToObject.
 *
 * TODO: Do validation check on data.
 */
function objectToXml(msg){

  var doc = new libxml.Document(function(n) {
    n.node('MESSAGE', {SUBJECT: msg.Subject, KIND: msg.Kind}, function(n) {
      for (var field in msg.Fields){
         n.node('FIELD', {NAME: field,
                          TYPE: msg.Fields[field].Type}, msg.Fields[field].Value);
      };
    });
  });

  /* Convert doc to string */
  docString = doc.toString();

  /* Remove first header line (<xml...) so that the message is compatible with GMSEC. */
  docStringArr = docString.split('\n');
  docStringArr.shift()
  docString = docStringArr.join('')

  return docString;
};

/*
 * Function to build an array of strings that act as compatible
 * subjects when linking incoming messages to available subscriptions.
 * For example, passing the message subject:
 *
 *     gmsec.test1.test2.test3
 *
 * through this function should result in the array:
 *
 *     ['*',
 *      'gmsec.*',
 *      'gmsec.test1.*',
 *      'gmsec.test1.test2.*',
 *      'gmsec.test1.test2.test3']
 *
 * TODO: String validation:
 *
 *            1) *******.***
 *            2) gmsec.\n.*
 */
function compatibleSubjects(subject){
  /* Extract out fields from subject string and built an array of compatible subjects. */
  var queries = ['*']
  var query   = ''
  var fields = subject.split('.');
  for (var i=0;i<fields.length-1;i++){
    query = query + fields[i];
    queries.push(query+'.*');
    query = query + '.';
  };
  queries.push(query+fields[fields.length-1]);
  return queries;
};

/*
 * Function to process received messages from the GMSEC interface. Note that we have
 * to perform re-routing measures to broadcast this message out to the subset of users
 * that have previously subscribed.
 */
function processReceivedMessage(msg){
  clients = [];

  /* Convert message xml to object and extract compatible subject titles. */
  msg = xmlToObject(msg);
  subjects = compatibleSubjects(msg.Subject);

  /* Extract unique clients from db. */
  Subscription.find({ subject : { $in : compatibleSubjects(msg.Subject) } }).all(function(subscriptions){
    for (ind in subscriptions){
      clients = clients.concat(subscriptions[ind].clients);
    };
  });

  /* Broadcast out to clients the message. */
  socket.broadcast( {type: 'subscribe',data: msg}, clients.unique());
};

/*
 * Function to process subscribe requests from client. Any requests to subscribe
 * to a GMSEC message must be logged in the database since the clients themselves
 * don't interface directly to the GMSEC interface. As a result, we need to group
 * users together by subject type so that we can easily sort out which clients
 * need to be forwarded the message.
 */
function processSubscribeRequest(client, subject){

  /* Query the db to see if the client has already subscribed to this message. */
  Subscription.find({ subject : { $in : compatibleSubjects(subject) } }).all(function(subscriptions){

    /* Go through each available subscriptions to see if the user is registered for a message.
     * If the user has already subscribed to this message, then return out of this function - there's
     * no need to store multiple instances of subscriptions. The user will already be receiving messages.
     */
    for (var ind=0; ind < subscriptions.length; ind++){
      if (subscriptions[ind].clients.indexOf(client.sessionId) >= 0){return; };
    };

    /* Query to see if this subject has been recorded in the database. If it has, then include this
     * client in the associated client list for that subscription.
     */
    for (var ind=0; ind < subscriptions.length; ind++){
      if (subscriptions[ind].subject == subject){
        subscriptions[ind].clients.push(client.sessionId);
        subscriptions[ind].save();
        return;
      };
    };

    /* At this point, the subscription does not exist within the database. Create a new entry in the database
     * with the current user as the sole subscriber and submit the subscription request to the GMSEC API.
     */
    var subscription = new Subscription();
    subscription.subject = subject;
    subscription.clients = [client.sessionId];
    subscription.save()

    Connection.Subscribe(subject, function(msg){processReceivedMessage(msg)});
  });
};

/*
 * Function to process requests from clients to publish out data.
 * Note that since clients don't know their ID, we're going to include
 * a field for CLIENT-ID within the message structure.
 */
function processPublishRequest(client, msg){
  /* Add a client-id field to the message */
  msg.Fields['CLIENT-ID'] = {Value : client.sessionId,
                             Type  : 'STRING'};
  
  /* Publish the message. */
  var msgStr = objectToXml(msg);
  Connection.Publish(msgStr);
console.log('Published!')
  
};

/* Create an interface to the database and create and instance of
 * our Subscription model. */
var db = mongoose.connect('mongodb://localhost/test');
var Subscription = db.model('Subscription');

/* We're going to need to clean out the subscriptions that existed in
 * the database previously.
 *
 * TODO: Verify pre-existing subscriptions in case of node.js restart
 */
Subscription.find().all( function(subscriptions){
  for (var i=0; i<subscriptions.length; i++) {
    subscriptions[i].remove();
  };
});

/* Create the web server. Right now, we're using Socket.IO to act as
 * a data proxy, so there's no need to serve out pages. */
var server = http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World\n');
});
server.listen(8124, '127.0.0.1')

/* Open up the Socket.IO listener. */
var socket = io.listen(server); 

/* Process new connections from clients. */
socket.on('connection', function(client){

  /* Process messages from the client. Note that we are going to be expecting objects
   * that have two fields: type and data. This is to provide additional context in the
   * mapping of client requests to the GMSEC API. */
  client.on('message', function(msg){
    if (msg.type == 'subscribe')   { processSubscribeRequest(client, msg.data) }
    if (msg.type == 'unsubscribe') { processSubscribeRequest(client, msg.data) }
    if (msg.type == 'publish')     { processPublishRequest(client, msg.data) }      
  })

  /* If a user disconnects, we're going to have to update the database to remove
   * unneeded subscriptions. */
  client.on('disconnect', function(){
    
    /* Perform a query to extract out the subscriptions for the client. */
    Subscription.find( {clients : client.sessionId} ).all(function(subscriptions){

      /* Step through each subscription and remove the client from the list of
       * clients associated with the subscription. */
      for (var ind=0; ind < subscriptions.length; ind++){

        /* Remove this client from the specific subscription. */
        subscriptions[ind].clients.splice( subscriptions[ind].clients.indexOf(client.sessionId) ,1)

        /* If there are no other clients associated with this subscription,
         * we're going to want to unsubscribe from it. Otherwise, save the
         * new record with the client removed from the subscriptions.
         *
         * TODO: Implement GMSEC Unsubscribe method. */
        if (subscriptions[ind].clients.length == 0){
          /* TODO: Change to sub[ind].remove() */
	  Subscription.remove( {_id: subscriptions[ind]._id} );
        }else{
	  subscriptions[ind].save()
        }
      }
    });
  });
});

/* Initiate the GMSEC connection! */
var Connection = new GMSEC.Connection();
Connection.Connect(function(){});

