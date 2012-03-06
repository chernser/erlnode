

var ErlNodeModule = require('./build/Release/erlnode');


console.log("Started");


var erlNode = ErlNodeModule.createNode({name: "czap", instanceId: 123, secretCookie: "secret"});
console.log("node created");
console.log(erlNode);
console.log("name: " + erlNode.getName());
console.log("instanceId: " + erlNode.getInstanceId());


var message = {
    text: "hello from node js",
   seqM: 23,
   x: -13,
    y: -0.233,
    meta: { 
        seqN: 1,
        level: "debug",
        userData: { 
               point: "server.start",
               case: "good"
        },
        name: "server.logger"   
    },
    isCritical: false,
    isPersitent: true
};

var destNode = "e1@Zeus";
var endpoint = "elogger";

var result = 0;
var start = new Date();
for (var i = 0; i < 10000000; ++i) {
    message.counter = i;
    result = erlNode.send(destNode, endpoint, message);
    if (result == 0) 
        console.log(".");
    else 
        console.log(result);
}

console.log(start);
console.log(new Date());


var erlNodeDefault = ErlNodeModule.createNode();
console.log("default node created");
console.log(erlNodeDefault);
console.log("name: " + erlNodeDefault.getName());
console.log("instanceId: " + erlNodeDefault.getInstanceId());



