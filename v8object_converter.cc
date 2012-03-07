/**
 * v8object_converter.cc -  v8 object converter 
 *
 * Author: Sergey Chernov <chernser@gmail.com>
 */

#include <iostream>
#include <sstream>
#include <cstdio>

/* Nodejs includes */
#include <v8.h>
#include <erl_interface.h>
#include <ei.h>

using namespace v8;

/**
 * Converts V8 Object to Erlang Term
 *
 */
_eterm* v8ObjectToETerm(const Local<Object> data) { 

    Local<Object> currentObject = data;
    ETERM* curObjectTuple;
    ETERM** curObjectZeroLevelTerms; 
    ETERM** propertyTupleBuff = new ETERM*[2];

    bool isDone = false;
    while (!isDone) { 
            Local<Array> objectPropertyNames = currentObject->GetOwnPropertyNames();
            uint32_t numberOfProperties = objectPropertyNames->Length();

            curObjectZeroLevelTerms = new ETERM*[numberOfProperties];
            uint32_t curZeroLevelTermIndex = 0;
            for (uint32_t propertyIndex = 0; propertyIndex < numberOfProperties; ++propertyIndex) { 
                Local<String> propertyKey = Local<String>::Cast(objectPropertyNames->Get(propertyIndex));
                Local<Value> property = currentObject->Get(objectPropertyNames->Get(propertyIndex));
                char* propertyName = new char[propertyKey->Length() + 1];
                propertyKey->WriteAscii(propertyName);
                

                if (!property->IsObject()) { 
                    if (property->IsString()) { 
                        Local<String> stringValue = Local<String>::Cast(property);
                        char* string = new char[stringValue->Length() + 1];
                        stringValue->WriteAscii(string);
                        
                                
                        propertyTupleBuff[0] = erl_mk_binary(propertyName, propertyKey->Length());                        
                        propertyTupleBuff[1] = erl_mk_binary(string, stringValue->Length());
                        
                        curObjectZeroLevelTerms[curZeroLevelTermIndex++] = erl_mk_tuple(propertyTupleBuff, 2);
                        delete string;
                    } else if (property->IsInt32()) { 
                            propertyTupleBuff[0] = erl_mk_binary(propertyName, propertyKey->Length());                        
                            propertyTupleBuff[1] = erl_mk_int(property->Int32Value());
                            curObjectZeroLevelTerms[curZeroLevelTermIndex++] = erl_mk_tuple(propertyTupleBuff, 2);  
                    } else if (property->IsUint32()) {         
                            propertyTupleBuff[0] = erl_mk_binary(propertyName, propertyKey->Length());                        
                            propertyTupleBuff[1] = erl_mk_int(property->Uint32Value());
                            curObjectZeroLevelTerms[curZeroLevelTermIndex++] = erl_mk_tuple(propertyTupleBuff, 2);  
                    } else if (property->IsNumber() && (!property->IsInt32() || !property->IsUint32())) { 
                            propertyTupleBuff[0] = erl_mk_binary(propertyName, propertyKey->Length());                        
                            propertyTupleBuff[1] = erl_mk_float(property->NumberValue());
                            curObjectZeroLevelTerms[curZeroLevelTermIndex++] = erl_mk_tuple(propertyTupleBuff, 2);  
                    } else if (property->IsBoolean()) { 
                            propertyTupleBuff[0] = erl_mk_binary(propertyName, propertyKey->Length());                        
                            propertyTupleBuff[1] = erl_mk_atom(property->IsTrue() ? "true" : "false");
                            curObjectZeroLevelTerms[curZeroLevelTermIndex++] = erl_mk_tuple(propertyTupleBuff, 2);  
                    }
                } else { 
                    // handle inner object

                }
                
                delete propertyName;
            }

            /* DEBUG */
            curObjectTuple = erl_mk_list(curObjectZeroLevelTerms, curZeroLevelTermIndex);
           /* erl_print_term(stderr, curObjectTuple);
            fprintf(stderr,"~\n");
            fflush(stdout);
            */
            isDone = true;
    } 

    Local<Array> objectProperties = data->GetOwnPropertyNames();

    return curObjectTuple;
}
