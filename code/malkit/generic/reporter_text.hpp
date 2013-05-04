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
 * @brief GraVitoN::Malkit_Reporter_Text
 *
*/

#ifndef _GVN_MALKIT_REPORTER_TEXT_HEAD_
#define _GVN_MALKIT_REPORTER_TEXT_HEAD_

#include "../../gvn_malkit.hpp"
#include "../../../gvn_utils/gvn_tcp_client.hpp"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief The Malkit_Reporter_Text class
 *
 * Send logs and reports to a third party TCP server.
 * 'run' method is useless here.
 * The key method is 'report' which does the job!
 *
 */
class Malkit_Reporter_Text : public GraVitoN::Malkit
{
protected:
	GraVitoN::TCP_Client client;
	bool stay_connectd;

public:
	Malkit_Reporter_Text()
	{
		stay_connectd = false;
	}

	virtual ~Malkit_Reporter_Text()
	{
	}

    /**
     * @options:
	 *	IP, PORT same as TCP_Client \n
	 *  STAY_CONNECTED: false means, you want to open a new socket for each attempt to report. and
	 *  true means you want to open a socket at initilizing process and close it after deleting object of this class.
     */
	virtual bool initialize(const string &_options);

	/// @brief Report a message to report-server
	virtual bool report(const string &message);

    /// @brief Close the open connection
    virtual bool close();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Malkit_Reporter_Text::initialize(const string &_options)
{
	options = _options;

	if( client.isActive() )
		client.close();

	if( !GraVitoN::OptParser::getValueAsBool(options, "STAY_CONNECTED", stay_connectd) )
		stay_connectd = true;
	//GraVitoN::Logger::logVariable("SC", stay_connectd);

	if(!client.initialize(options))
		return false;

	if( stay_connectd ) if (!client.open())
		return false;

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Malkit_Reporter_Text::report(const string &message)
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
bool Malkit_Reporter_Text::close()
{
    return client.close();
}

#endif // _GVN_MALKIT_REPORTER_TEXT_HEAD_
