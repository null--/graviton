------------------------ TCP CLIENT ------------------------
print ("----- testing TCP Client -----")
cli = GraVitoN.Core.TCP_Client("127.0.0.1", 7357)
cli:connect()
data = cli:recvString()
print (data)
cli:sendString(data)
