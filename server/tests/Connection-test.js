var io = require('socket.io'),
    http = require('http'),
    mongo = require('mongodb'),
    GMSEC = require('../src/build/default/gmsec'),
    libxml = require('libxmljs'),
    mongoose = require('mongoose');

Model.define('Subscription', {
  collection : 'subscriptions',
  types : {
    _id : Object,
    subject : String,
    clients : Array
  },
  indexes : ['subject'],
  static : {},
  methods : {},
});

function xmlToJSON(xmlStr){
  // Extract out XML object from string.
  var doc = libxml.parseXmlString(xmlStr);

  // Create a message obj to store XML data.
  var msg = {};
  msg['Subject'] = doc.root().attr('SUBJECT').value();
  msg['Kind']    = doc.root().attr('KIND').value();
  msg['Fields']  = {};
  
  // Extract Fields elements.
  for (i=2;i<doc.root().childNodes().length;i=i+2){
    msg['Fields'][doc.root().child(i).attr('NAME').value()] = {Value: doc.root().child(i).text(),
                                                               Type:  doc.root().child(i).attr('TYPE').value()};
  };
  return msg;
};

// Create db.
var db = new mongo.Db('gmsecDemo', new mongo.Server('localhost', 27017, {}), {});
db.open(function(){});

// Create server.
var server = http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World\n');
});
server.listen(8124, '127.0.0.1')

// Create socket.
var socket = io.listen(server); 
socket.on('connection', function(client){});

function buildSubjectRegEx(subject){
  // Extract out fields from subject string and build regex.
  var fields = subject.split('.');
  var regex_str = '('+fields[0]+'|\*)';
  for (i=1;i<fields.length;i++){
    regex_str = '((?<!\*)\.'+fields[i]+'1|\.\*|(?<=\*)$)';
  }
  return new RegExp(regex);

// Process subscribe requests from client.
function processSubscribeRequest(client, subject){
  var regex = buildSubjectRegEx(subject);

  // Query the db to see if the client has already subscribed to this message.
  Subscription.find({ subject : regex }).all(function(msgs){
    client_found = false;

    for (var subscription in subscriptions){
      if (client in subscription.clients){ client_found = true };
    }

    if (!client_found){
      
    }

};


Connection = new GMSEC.Connection();
Connection.Connect(function(){
  Connection.Subscribe('gmsec.test2.publish.another',
                       function(message){socket.broadcast(xmlToJSON(message))});
});

