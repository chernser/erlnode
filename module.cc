/**
 * enode.cc - module for node.js
 *
 * Purpose:
 * 		Creating erlang nodes to allow node.js script 'talk' to erlang application
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */
#define BUILDING_NODE_EXTENSION
#include <iostream>

/* Nodejs includes */
#include <v8.h>
#include <node.h>
#include <node_version.h>

#include "erlnode.h"

using namespace v8;


/**
 * createNode - creates erlang node
 */
Handle<Value> createNode(const Arguments& args) {
  HandleScope scope;
  return scope.Close(ErlNode::NewInstance(args));
}


void init(Handle<Object> target) {
	ErlNode::Init();

	// Add functions to root object
	target->Set(String::NewSymbol("createNode"),
			FunctionTemplate::New(createNode)->GetFunction());
}
NODE_MODULE(erlnode, init)

