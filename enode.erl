-module(enode).

-import(io).
-export([start/0]).

start() ->
    io:fwrite("Start listening for messages"),
    io:fwrite("self: ~p~n", [self()]),
    receive
        Msg -> 
            io:fwrite("Msg: ~p~n", [Msg])
    end.
            

