Node.js Bindings for the Goddard Mission Services Evolution Center (GMSEC) API
==============================================================================

Required packages:

* [GMSEC API 3.1](http://sourceforge.net/projects/gmsec/)
* [node.js](http://nodejs.org/)

Recommended packages:

* [Socket.IO](http://socket.io/)
* [MongoDB](http://www.mongodb.org/) along with the node.js bindings `npm install mongodb`
* [Mongoose](http://blog.learnboost.com/blog/mongoose/)
* [libxmljs](https://github.com/polotek/libxmljs)

Overview
--------

gmsec-js provides Node.js bindings to the GMSEC API in order to facilitate the distribution of SOA resources to the web. By exploiting the low-level functionality of Node.js and its goal of integrating evented IO to the V8 Javascript engine, we can deploy the GMSEC API in a high-performance, event-driven environment that can easily be integrated into web applications. Ideally, it can be implemented as a data proxy to existing web services, allowing you to stream GMSEC-related data into the web browser without having to re-design pre-existing web services when using other libraries such as Socket.IO, MongoDB, etc.

Example
-------

    GMSEC = require('gmsec');
    
    var Connection = new GMSEC.Connection();

    Connection.Connect(function(){
        console.log('Connected to server!')
    });

    Connection.Subscribe('GMSEC.TEST.SUBJECT', function(msg){
        console.log('Received message.');
        console.log(msg.Fields["SampleField"].Value);
    });
