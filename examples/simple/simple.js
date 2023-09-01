var http = require('http'),
	GMSEC = require('../../deps/node.js/Release/gmsec');

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
	
	var counter = 0;
	var msgCount = 1000;
	var publishMessage = function(){
		if (counter < msgCount){
			var testMessage = "<MESSAGE SUBJECT='GMSEC.TEST.SUBJECT' KIND='PUBLISH'>" +
									'<FIELD TYPE="STRING" NAME="SUBJECTLINE">Message #'+ counter +'</FIELD>' +
								'</MESSAGE>';

			Connection.Publish(testMessage);
		
			counter++;
			setTimeout(publishMessage, 100);
			
			console.log("Published Message #" + counter);
		}
	};
	
	publishMessage();
});
