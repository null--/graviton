/*
+-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
| This file is part of Graviton.                                       |
|                                                                      |
| Graviton is free software: you can redistribute it and/or modify     |
| it under the terms of the GNU General Public License as published by |
| the Free Software Foundation, either version 3 of the License, or    |
| (at your option) any later version.                                  |
|                                                                      |
| Graviton is distributed in the hope that it will be useful,          |
| but WITHOUT ANY WARRANTY; without even the implied warranty of       |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
| GNU General Public License for more details.                         |
|                                                                      |
| You should have received a copy of the GNU General Public License    |
| along with Graviton.  If not, see <http://www.gnu.org/licenses/>.    |
+-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
*/

/*
 * Programmers:
 * 		1- Sina Hatef Matbue | _null_ | sinahatef.cpp@gmail.com
 */

/*
 * Valid Options:
 * 		Sample1: PORT=7357
 * 		Sample2: PORT=7357 HOST=127.0.0.1
 * 		Sample3: MSG='Hell\'o GraVitoN!'"
 * 			Note: Inside C++ Code Write It As: "MSG='Hell\\'o GraVitoN!'"
 * Invalid Options:
 * 		Invalid1: PORT = 7357
 *
 */

#ifndef _GVN_OPTPARSER_HEAD_
#define _GVN_OPTPARSER_HEAD_

#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ios>
using namespace std;

namespace GvnOptParser
{
	bool getValueAsString(const string &option, const string &entity, string &value)
	{
		int pos = option.find(entity);
		value = "";
		
		if (pos == string::npos)
			return false;
		
		while(option[pos] != '\0' && option[pos] != '=')
			++pos;

		if (option[pos] == '\0' || option[pos+1] == '\0')
			return false;
		
		
		if( option[pos+1] == '\'')
		{
			++pos;
			while( option[pos] != '\0' )
			{
				++pos;
				//cout << option[pos] << endl;
				if (option[pos] == '\0' )
				{
					//cout << "INVALID"<<endl;
					value = "";
					return false;
				}
				
				if( option[pos] == '\\' )
				{
					if( option[pos+1] == '\0' )
					{
						value = "";
						return false;
					}
					else if ( option[pos+1] == '\'' )
					{
						++pos;
					}
				}
				else if (option[pos] == '\'')
				{
					//cout << "Break"<<endl;
					break;
				}

				value += option[pos];
			}
		}
		else
		{
			while( option[pos] != '\0' )
			{
				++pos;

				if( option[pos] == ' ' || option[pos] == '\0')
					break;
				
				value += option[pos];
			}
		}
		
		return true;
	}

	bool getValueAsInt(const string &option, const string &entity, int &value)
	{
		string tmp_value;
		
		if ( !getValueAsString(option, entity, tmp_value) )
			return false;

		value = atoi( tmp_value.c_str() );
		return true;
	}

	bool getValueAsFloat(const string &option, const string &entity, float &value)
	{
		string tmp_value;

		if ( !getValueAsString(option, entity, tmp_value) )
			return false;

		value = atof( tmp_value.c_str() );
		return true;
	}

	bool getValueAsInt64(const string &option, const string &entity, float &value)
	{
		string tmp_value;

		if ( !getValueAsString(option, entity, tmp_value) )
			return false;

		value = atoll( tmp_value.c_str() );
		return true;
	}
	
	bool getValueAsHexString(const string &option, const string &entity, string &hex_value)
	{
		int tmp_value;
		
		if ( !getValueAsInt(option, entity, tmp_value) )
			return false;

		hex_value = "";
		char buf[256];
		sprintf(buf, "%x", tmp_value);
		hex_value = buf;
		//gvnLogItLn(hex_value);
		return true;
	}
}

#endif // _GVN_OPTPARSER_HEAD_