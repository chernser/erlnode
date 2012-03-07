/**
 * enode.h - header file for node.js module
 *
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */
#define BUILDING_NODE_EXTENSION
#ifndef _ERL_NODE_H
#define _ERL_NODE_H

#include <node.h>
#include <node_object_wrap.h>
#include <string>

/* Converter prototypes */


using namespace node;
using namespace v8;

#define CLASS_NAME "ErlNode"

#define ERL_NODE_CONNECTION_CLASS "ErlNodeConnection"

struct _eterm;

/**
 * Converter prototypes
 */
struct _eterm* v8ObjectToETerm(const Local<Object> data);



/**
 * Holder for connections 
 * Represents current node
 *
 * ErlNode API (see also: http://www.erlang.org/doc/tutorial/cnode.html)
 */
class ErlNode : public node::ObjectWrap {
   
public:
    static void Init();
    static Handle<Value> NewInstance(const Arguments& args);

    /**
     *  Handles connection to remote node 
     *
     */
    class ErlNodeConnection : public node::ObjectWrap { 
    friend class ErlNode; 
    public:        
        ErlNodeConnection(const char* nodeId, const char* endpoint, ErlNode& p);
        ~ErlNodeConnection();

        static void Init();
            
    protected:
        static Handle<Value> New(const Arguments& args);

        /**
         * send(message) - sends message which is javascript object to
         *                  endpoint of current node
         *
         */
        static Handle<Value> send(const Arguments& args);
        int32_t send(const _eterm* data);
        
    private: 
        static Persistent<Function> objTemplate;

        const char* nodeId; // full node id node@host.domain 
        const char* endpoint; // registered endpoint 
        const ErlNode& parent; // parent erlang node
    };

protected:
     /**
      * name - current node short name 
      * secretCookie - secret token to authinticate allowed nodes
      * instanceId - current node's instance id
      */
     ErlNode(const char* name, const char* secretCookie, int32_t instanceId);
     ~ErlNode();

    static Persistent<Function> constructor;
    static Handle<Value> New(const Arguments& args);

    /**
     * Returns current node's name 
     */
    static Handle<Value> getName(const Arguments& args);
    /**
     * Returns current node's instance id
     */
    static Handle<Value> getInstanceId(const Arguments& args);   
    
    /**
     * connect(nodeId, endpoint) - creates new connection to nodeId and its endpoint 
     *
     */
    static Handle<Value> connect(const Arguments& args);
    ErlNode::ErlNodeConnection* connect(const char* nodeId, const char* endpoint);
    
private:

    const static char* DEFAULT_NODE_NAME;
    const static char* DEFAULT_SECRET;

    int32_t instanceId;
    const char* name;
};

#endif
