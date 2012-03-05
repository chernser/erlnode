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


//ErlNode::ErlNode(v8::Local<v8::String> name, v8::Local<v8::String> secretCookie, v8::Local<v8::Number> instanceId) {
ErlNode::ErlNode(v8::String* name, v8::Local<v8::String> secretCookie, v8::Local<v8::Number> instanceId) {  


    this->name = name;
    this->instanceId = instanceId;

//	erl_connect_init(1, )
}

ErlNode::~ErlNode() { 

}


v8::Handle<v8::Value> ErlNode::New(const v8::Arguments& args) {
	
//	v8::Local<v8::String> name;
	v8::String* name;
    v8::Local<v8::String> secretCookie;
    v8::Local<v8::Number> instanceId;

	if (!args[0]->IsUndefined() && args[0]->IsString()) {
        name = *(args[0]->ToString());
	}  else { 
        name = *String::New("cnode");
    }

    if (!args[1]->IsUndefined() && args[1]->IsString()) { 
        secretCookie = Local<String>::New(args[1]->ToString());
    } else { 
        secretCookie = String::New("secret");
    }

    if (!args[2]->IsUndefined() && args[2]->IsNumber()) { 
        instanceId = Local<Number>::New(args[2]->ToNumber());
    } else { 
        instanceId = Number::New(0);
    }

	ErlNode* erlNode = new ErlNode(name, secretCookie, instanceId);


	erlNode->Wrap(args.This());

	return args.This();
}

void ErlNode::Init() {

	Local<FunctionTemplate> tmpl = FunctionTemplate::New(New);

	tmpl->SetClassName(String::NewSymbol(CLASS_NAME));
	tmpl->InstanceTemplate()->SetInternalFieldCount(2);


	// .protorype //

	// getName()
	Local<FunctionTemplate> getNameFunc = FunctionTemplate::New(getName);
	tmpl->PrototypeTemplate()->Set(String::NewSymbol("getName"), getNameFunc->GetFunction());

    // getInstanceId()
    Local<FunctionTemplate> getInstanceIdFunc = FunctionTemplate::New(getInstanceId);
    tmpl->PrototypeTemplate()->Set(String::NewSymbol("getInstanceId"), getInstanceIdFunc->GetFunction());

	constructor = Persistent<Function>::New(tmpl->GetFunction());

	// erl_interface memory init function 
	erl_init(NULL, 0);
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
	return scope.Close(Local<String>(that->name));
}

v8::Handle<v8::Value> ErlNode::getInstanceId(const v8::Arguments& args) { 
    HandleScope scope;
    
    ErlNode* that = ObjectWrap::Unwrap<ErlNode>(args.This());
    return scope.Close(Local<Number>::New(that->instanceId));
}




