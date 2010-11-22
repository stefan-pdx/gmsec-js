var GMSEClib = require('../src/build/default/gmsec');

function GMSEC_SubscribeCallback(message){
	console.log("WOOT");
};

function TestSubscribe(connection, subject){
	console.log("test?");
	connection.Subscribe(subject, GMSEC_SubscribeCallback, function(message){
			console.log("?");
		});
	console.log("test!");
};
	
Connection = new GMSEClib.Connection();
Connection.Connect(function(data){
	console.log("?");
	TestSubscribe(Connection, "gmsec.test2.publish.another")
	console.log("A");
});

var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World\n');
}).listen(8124, "127.0.0.1");
console.log('Server running at http://127.0.0.1:8124/');

