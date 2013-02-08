var express = require('express'),
app = express(),
server = require('http').createServer(app).listen(8081),
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
Connection.Connect('localhost', function() {
	console.log('Connected to GMSEC');

	io.sockets.on('connection', function(socket) {


	});

});

var messages = [];
var readStream = fs.createReadStream('cat.txt');

	readStream.on('open', function() {
		console.log('Opening file...');

		var remainingData = '';
		var singleMessage = '';
	readStream.on('data', function(data) {

		console.log('Getting data...');

		remainingData += data;

		var index = remainingData.indexOf('\n');
		var last = 0;

		while (index > -1) {
			var line = remainingData.substring(last, index);
			last = index + 1;

			singleMessage += line;

			if (line === '<\/MESSAGE>'){
				io.sockets.send(singleMessage);

				//Publish GMSEC message here
				console.log('Pushing to array');
				messages.push(singleMessage);
				singleMessage = ''; 
		}

		index = remainingData.indexOf('\n', last);
	}
	remainingData = remainingData.substring(last);
});

	readStream.on('end', function() {
		console.log('Finished reading file. Messages length: ' + messages.length);

		var counter = 0;

		function next() {
			if (counter < messages.length){
				console.log("Publishing Message #:" + counter);

				Connection.Publish(messages[counter]);

				counter++;
				setTimeout(next, 50);
			}
		}

		next();

	});
});

