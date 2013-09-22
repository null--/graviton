------------------------ TCP SERVER ------------------------
print ("----- testing TCP Server -----")

local response=function(cli)
    print ("RESPONSE CALLED:")
    d = cli:recvString()
    print( d )
    cli:sendString(d)
    cli:close()
end

srv = GraVitoN.Core.TCP_Server(7357,response,true)
srv:run()
