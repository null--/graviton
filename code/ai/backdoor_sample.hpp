/**
 * @file
 * 
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
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
 * @brief A very simple trojan-AI
 *
 */

#ifndef _GVN_AI_TROJAN_SAMPLE_HEAD_
#define _GVN_AI_TROJAN_SAMPLE_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <ai/ai.hpp>
#include <payload/payload.hpp>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
namespace GraVitoN
{

namespace AI
{

class Backdoor_Sample : public GraVitoN::AI::AI_Component
{
protected:
	/// Our payload
    Payload::Payload_Component &payload;

public:
	/// Constructor
    Backdoor_Sample(Payload::Payload_Component &mypayload);
	
	/// Destructor
    virtual ~Backdoor_Sample();

    /// Think!
    virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Backdoor_Sample::Backdoor_Sample(Payload::Payload_Component &mypayload) : payload(mypayload)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Backdoor_Sample::~Backdoor_Sample()
{
}

bool Backdoor_Sample::run()
{
    return payload.run();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

}
}
#endif // _GVN_AI_TROJAN_SAMPLE_HEAD_
