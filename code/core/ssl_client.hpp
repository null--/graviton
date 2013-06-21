/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief GraVitoN::Core::SSL_Client
 */

#ifndef GRAVITON_SSL_CLIENT_H
#define GRAVITON_SSL_CLIENT_H

#include <graviton.hpp>
#include <core/ssl_socket.hpp>
#include <core/tcp_client.hpp>

namespace GraVitoN
{
    namespace Core
    {
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        class SSL_Client : public TCP_Client
        {
        protected:
            SSL_CTX* ctx;
            SSL*     ssl;

        protected:
            bool handshake();

        public:
            SSL_Client(const std::string &ip, const unsigned int port);
            SSL_Client(Socket::Handle _sock, Socket::Address _sa);

            virtual ~SSL_Client();

            /// connect to server
            virtual bool connect();

            /// close socket
            virtual bool close();

            /// recieve data from server
            template<class Type>
            bool recv(Memory<Type> &data);

            /// recv string from server
            string recvString();

            /// send data to server
            template<class Type>
            bool send(const Memory<Type> &data);

            /// send string to server
            bool sendString(const std::string &data);

            bool handshake_connect();
            bool handshake_accept();


            static bool getCertInfo(X509 *cert, std::string &subject, std::string &issuer);
            bool getRemoteCertInfo(std::string &subject, std::string &issuer);
            bool getLocalCertInfo(std::string &subject, std::string &issuer);

            string getCipher();

            bool loadPemFromFile(const std::string &cert_file, const std::string &key_file);

        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        SSL_Client::SSL_Client(const string &ip, const unsigned int port) : TCP_Client(ip, port)
        {
            SSL_Socket::getInstance();
            
            ctx = SSL_CTX_new (SSLv23_client_method());
            sock = Socket::Invalid;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        SSL_Client::SSL_Client(Socket::Handle _sock, Socket::Address _sa) : TCP_Client(_sock, _sa)
        {
            SSL_Socket::getInstance();
            
            ctx = SSL_CTX_new (SSLv23_client_method());
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
            if( TCP_Client::connect() )
            {
                return handshake_connect();
            }

            return false;
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
                if( ssl )
                {
                    SSL_shutdown (ssl);  /* send SSL/TLS close_notify */
                }

                TCP_Client::close();
                
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
        template<class Type>
        bool SSL_Client::send(const Memory<Type> &data)
        {
            try
            {
                if( !ssl ) return false;

                // Logger::logVariable("[SSL_Client] ssl", ssl);
                // Logger::logItLn("[SSL_Client] send");

                int err = SSL_write(ssl, (void*)data.address(), data.size());
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
        bool SSL_Client::sendString(const std::string &data)
        {
            Memory<char> data_(data.c_str(), data.size());
            return SSL_Client::send(data_);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool SSL_Client::recv(Memory<Type> &data)
        {
            try
            {
                if( !ssl ) return false;

                // Logger::logVariable("[SSL_Client] recving", ssl);
                Type buf[ Config::MAX_TCP_PACKET_SIZE ];
                int err = -1;

                err = SSL_read (ssl, (void*)buf, Config::MAX_TCP_PACKET_SIZE - 1);
                if( Socket::socketError(err) )
                {
                    // close();
                    Logger::logVariable("[SSL_Client] recv failed", err);
                    return false;
                }

                data.copy(buf, err);
            }
            catch(...)
            {
                Logger::logItLn("[SSL_Client] connect failed - EXCEPTION");
                return false;
            }
            return true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        std::string SSL_Client::recvString()
        {
            // sdata = "";
            Memory<guchar> data;
            if( SSL_Client::recv(data) )
            {
                return std::string((char*)data.address(), data.size());
            }

            return std::string("");
        }
    }

}
#endif
