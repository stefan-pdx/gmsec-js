Node.js Bindings for the Goddard Mission Services Evolution Center (GMSEC) API
==============================================================================

Required packages:

* [GMSEC API](http://sourceforge.net/projects/gmsec/) 3.1 or newer
* [node.js](http://nodejs.org/) 0.8.18 or newer

Optional packages (for examples):

* [Socket.IO](http://socket.io/)
* [MongoDB](http://www.mongodb.org/) along with the node.js bindings `npm install mongodb`
* [Mongoose](http://blog.learnboost.com/blog/mongoose/)
* [libxmljs](https://github.com/polotek/libxmljs)

Overview
--------

gmsec-js provides Node.js bindings to the GMSEC API in order to facilitate the distribution of SOA resources to the web. By exploiting the low-level functionality of Node.js and its goal of integrating evented IO to the V8 Javascript engine, we can deploy the GMSEC API in a high-performance, event-driven environment that can easily be integrated into web applications. Ideally, it can be implemented as a data proxy to existing web services, allowing you to stream GMSEC-related data into the web browser without having to re-design pre-existing web services when using other libraries such as Socket.IO, MongoDB, etc.

Example
-------

    GMSEC = require('/path/to/gmsec');	//Note: You must change to where you installed the addon.
    
    var Connection = new GMSEC.Connection();

    Connection.Connect("127.0.0.1", function(){
        console.log('Connected to server!')

		Connection.Subscribe('GMSEC.TEST.SUBJECT', function(msg){
        	console.log('Received message.');
			console.log(msg);
	    });
    });

Build Instructions (Windows)
-------

(Note: Python needs to be installed and added to your system PATH)

1. git clone https://github.com/jpf200124/gmsec-js.git gmsec-js
2. git submodule update --init
3. Download and extract Gmsec 3.1 or higher to /deps/gmsec folder. (Or download to wherever you like and change the build files)
4. cd /deps/node.js
5. Run vcbuild.bat
6. cd /build
7. Open gmsec-js.sln and build
8. cd /build/Release or /build/Debug and copy gmsec-js.node to the /deps/node.js/Release folder.
9. cd /deps/gmsec/bin
10. copy all .dll files to the /deps/node.js/Release folder.
11. Turn on the GMSEC Mbserver.
12. Run the simple.js example in /examples/simple/ folder to test if the addon works with node and the messagebus.

Deploy Instructions (Windows)
------
1. Repeat steps 8 thru 10 in the build instructions but deploy to your installed node system folder. (Make sure that the version of your installed node version is 0.8.18 or above. (Otherwise node will fail to load the module)

Build Instructions (Linux)
-------
Not yet available. Still need to create makefiles and get gmsec compiled on something other than redhat.