var express = require('express'),
	app = express(),
	server = require('http').createServer(app).listen(8080),
	GMSEC = require('../../deps/node.js/Release/gmsec'),
	fs = require('fs'),
	io = require('socket.io').listen(server);

//// Configuration ////

app.configure(function() {
	app.set('views', __dirname);
	app.use(express.static(__dirname + '/public'));
});

// Set the socket.io log level
io.set('log level', 1);

//// Page ////

app.get('/', function(request, response) {
	response.sendfile('index.html');
});

//// GMSEC Connection ///

var Connection = new GMSEC.Connection(); 

Connection.Connect("127.0.0.1", function(){
    console.log('Connected to GMSEC Message Bus Server.');
	console.log('Subscribing to GMSEC');
	
    Connection.Subscribe('GMSEC.FREEFLYER.PUBLISHER.SC.POSITION.UPDATE', function(freeFlyerData){
        console.log('Received message.');
        console.log(freeFlyerData);

        fs.appendFile('recording.txt', freeFlyerData, function(error) {
            if (error){
                console.log(error);
            }
        });
    });

});