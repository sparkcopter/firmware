var arDrone = require('ar-drone');
var client  = arDrone.createClient({ip: (process.env.IP || "192.168.1.111")});
client.createRepl();
