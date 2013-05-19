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
 * @brief GraVitoN::Reporter_Text
 *
*/

#ifndef _GVN_Reporter_Text_HEAD_
#define _GVN_Reporter_Text_HEAD_

#include <malkit/malkit.hpp>
#include <core/tcp_client.hpp>

namespace GraVitoN
{

namespace Malkit
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief The Reporter_Text class
 *
 * Send logs and reports to a third party TCP server.
 * 'run' method is useless here, so Reporter_Text is not a real Malkit_Component
 * The key method is 'report' which does the job!
 *
 */
class Reporter_Text // : public Malkit_Component
{
protected:
    GraVitoN::Core::TCP_Client client;
	bool stay_connectd;

public:
    Reporter_Text()
	{
		stay_connectd = false;
	}

    virtual ~Reporter_Text()
	{
	}

    /**
     * @options:
	 *	IP, PORT same as TCP_Client \n
	 *  STAY_CONNECTED: false means, you want to open a new socket for each attempt to report. and
	 *  true means you want to open a socket at initilizing process and close it after deleting object of this class.
     */
    virtual bool initialize(
            const string &ip,
            const unsigned int &port,
            const bool &_stay_connectd = false);

	/// @brief Report a message to report-server
	virtual bool report(const string &message);

    /// @brief Close the open connection
    virtual bool close();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Reporter_Text::initialize(const string &ip, const unsigned int &port, const bool &_stay_connectd)
{
    stay_connectd = _stay_connectd;

    if( client.isActive() )
		client.close();

    stay_connectd = true;

    if(!client.initialize(ip, port) )
		return false;

	if( stay_connectd ) if (!client.open())
		return false;

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Reporter_Text::report(const string &message)
{
	if(!stay_connectd)
		if(!client.open())
			return false;

	client.send((unsigned char*)message.c_str(), message.size());

	if( !stay_connectd )
		if(client.isActive() && !client.close())
			return false;
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Reporter_Text::close()
{
    return client.close();
}

}
}

#endif // _GVN_Reporter_Text_HEAD_
