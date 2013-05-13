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
 * @brief GraVitoN::OptParser
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_OPTPARSER_HEAD_
#define _GVN_OPTPARSER_HEAD_

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ios>
using namespace std;

namespace GraVitoN
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief GraVitoN component option parser
 *
 * @note
 *
 * Valid Options: \n
 * 		Sample1: PORT=7357 \n
 * 		Sample2: PoRt=7357 HOST=127.0.0.1 \n
 * 		Sample3: MSG='Hell\'o GraVitoN!'" \n
 * 			Note: Inside C++ Code Write It As: "MSG='Hell\\'o GraVitoN!'" \n
 * Invalid Options: \n
 * 		Invalid1: PORT = 7357 \n
 *
 */

namespace Utils
{

namespace OptParser
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// String to lower
string stringLower ( const string &m_str )
{
    string str = m_str;
    for ( string::iterator i = str.begin(); i != str.end(); ++i )
    {
        *i = tolower ( *i );
    }
    return str;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get value of an entity as a string
 *
 * @param [in] m_option
 * Option string
 *
 * @param [in] m_entity
 * Entity name
 *
 * @param [in] to_lower
 * Return value as a lower string
 *
 * @param [out] value
 * Entity value
 *
 * @return
 * True if query is valid
 *
 */
bool getValueAsString ( const string &m_option, const string &m_entity, string &value, bool to_lower = false )
{
    string option = stringLower ( m_option ),
           entity = stringLower ( m_entity );

    /// Search for entity
	int pos = option.find ( entity+"=" );
    value = "";

    if ( pos == string::npos )
    {
        return false;
    }

    while ( option[pos] != '\0' && option[pos] != '=' )
	{
        ++pos;
    }

    if ( /* option[pos] == '\0' ||*/ option[pos+1] == '\0' )
    {
        return false;
    }

    /// If value is a qouted string
    if ( option[pos+1] == '\'' )
    {
        ++pos;
        while ( option[pos] != '\0' )
        {
            ++pos;
            //cout << option[pos] << endl;
            if ( option[pos] == '\0' )
            {
                //cout << "INVALID"<<endl;
                value = "";
                return false;
            }

            /*
			if ( option[pos] == '\\' )
            {
                if ( option[pos+1] == '\0' )
                {
                    value = "";
                    return false;
                }
                else if ( option[pos+1] == '\'' )
                {
                    ++pos;
                }
            }
            else
			*/
			if ( option[pos] == '\'' )
            {
                //cout << value << ", " << pos << "Break"<<endl;
                break;
            }

            value += m_option[pos];
        }
    }
    /// If value is a single string
    else
    {
        while ( option[pos] != '\0' )
        {
            ++pos;

            if ( option[pos] == ' ' || option[pos] == '\0' )
            {
                break;
            }

            value += m_option[pos];
        }
    }

    if ( to_lower )
    {
        value = stringLower ( value );
    }

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get entity value as bool
 *
 * @see getValueAsString
 */
bool getValueAsBool ( const string &option, const string &entity, bool &value )
{
    string tmp_value;

    if ( !getValueAsString ( option, entity, tmp_value, true ) )
    {
        return false;
    }

    //cout << tmp_value << "|" << endl;
    value = ( tmp_value == "true" );

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get entity value as int
 *
 * @see getValueAsString
 */
bool getValueAsInt ( const string &option, const string &entity, int &value )
{
    string tmp_value;

    if ( !getValueAsString ( option, entity, tmp_value ) )
    {
        return false;
    }

    value = atoi ( tmp_value.c_str() );
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get entity value as int
 *
 * @see getValueAsString
 */
bool getValueAsUInt ( const string &option, const string &entity, unsigned int &value )
{
	string tmp_value;

	if ( !getValueAsString ( option, entity, tmp_value ) )
	{
		return false;
	}

	value = atoi ( tmp_value.c_str() );
	return true;
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get entity value as float
 *
 * @see getValueAsFloat
 */
bool getValueAsFloat ( const string &option, const string &entity, float &value )
{
    string tmp_value;

    if ( !getValueAsString ( option, entity, tmp_value ) )
    {
        return false;
    }

    value = atof ( tmp_value.c_str() );
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get entity value as 64bit int
 *
 * @see getValueAsString
 */
bool getValueAsInt64 ( const string &option, const string &entity, long &value )
{
    string tmp_value;

    if ( !getValueAsString ( option, entity, tmp_value ) )
    {
        return false;
    }

    value = atol ( tmp_value.c_str() );
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Get entity value as a hexadecimal string
 *
 * PORT=7357 ----> HexString="1CBD"
 *
 * @see getValueAsString
 */
bool getValueAsHexString ( const string &option, const string &entity, string &hex_value )
{
    int tmp_value;

    if ( !getValueAsInt ( option, entity, tmp_value ) )
    {
        return false;
    }

    hex_value = "";
    char buf[256];
    snprintf (buf, 255, "%x", tmp_value );
    hex_value = buf;
    //Logger::logItLn(hex_value);
    return true;
}

}
}
}
#endif // _GVN_OPTPARSER_HEAD_
