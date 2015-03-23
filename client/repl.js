var arDrone = require('ar-drone');
var client  = arDrone.createClient({ip: "192.168.1.111"});
client.createRepl();
