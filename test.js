

var ErlNodeModule = require('./build/Release/erlnode');


console.log("Started");


var erlNode = ErlNodeModule.createNode();
console.log("node created");
console.log(erlNode);
console.log(erlNode.getName());
console.log(erlNode.getInstanceId());


var erlNode = ErlNodeModule.createNode("testnode", "test", 123);
console.log("node created");
console.log(erlNode);
console.log(erlNode.getName());
console.log(erlNode.getInstanceId());



