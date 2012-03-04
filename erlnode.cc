/**
 * enode.cc - module for node.js
 *
 * Purpose:
 * 		Creating erlang nodes to allow node.js script 'talk' to erlang application
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */
#define BUILDING_NODE_EXTENSION
#include <cstdio>

/* Nodejs includes */
#include <v8.h>
#include <node.h>
#include <node_version.h>

#include <erl_interface.h>
#include <ei.h>

#include "erlnode.h"

using namespace v8;

v8::Persistent<v8::Function> ErlNode::constructor;


v8::Handle<v8::Value> ErlNode::New(const v8::Arguments& args) {
	ErlNode* erlNode = new ErlNode();

	if (!args[0]->IsUndefined()) {
		erlNode->name = Local<String>::New(args[0]->ToString());
	}
	erlNode->Wrap(args.This());

	return args.This();
}

void ErlNode::Init() {

	Local<FunctionTemplate> tmpl = FunctionTemplate::New(New);

	tmpl->SetClassName(String::NewSymbol(CLASS_NAME));
	tmpl->InstanceTemplate()->SetInternalFieldCount(1);


	// .protorype //

	// getName()
	Local<FunctionTemplate> getNameFunc = FunctionTemplate::New(getName);
	tmpl->PrototypeTemplate()->Set(String::NewSymbol("getName"), getNameFunc->GetFunction());

	constructor = Persistent<Function>::New(tmpl->GetFunction());
}


v8::Handle<Value> ErlNode::NewInstance(const v8::Arguments& args) {
	HandleScope scope;

	const unsigned argc = 1;
	Handle<Value> argv[argc] = { args[0] };
	Local<Object> instance = constructor->NewInstance(argc, argv);

	return scope.Close(instance);
}

v8::Handle<v8::Value> ErlNode::getName(const v8::Arguments& args){
	HandleScope scope;

	ErlNode* that = ObjectWrap::Unwrap<ErlNode>(args.This());
	if (that->name->IsNull())
		return scope.Close(String::New("Teszzz"));
	else
		return scope.Close(Local<String>::New(that->name));
}




