var GMSEClib = require('../src/build/default/gmsec');
	
Connection = new GMSEClib.Connection();
Connection.Connect(function(){
	Connection.Subscribe("gmsec.test2.publish.another",
                             function(message){console.log(message);},
                             function(message){console.log("      Post-subscribe complete!");});
	console.log("Submitted subscribe.");
});

var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World\n');
}).listen(8124, "127.0.0.1");
console.log('Server running at http://127.0.0.1:8124/');
