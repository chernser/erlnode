/**
 * erlnode_connection.cc - ErlNodeConnection implementation
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */
#define BUILDING_NODE_EXTENSION
#include <iostream>
#include <sstream>
#include <cstdio>

/* Nodejs includes */
#include <v8.h>
#include <node.h>
#include <node_version.h>

#include <erl_interface.h>
#include <ei.h>

#include "erlnode.h"


using namespace node;
using namespace v8;

Persistent<Function> ErlNode::ErlNodeConnection::objTemplate;

void ErlNode::ErlNodeConnection::Init() { 

    Local<FunctionTemplate> tmpl = FunctionTemplate::New(New);

	tmpl->SetClassName(String::NewSymbol(ERL_NODE_CONNECTION_CLASS));
	tmpl->InstanceTemplate()->SetInternalFieldCount(1);

	// .protorype //
    
    // send()
    Local<FunctionTemplate> sendFunc = FunctionTemplate::New(send);
    tmpl->PrototypeTemplate()->Set(String::NewSymbol("send"), sendFunc->GetFunction());
    
	ErlNode::ErlNodeConnection::objTemplate = Persistent<Function>::New(tmpl->GetFunction());  
}

ErlNode::ErlNodeConnection::ErlNodeConnection(const char* nodeId, const char* endpoint, ErlNode& p)  : parent(p) { 
    this->nodeId = nodeId;
    this->endpoint = endpoint;
}

ErlNode::ErlNodeConnection::~ErlNodeConnection() { 
    delete this->nodeId;
    delete this->endpoint;
}

Handle<Value> ErlNode::ErlNodeConnection::New(const Arguments& args) { 

}
        
Handle<Value> ErlNode::ErlNodeConnection::send(const Arguments& args) { 
    HandleScope scope;
    
    int32_t resultCode = 0; // OK    
    if (!args[0]->IsObject()) { // message
        resultCode = -110;
    } else { 
        ETERM* data = NULL;

        try { 
            ErlNodeConnection* that = ObjectWrap::Unwrap<ErlNodeConnection>(args.This());
            data = v8ObjectToETerm(Local<Object>::Cast(args[0]));

            resultCode = that->send((const _eterm*)data);
        } catch (...) {                 
            resultCode = -1000;
        }
        
        if (data != NULL) {
            erl_free_compound(data);
        }
    }

    return scope.Close(Integer::New(resultCode));
}

int32_t ErlNode::ErlNodeConnection::send(const _eterm* data) { 
    std::cout << "nodeId: " << this->nodeId << "\n";
    static int fd = 0;
    
    if (fd == 0) {
        fd = erl_connect((char*)nodeId);
    
        if (fd <= 0) {
            std::cout << "Failed to connect\n";
            return -300;
        }
    }
   
    if (erl_reg_send(fd, (char*)endpoint, (ETERM*)data) != 1) { 
        erl_close_connection(fd);
        fd = 0;
        return -310;
    }
    
    return 0;
}




