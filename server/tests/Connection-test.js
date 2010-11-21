var GMSEClib = require('../src/build/default/gmsec');

Connection = new GMSEClib.Connection();
Connection.Connect(function(data){
  console.log(data);
}); 

