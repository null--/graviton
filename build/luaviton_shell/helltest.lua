tcp_client = gvn.core.TCP_Client("127.0.0.1", 7357)

if( tcp_client:connect() ) then
    print "Connection Established"
    data = "HELL'O\n"
    tcp_client:sendString(data)

    tcp_client:recvString(data)

    tcp_client:close()
else
    print "Connection Refused"
end
