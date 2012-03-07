/**
 * test.js - simple test just to show how use mode
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */


var ErlNodeModule = require('./build/Release/erlnode');


console.log("Started");


var erlNode = ErlNodeModule.createNode({name: "czap", instanceId: 123, secretCookie: "secret"});
console.log("node created");
console.log(erlNode);
console.log("name: " + erlNode.getName());
console.log("instanceId: " + erlNode.getInstanceId());

var destNode = "e1@localhost";
var endpoint = "elogger";

var destNodeConnection = erlNode.connect(destNode, endpoint);
console.log(destNodeConnection);
console.log(destNodeConnection.send());
console.log(destNodeConnection.send({msg: "Hello brother!"}));

