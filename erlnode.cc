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

const char* ErlNode::DEFAULT_NODE_NAME = "cnode";

const char* ErlNode::DEFAULT_SECRET = "secret";


ErlNode::ErlNode(const char* name, const char* secretCookie, int32_t instanceId) {  

    this->name = name;
    this->instanceId = instanceId;

	erl_connect_init(1, (char*)this->name, this->instanceId);
}

ErlNode::~ErlNode() { 
    delete this->name;
}


v8::Handle<v8::Value> ErlNode::New(const v8::Arguments& args) {	
    char* name = (char*)ErlNode::DEFAULT_NODE_NAME;
    char* secretCookie = (char*)ErlNode::DEFAULT_SECRET;
    int32_t instanceId = 0;


    if (args[0]->IsObject()) { 
        Local<Object> obj = Local<Object>::Cast(args[0]);    
        Local<Value> value;
        
        value = obj->Get(String::New("instanceId"));
        if (!value->IsUndefined() && value->IsNumber()) 
            instanceId = Local<Number>::Cast(value)->Value();
            
        value = obj->Get(String::New("name"));
        if (!value->IsUndefined() && value->IsString()) { 
            Local<String> string = Local<String>::Cast(value);
            name = new char[string->Length() + 1];
            string->WriteAscii(name);            
        }
        
        value = obj->Get(String::New("secretCookie"));
        if (!value->IsUndefined() && value->IsString()) { 
            Local<String> string = Local<String>::Cast(value);
            secretCookie = new char[string->Length() + 1];
            string->WriteAscii(secretCookie);
        }
    } else { 
        // TODO: throw exception or do something   
    }

	ErlNode* erlNode = new ErlNode((const char*)name, (const char*)secretCookie, instanceId);


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
	return scope.Close(String::New(that->name));
}

v8::Handle<v8::Value> ErlNode::getInstanceId(const v8::Arguments& args) { 
    HandleScope scope;
    
    ErlNode* that = ObjectWrap::Unwrap<ErlNode>(args.This());
    return scope.Close(Integer::New(that->instanceId));
}




