------------------------ UDP SOCKET ------------------------
print ("----- testing UDP Socket -----")
uds = GraVitoN.Core.UDP_Socket(7357)
uds:sendString("hell'o peer!\n", "127.0.0.1", 7358)
s = uds:recvString ()
print (s)
