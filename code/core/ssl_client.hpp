#ifndef _GVN_SSL_CLIENT_HEAD_
#define _GVN_SSL_CLIENT_HEAD_

#include <graviton.hpp>
#include <core/ssl_socket.hpp>

namespace GraVitoN
{
namespace Core
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class SSL_Client
{
protected:
    bool is_dead;
    SSL_Socket::Handle sock;

    string r_ip_str;
    unsigned int r_ip_hex;
    unsigned int r_port;

    SSL_Socket::Address sa;
    SSL_CTX* ctx;
    SSL*     ssl;

protected:
    bool handshake();

public:
    SSL_Client(const string &ip, const unsigned int port);
    SSL_Client(SSL_Socket::Handle _sock, SSL_Socket::Address _sa);

    virtual ~SSL_Client();

    virtual bool connect();

    virtual bool close();

    virtual bool recv(unsigned char *&data, size_t &data_size);

    virtual string recvString()
    {
        // sdata = "";

        unsigned char *data = _null_;
        size_t data_size;
        if( SSL_Client::recv(data, data_size) )
        {
            /*
            for(size_t i = 0; i<data_size; ++i)
            {
                sdata += (char)data[i];
            }
            return true;
            */
            return string((char*)data);
        }

        return "";
    }

    virtual bool send(const unsigned char *data, const size_t &data_size);

    virtual bool sendString(const string &data)
    { return SSL_Client::send((unsigned char*)data.c_str(), data.size()); }

    virtual bool isActive();

    string getRemoteIP()
    {
        return r_ip_str;
    }

    unsigned int getRemoteIPHex()
    {
        return r_ip_hex;
    }

    unsigned int getRemotePort()
    {
        return r_port;
    }

    SSL_Socket::Handle getHandle()
    {
        return sock;
    }

    bool handshake_connect();
    bool handshake_accept();


    static bool getCertInfo(X509 *cert, string &subject, string &issuer);
    bool getRemoteCertInfo(string &subject, string &issuer);
    bool getLocalCertInfo(string &subject, string &issuer);

    string getCipher();

    bool loadPemFromFile(const string &cert_file, const string &key_file);

};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
SSL_Client::SSL_Client(const string &ip, const unsigned int port)
{
    is_dead = true;

    SSL_Socket::getInstance();
    r_ip_str = ip;
    r_port = port;

    memset (&sa, '\0', sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr (ip.c_str());   /* Server IP */
    sa.sin_port        = htons     (r_port);          /* Server Port number */

    ssl = _null_;

    ctx = SSL_CTX_new (SSLv23_client_method());
    sock = SSL_Socket::invalidSocket();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
SSL_Client::SSL_Client(SSL_Socket::Handle _sock, SSL_Socket::Address _sa)
{
    is_dead = true;

    sock = _sock;

    ssl = _null_;
    ctx = SSL_CTX_new (SSLv23_client_method());
    /// @todo get sock's remote ip address and remote port
    sa = _sa;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
SSL_Client::~SSL_Client()
{
    // SSL_Client::close();
    // if(ctx)
        // SSL_CTX_free (ctx);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::loadPemFromFile(const string &cert_file, const string &key_file)
{
    if( ctx )
        SSL_CTX_free(ctx);

    /* SSL preliminaries. We keep the certificate and key with the context. */
    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();
    ctx = SSL_CTX_new ( SSLv23_server_method() );

    if (!ctx)
    {
        Core::Logger::logItLn("[SSL SERVER] CTX Failed!");
        return false;
    }
    // SSL_CTX_set_mode(ctx, 3);

    if (SSL_CTX_use_certificate_file(ctx, cert_file.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        Core::Logger::logItLn("[SSL SERVER] SSL_CTX_use_certificate_file Failed!");
        return false;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, key_file.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        Core::Logger::logItLn("[SSL SERVER] SSL_CTX_use_PrivateKey_file Failed!");
        ERR_print_errors_fp(stderr);
        return false;
    }

    if (!SSL_CTX_check_private_key(ctx))
    {
        Core::Logger::logItLn("[SSL SERVER] Private key does not match public key");
        return false;
    }

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::connect()
{
    Logger::logItLn("[SSL_Client] connecting");
    try
    {
        /* ----------------------------------------------- */
        /* Create a socket and connect to server using normal socket calls. */

        sock = socket (AF_INET, SOCK_STREAM, 0);

        int err = ::connect(sock, (struct sockaddr*) &sa, sizeof(sa));

        if( err == SSL_Socket::socketError() )
        {
            Logger::logItLn("[SSL_Client] connect failed");
            return false;
        }
    }
    catch(...)
    {
        Logger::logItLn("[SSL_Client] connect failed - EXCEPTION");
        return false;
    }

    return handshake_connect();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::getCertInfo(X509 *server_cert, string &subject, string &issuer)
{
    /// Get the cipher - opt
    // X509*    server_cert = _null_;

    if( !server_cert )
    {
        Logger::logItLn("[SSL_Client] SSL_get_peer_certificate failed");
        // char error[65535];
        // ERR_error_string_n(ERR_get_error(), error, 65535);
        // printf("Error: %s\n\n", error);
        return false;
    }

    subject = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);

    issuer = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);

    // printf ("\t issuer: %s\n", str);
    // OPENSSL_free (str);

    /// We could do all sorts of certificate verification stuff here before deallocating the certificate.
    X509_free (server_cert);

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::getRemoteCertInfo(string &subject, string &issuer)
{
    return SSL_Client::getCertInfo(SSL_get_peer_certificate (ssl), subject, issuer);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::getLocalCertInfo(string &subject, string &issuer)
{
    return SSL_Client::getCertInfo(SSL_get_certificate (ssl), subject, issuer);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
string SSL_Client::getCipher()
{
    return SSL_get_cipher (ssl);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::handshake_connect()
{
    Logger::logItLn("[SSL_Client] handhaking");
    try
    {
        /* ----------------------------------------------- */
        /* Now we have TCP conncetion. Start SSL negotiation. */

        ssl = SSL_new (ctx);
        if( !ssl )
        {
            Logger::logItLn("[SSL_Client] SSL_new failed");
            // char error[65535];
            // ERR_error_string_n(ERR_get_error(), error, 65535);
            // printf("Error: %s\n\n", error);
            return false;
        }

        SSL_set_cipher_list(ssl,"ALL");
        SSL_set_fd (ssl, sock);
        // SSL_set_connect_state(ssl);

        int err = SSL_connect (ssl);
        if( err == -1 )
        {
            Core::Logger::logVariable("[SSL SERVER] SSL_connect failed. Error", SSL_get_error(ssl, err));
            Logger::logItLn("[SSL_Client] SSL_connect failed");
            // char error[65535];
            // ERR_error_string_n(ERR_get_error(), error, 65535);
            // printf("Error: %s\n\n", error);
            return false;
        }

        // Logger::logVariable("SSL connection using", SSL_get_cipher (ssl));

        // Logger::logVariable("[SSL_Client] ssl", ssl);
/*
        err = SSL_write (ssl, "Hello World!", strlen("Hello World!"));
*/
        is_dead = false;
    }
    catch(...)
    {
        Logger::logItLn("[SSL_Client] handshake failed - EXCEPTION");
        return false;
    }

    Logger::logItLn("[SSL_Client] handshake DONE");
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::handshake_accept()
{
    Logger::logItLn("[SSL_Client] accept handhake");
    try
    {
        int err;
        X509*    client_cert;
        char*    str;
        /* ----------------------------------------------- */
        /* TCP connection is ready. Do server side SSL. */
        ssl = SSL_new (ctx);
        if( !ssl )
        {
            Core::Logger::logItLn("[SSL Client] SSL_new failed");
            ERR_print_errors_fp(stderr);
            return false;
        }
        SSL_set_cipher_list(ssl,"ALL");
        SSL_set_fd (ssl, sock);
        // SSL_set_accept_state(ssl);

        err = SSL_accept (ssl);
        if( err == -1 )
        {
            Core::Logger::logVariable("[SSL Client] SSL_accept failed. Error", SSL_get_error(ssl, err));
            // char error[65535];
            // ERR_error_string_n(ERR_get_error(), error, 65535);
            // printf("Error: %s\n\n", error);
            close();
            return false;
        }
/*
        // DATA EXCHANGE - Receive message and send reply.
        err = SSL_read (ssl, buf, sizeof(buf) - 1);
        if( err == -1 )
        {
            Core::Logger::logItLn("[SSL Client] SSL_read failed");
            return false;
        }

        buf[err] = '\0';
        cout << "Got " << err << "chars:'" << buf << "'" << endl;

        err = SSL_write (ssl, "I hear you.", strlen("I hear you."));
        if( err == -1 )
        {
            Core::Logger::logItLn("[SSL Client] SSL_write failed");
            return false;
        }
*/
        is_dead = false;
    }
    catch(...)
    {
        Core::Logger::logItLn("[SSL Server] Init Connection Failed - Exception");
        return false;
    }

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::close()
{
    Logger::logItLn("[SSL_Client] closing...");
    try
    {
        is_dead = true;
		cout << "F1" << endl;
        if( ssl )
        {
            SSL_shutdown (ssl);  /* send SSL/TLS close_notify */
        }
		cout << "F2" << endl;
        /* Clean up. */
        if( sock != SSL_Socket::invalidSocket() )
        {
            ::close(sock);
        }
		cout << "F3" << endl;
        if(ssl)
            SSL_free (ssl);

    }
    catch(...)
    {
        Logger::logItLn("[SSL_Client] close - EXCEPTION");
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::send(const unsigned char *data, const size_t &data_size)
{
    try
    {
        if( !ssl ) return false;

        // Logger::logVariable("[SSL_Client] ssl", ssl);
        // Logger::logItLn("[SSL_Client] send");

        int err = SSL_write(ssl, (void*)data, data_size);
        if( err == -1 )
        {
            Logger::logVariable("[SSL_Client] send failed err", err);
            // close();
            return false;
        }
        // Logger::logVariable("[SSL_Client] send success", err);
    }
    catch(...)
    {
        Logger::logItLn("[SSL_Client] send failed - EXCEPTION");
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::recv(unsigned char *&data, size_t &data_size)
{
    try
    {
        if( !ssl ) return false;

        if( data )
            delete data;

        // Logger::logVariable("[SSL_Client] recving", ssl);
        unsigned char buf[ Config::MAX_TCP_PACKET_SIZE ];
        int err = -1;

        err = SSL_read (ssl, buf, Config::MAX_TCP_PACKET_SIZE - 1);
        if( err == -1 )
        {
            // close();
            Logger::logVariable("[SSL_Client] recv failed", err);
            return false;
        }
        // Logger::logVariable("[SSL_Client] recv", err);

        data = new unsigned char[err + 1];
        data_size = err;
        memcpy((void*)data, (void*)buf, data_size);
        data[ data_size ] = '\0';
    }
    catch(...)
    {
        Logger::logItLn("[SSL_Client] connect failed - EXCEPTION");
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Client::isActive()
{
    return !is_dead && (sock != SSL_Socket::invalidSocket()) && (sock != SSL_Socket::socketError());
}
}

}
#endif
