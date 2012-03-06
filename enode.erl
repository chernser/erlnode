-module(enode).

-import(io).
-export([start/0]).


listen() -> 
    receive
        Msg -> 
            io:fwrite("Msg: ~p~n", [Msg])
    end,
    listen().



start() ->
    io:fwrite("Start listening for messages"),
    register(elogger, self()),
    listen().
    
  
    
            

