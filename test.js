

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

var destNode = "erl_node@localhost";

console.log("debug: " + erlNode.send(message, destNode));


var erlNodeDefault = ErlNodeModule.createNode();
console.log("default node created");
console.log(erlNodeDefault);
console.log("name: " + erlNodeDefault.getName());
console.log("instanceId: " + erlNodeDefault.getInstanceId());



