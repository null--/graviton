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

#ifndef _GVN_AI_TROJAN_SAMPLE_HEAD_
#define _GVN_AI_TROJAN_SAMPLE_HEAD_

#include "gvn_ai.hpp"

class Gvn_AI_Trojan_Sample : public Gvn_AI
{
private:
	Gvn_Payload *payload;
	
public:
    Gvn_AI_Trojan_Sample();
    virtual ~Gvn_AI_Trojan_Sample();
	
	virtual bool initialize (
		const string &_options,
		Gvn_Payload *_payload
  	);
	virtual bool think();
};

Gvn_AI_Trojan_Sample::Gvn_AI_Trojan_Sample()
{
	Gvn_AI();

	payload = _null_;
	
	info.name = "Sample Trojan AI";
    info.version = "0.1b";
    info.os = "General";
    info.architecture = "General";
    info.hardware = "General";
    info.about =
        "This is a sample AI for trojan based attacks.\n"
        "\n";
}

Gvn_AI_Trojan_Sample::~Gvn_AI_Trojan_Sample()
{
}

bool Gvn_AI_Trojan_Sample::initialize ( const string& _options, Gvn_Payload *_payload )
{
	options = _options;
	payload = _payload;
}

bool Gvn_AI_Trojan_Sample::think()
{
	payload->initialize( options );
	payload->call();
	
    return true;
}

#endif // _GVN_AI_TROJAN_SAMPLE_HEAD_