/**
 * erlnode.cc - ErlNode implementation
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */
#define BUILDING_NODE_EXTENSION
#include <iostream>
#include <sstream>
#include <cstdio>

/* Nodejs and V8 includes */
#include <v8.h>
#include <node.h>
#include <node_version.h>

/* Erlang includes */
#include <erl_interface.h>
#include <ei.h>

#include "erlnode.h"

using namespace v8;
using namespace node;

Persistent<Function> ErlNode::constructor;

const char* ErlNode::DEFAULT_NODE_NAME = "cnode@localhost";

const char* ErlNode::DEFAULT_SECRET = "secret";


ErlNode::ErlNode(const char* name, const char* secretCookie, int32_t instanceId) {  

    this->name = name;
    this->instanceId = instanceId;
    std::cout << "this node name: " << this->name << ", instanceId: " << this->instanceId << "\n";
	erl_connect_init(1, (char*)secretCookie, this->instanceId);
}

ErlNode::~ErlNode() { 
    delete this->name;
}


Handle<Value> ErlNode::New(const Arguments& args) {	
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
    
    // connect()
    Local<FunctionTemplate> connectFunc = FunctionTemplate::New(connect);
    tmpl->PrototypeTemplate()->Set(String::NewSymbol("connect"), connectFunc->GetFunction());

	constructor = Persistent<Function>::New(tmpl->GetFunction());


    ErlNode::ErlNodeConnection::Init();
    
	// erl_interface memory init function 
    erl_init(NULL, 0);
}


Handle<Value> ErlNode::NewInstance(const Arguments& args) {
	HandleScope scope;

	const unsigned argc = 1;
	Handle<Value> argv[argc] = { args[0] };
	Local<Object> instance = constructor->NewInstance(argc, argv);

	return scope.Close(instance);
}

Handle<Value> ErlNode::getName(const Arguments& args){
	HandleScope scope;

	ErlNode* that = ObjectWrap::Unwrap<ErlNode>(args.This());
	return scope.Close(String::New(that->name));
}

Handle<Value> ErlNode::getInstanceId(const Arguments& args) { 
    HandleScope scope;
    
    ErlNode* that = ObjectWrap::Unwrap<ErlNode>(args.This());
    return scope.Close(Integer::New(that->instanceId));
}
      
Handle<Value> ErlNode::connect(const Arguments& args) { 
    HandleScope scope;
    
    if (!args[0]->IsString()) { 
        throw "Invalid nodeId value";
    } else if (!args[1]->IsString()) { 
        throw "Invalid endpoint value";
    }
    
    Local<String> nodeIdArg = Local<String>::Cast(args[0]);
    Local<String> endpointArg = Local<String>::Cast(args[1]);    

    char* nodeId = new char[nodeIdArg->Length() + 1];
    char* endpoint = new char[endpointArg->Length() + 1];
    
    nodeIdArg->WriteAscii(nodeId);
    endpointArg->WriteAscii(endpoint);


    ErlNode* erlNode = ObjectWrap::Unwrap<ErlNode>(args.This());
    ErlNodeConnection* connection = erlNode->connect(nodeId, endpoint);
    Local<Object> connectionObj = ErlNodeConnection::objTemplate->NewInstance();
    connection->Wrap(connectionObj);    
    
    return scope.Close(connectionObj);
}
    
ErlNode::ErlNodeConnection* ErlNode::connect(const char* nodeId, const char* endpoint) {
    // TODO: implement saving all child connections to linked list 
    return new ErlNodeConnection(nodeId, endpoint, *this);
}



