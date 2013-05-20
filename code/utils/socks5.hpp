/// @todo socks5
#include <graviton.hpp>

namespace GraVitoN
{

namespace Utils
{

class SOCKS5
{
protected:
    /// rfc1928: REQUEST
    ///    The SOCKS request is formed as follows:
    ///
    ///         +----+-----+-------+------+----------+----------+
    ///         |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
    ///         +----+-----+-------+------+----------+----------+
    ///         | 1  |  1  | X'00' |  1   | Variable |    2     |
    ///         +----+-----+-------+------+----------+----------+
    ///
    ///      Where:
    ///
    ///           o  VER    protocol version: X'05'
    ///           o  CMD
    ///              o  CONNECT X'01'
    ///              o  BIND X'02'
    ///              o  UDP ASSOCIATE X'03'
    ///           o  RSV    RESERVED
    ///           o  ATYP   address type of following address
    ///              o  IP V4 address: X'01'
    ///              o  DOMAINNAME: X'03'
    ///              o  IP V6 address: X'04'
    ///           o  DST.ADDR       desired destination address
    ///           o  DST.PORT desired destination port in network octet
    ///              order
    ///
    const static unsigned char VER                              = 0x05;
    const static unsigned char CMD_CONNECT                      = 0x01;
    const static unsigned char CMD_BIND                         = 0x02;
    const static unsigned char CMD_UDP                          = 0x03;
    const static unsigned char RSV                              = 0x00;
    const static unsigned char ATYP_IPV4                        = 0x01;
    const static unsigned char ATYP_DOMAINNAME                  = 0x03;
    const static unsigned char ATYP_IPV6                        = 0x04;

    /// rfc1928: REPLY
    ///    +----+-----+-------+------+----------+----------+
    ///    |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
    ///    +----+-----+-------+------+----------+----------+
    ///    | 1  |  1  | X'00' |  1   | Variable |    2     |
    ///    +----+-----+-------+------+----------+----------+
    ///
    /// Where:
    ///
    ///      o  VER    protocol version: X'05'
    ///      o  REP    Reply field:
    ///         o  X'00' succeeded
    ///         o  X'01' general SOCKS server failure
    ///         o  X'02' connection not allowed by ruleset
    ///         o  X'03' Network unreachable
    ///         o  X'04' Host unreachable
    ///         o  X'05' Connection refused
    ///         o  X'06' TTL expired
    ///         o  X'07' Command not supported
    ///         o  X'08' Address type not supported
    ///         o  X'09' to X'FF' unassigned
    ///      o  RSV    RESERVED
    ///      o  ATYP   address type of following address
    const static unsigned char REP_SUCCEEDED                    = 0x00;
    const static unsigned char REP_GENERIC_FAILIURE             = 0x01;
    const static unsigned char REP_CONNECTION_NOT_ALLOWED       = 0x02;
    const static unsigned char REP_NETWORK_UNREACHABLE          = 0x03;
    const static unsigned char REP_HOST_UNREACHABLE             = 0x04;
    const static unsigned char REP_CONNECTION_REFUSED           = 0x05;
    const static unsigned char REP_TTL_EXPIRED                  = 0x06;
    const static unsigned char REP_COMMAND_NOT_SUPPORTED        = 0x07;
    const static unsigned char REP_ADDRESS_TYPE_NOT_SUPPORTED   = 0x08;

public:
    SOCKS5() {}
    virtual ~SOCKS5() {}
};

}
}
