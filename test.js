

var ErlNodeModule = require('./build/Release/erlnode');


console.log("Started");

var erlNode = ErlNodeModule.createNode("testnode");
console.log("node created");
console.log(erlNode.getName());

