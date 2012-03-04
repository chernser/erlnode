#define BUILDING_NODE_EXTENSION
#ifndef _ERL_NODE_H
#define _ERL_NODE_H

#include <node.h>
#include <node_object_wrap.h>
#include <string>


using namespace node;

#define CLASS_NAME "ErlNode"

class ErlNode : public node::ObjectWrap {


public:
	  static void Init();
	  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

private:


	  static v8::Persistent<v8::Function> constructor;
	  static v8::Handle<v8::Value> New(const v8::Arguments& args);

	  // ErlNode API (see also: http://www.erlang.org/doc/tutorial/cnode.html)
	  static v8::Handle<v8::Value> getName(const v8::Arguments& args);



private:
	 v8::Local<v8::String> name;
};

#endif
