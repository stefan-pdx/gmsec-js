var http = require('http'),
	GMSEC = require('../../build/Release/gmsec');

//Create simple http server to keep node alive.
http.createServer(function (req, res) {
}).listen(1337, '127.0.0.1');

console.log('Simple gmsec-js example.');

var Connection = new GMSEC.Connection(); 

Connection.Connect("127.0.0.1", function(){
    console.log('Connected to GMSEC Message Bus Server.')
	console.log('Subscribing to GMSEC.TEST.SUBJECT.')
	
    Connection.Subscribe('GMSEC.TEST.SUBJECT', function(msg){
        console.log('Received message.');
        console.log(msg);
    });
});
