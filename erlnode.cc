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
    
    // send()
    Local<FunctionTemplate> sendFunc = FunctionTemplate::New(send);
    tmpl->PrototypeTemplate()->Set(String::NewSymbol("send"), sendFunc->GetFunction());

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

int32_t ErlNode::send(const _eterm* data, const char* nodeId) { 

    return 0;
}
    
_eterm* ErlNode::jsObjectToETerm(const v8::Local<v8::Object> data) { 

    return NULL;
}


v8::Handle<v8::Value> ErlNode::send(const v8::Arguments& args) { 
    HandleScope scope;
    
    int32_t resultCode = 0; // OK
    if (!args[0]->IsObject()) { 
        resultCode = -100;
    } else if (!args[1]->IsString()) { 
        resultCode = -200;   
    } else { 
        Local<String> destNodeStr = Local<String>::Cast(args[1]);
        if (destNodeStr->Length() < 1) { 
            resultCode = -210;
        } else { 
            char* destinationNodeId = new char[destNodeStr->Length() + 1];
            try { 
                destNodeStr->WriteAscii(destinationNodeId);
                ErlNode* that = ObjectWrap::Unwrap<ErlNode>(args.This());
                ETERM* data = ErlNode::jsObjectToETerm(Local<Object>::Cast(args[0]));
                resultCode = that->send(data, destinationNodeId);
            } catch (...) { 
                resultCode = -1000;
            }
            delete destinationNodeId;
        }
    }

    return scope.Close(Integer::New(resultCode));
}




