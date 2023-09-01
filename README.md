Node.js Bindings for the Goddard Mission Services Evolution Center (GMSEC) API
==============================================================================

Required packages:

* [GMSEC API](http://sourceforge.net/projects/gmsec/) 3.1
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

		string testMessage = "<MESSAGE SUBJECT="GMSEC.FREEFLYER.PUBLISHER.SC.POSITION.UPDATE" KIND="PUBLISH">
								<FIELD TYPE="U16" NAME="CONNECTION-ID">3</FIELD>
								<FIELD TYPE="F64" NAME="DX">999.409</FIELD>
								<FIELD TYPE="F64" NAME="DY">-505.047</FIELD>
								<FIELD TYPE="F64" NAME="DZ">6983.1</FIELD>
								<FIELD TYPE="STRING" NAME="EpochText">Apr 06 2011 00:01:00.000</FIELD>
								<FIELD TYPE="F64" NAME="Latitude">81.0011</FIELD>
								<FIELD TYPE="F64" NAME="Longitude">138.968</FIELD>
								<FIELD TYPE="STRING" NAME="MW-INFO">gmsec_mb</FIELD>
								<FIELD TYPE="STRING" NAME="NODE">AWGXMVYQ1</FIELD>
								<FIELD TYPE="U16" NAME="PROCESS-ID">9344</FIELD>
								<FIELD TYPE="STRING" NAME="PUBLISH-TIME">2013-038-17:46:17.243</FIELD>
								<FIELD TYPE="STRING" NAME="SCName">Aqua</FIELD>
								<FIELD TYPE="STRING" NAME="SimulationName"></FIELD>
								<FIELD TYPE="STRING" NAME="UNIQUE-ID">GMSEC_AWGXMVYQ1_9344_3_2</FIELD>
								<FIELD TYPE="STRING" NAME="USER-NAME"></FIELD>
								<FIELD TYPE="F64" NAME="X">999.409</FIELD>
								<FIELD TYPE="F64" NAME="Y">-505.047</FIELD>
								<FIELD TYPE="F64" NAME="Z">6983.1</FIELD>
							</MESSAGE>";

		Connection.Publish(testMessage);
    });

Build Instructions (Windows x86)
-------

(Prereqs: Python needs to be installed and added to your system PATH)

1. git clone https://github.com/jpf200124/gmsec-js.git gmsec-js
2. git submodule update --init
3. [Download and extract Gmsec 3.1](http://sourceforge.net/projects/gmsec/files/GMSEC%20API%203.1/GMSEC_API_3.1_WinXP_VC6.tar.gz/download) to /gmsec-js/deps/gmsec folder. (Or download to wherever you like and change the build files)
4. cd /gmsec-js/deps/node.js
5. Run vcbuild.bat
6. cd /gmsec-js/build
7. Open gmsec-js.sln and build
8. cd /gmsec-js/build/Release or /gmsec-js/build/Debug and copy gmsec-js.node to the /gmsec-js/deps/node.js/Release folder.
9. cd /gmsec-js/deps/gmsec/bin
10. copy all .dll files to the  /gmsec-js/deps/node.js/Release folder.
11. Turn on the GMSEC Mbserver located in  /gmsec-js/deps/gmsec/MBServer.exe
12. Run the simple.js example in /gmsec-js/examples/simple/ folder to test if the addon works with node and the messagebus.

Notes:

1. IF you run into build problems the result is most likely  problems with the version of the gmsec api you downloaded. 
2. For gmsec 3.1 on windows 7 you need to use the WinXP_VC6 build. 
3. 3.4 has builds for Win7 but they have yet to be tested. 
4. Currently the VS solution only supports Win32.

Deploy Instructions (Windows)
------
1. Repeat steps 8 thru 10 in the build instructions but deploy to your installed node system folder. (Make sure that the version of your installed node version is 0.8.18 or above. (Otherwise node will fail to load the module)

Build Instructions (Linux)
-------
Not yet available. Still need to create makefiles and get gmsec compiled on something other than redhat.