print("Beginning Test...")

ss = "1"
testStrRef(ss)
print(ss)

------------------------ CALLBACK ------------------------
--print ("----- testing CallBack -----")
--local callBack=function(a)
--    print ("--- LUA CallBack")
--    s  = a:getName()
--    print ("--> ", s)
--end
--callLua(callBack)

------------------------ THREAD ------------------------
--print ("----- testing Thread -----")

--local mainLoop=function()
--    print("Inside Main Loop")
--    return;
--end

--trd = GraVitoN.Core.Thread(mainLoop);

--trd:run();

--while trd:isActive() do
--    print("*")
--    gvn.core.sleep(10)
--end

--trd:stop()

------------------------ TCP CLIENT ------------------------
--print ("----- testing TCP Client -----")
--cli = GraVitoN.Core.TCP_Client("127.0.0.1", 7357)
--cli:connect()
--data = cli:recvString()
--print (data)
--cli:sendString(data)

------------------------ TCP SERVER ------------------------
--print ("----- testing TCP Server -----")

--local response=function(cli)
--    print ("RESPONSE CALLED:")
--    d = cli:recvString()
--    print( d )
--    cli:sendString(d)
--    cli:close()
--end

--srv = GraVitoN.Core.TCP_Server(7357,response,true)
--srv:run()

------------------------ UDP SOCKET ------------------------
--print ("----- testing UDP Socket -----")
--uds = GraVitoN.Core.UDP_Socket(7357)
--uds:sendString("hell'o peer!\n", "127.0.0.1", 7358)
--s = uds:recvString ()
--print (s)
