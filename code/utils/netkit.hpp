#ifndef _GVN_NETKIT_HEAD_
#define _GVN_NETKIT_HEAD_

#include <graviton.hpp>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std;

/// @todo android, windows, bsd, solaris
#if defined(INFO_OS_LINUX) || defined(INFO_OS_OSX)
    #include <stdint.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif
#ifdef INFO_OS_WINDOWS
    #include <winsock2.h>
#endif

namespace GraVitoN
{

namespace Utils
{

namespace Netkit
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool isAValidIPv4(const string &str)
{
    if( str.size() < 7 )
        return false;
    if( str.size() > 15 )
        return false;

    int dot = 0;
    for(size_t i = 0; i < str.size(); ++i)
        if( str[i] == '.' )
            ++dot;
        else if( !isdigit(str[i]) )
            return false;
    return dot == 3;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
string dnsLookup4(const char *domain)
{
    //for(int i=0; domain[i] != '\0'; ++i)
    //    cout << (int)domain[i] << endl;

    hostent *m_hostent;
    m_hostent = gethostbyname( domain );
    if( !m_hostent )
        return "";
    unsigned int addr;
    memcpy((void*)&addr, (void*)*m_hostent->h_addr_list, 4);

    return string(
                inet_ntoa(
                    *(in_addr*)&addr
                    )
                );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// host to network: 32bit
unsigned int hostToNet_32(const unsigned int &val)
{
    return ntohl(val);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// host to network: 16bit
unsigned int hostToNet_16(const unsigned short &val)
{
    return htons(val);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// network to host: 32 bit
unsigned int netToHost_32(const unsigned int &val)
{
    return ntohl(val);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// network to host: 16 bit
unsigned int netToHost_16(const unsigned short &val)
{
    return ntohs(val);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
string hexToStrIPv4(const unsigned int hex)
{
    if(!hex)
        return "";

    char ipc[16];
    unsigned char hip[4];
    memcpy((void*)hip, (void*)&hex, 4);
    snprintf(ipc, sizeof(ipc), "%u.%u.%u.%u",
             (unsigned int)hip[3],
             (unsigned int)hip[2],
             (unsigned int)hip[1],
             (unsigned int)hip[0]);
    return string(ipc);

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
unsigned int strToHexIPv4(string str)
{
    string res;

    unsigned int hexip;
    unsigned char hex[4];
    sscanf (str.c_str(), "%u.%u.%u.%u",
            (unsigned int *)&hex[3],
            (unsigned int *)&hex[2],
            (unsigned int *)&hex[1],
            (unsigned int *)&hex[0]);

    memcpy((void*)&hexip, (void*)hex, 4);
    return hexip;
}

}
}
}
#endif
