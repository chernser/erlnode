#define BUILDING_NODE_EXTENSION
#ifndef _ERL_NODE_H
#define _ERL_NODE_H

#include <node.h>
#include <node_object_wrap.h>
#include <string>


using namespace node;

#define CLASS_NAME "ErlNode"

struct _eterm;
class ErlNode : public node::ObjectWrap {

    

public:
    static void Init();
    static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

private:
     //ErlNode(v8::Local<v8::String> name, v8::Local<v8::String> secretCookie, v8::Local<v8::Number> instanceId);
     ErlNode(const char* name, const char* secretCookie, int32_t instanceId);
     ~ErlNode();

    static v8::Persistent<v8::Function> constructor;
    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    // ErlNode API (see also: http://www.erlang.org/doc/tutorial/cnode.html)
    static v8::Handle<v8::Value> getName(const v8::Arguments& args);
    static v8::Handle<v8::Value> getInstanceId(const v8::Arguments& args);   

    static v8::Handle<v8::Value> send(const v8::Arguments& args);
    int32_t send(const char* nodeId, const char* endpoint, const _eterm* data);
    
    static _eterm* jsObjectToETerm(const v8::Local<v8::Object> data);

private:

    const static char* DEFAULT_NODE_NAME;
    const static char* DEFAULT_SECRET;

    int32_t instanceId;
    const char* name;
    


};

#endif
