
===== ErlNode ===========

This is erlang interface wrapper for node.js 

==== How to build =======

1. node-waf distclean configure build
2. erlc enode.erl

==== How to run =========

Firstly change `destNode` to e1@<your host name> in test.js

Order is important:
1. erl -sname e1 -setcookie secret
2. node ./test.js
 
==== Current Performance ====== 

Machine: 
i7 2600K
16GB RAM 
Ubuntu 11.04 (latest update)

------ summary
6M of 10M requests are handled

4.1Gb - node process
117Mb - erlang process 

after 5.5M significant pauses 


[![endorse](http://api.coderwall.com/chernser/endorsecount.png)](http://coderwall.com/chernser)

 
