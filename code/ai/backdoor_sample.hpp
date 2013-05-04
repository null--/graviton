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
#include "gvn_ai.hpp"
#include "../gvn_payload/gvn_payload.hpp"
using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief A Sample AI for a trojan-based attacks
 *
 * This AI was designed to do two simple things:\n
 * 0. Derive a class from this component and onerload initializeInternalComponents
 * in order to initialize your components
 * 1. Initializes a payload \n
 * 2. Call that payload \n
 *
 */
class AI_Trojan_Sample : public GraVitoN::AI
{
protected:
	/// Our payload
	Payload *payload;

protected:
	/**
	  * @brief Initialize internal components
	  */
	virtual bool initializeInternalComponents();

public:
	/// Constructor
    AI_Trojan_Sample();
	
	/// Destructor
    virtual ~AI_Trojan_Sample();

	/**
	 * @brief run
	 *
	 * Think, think, think!
	 */
	virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
AI_Trojan_Sample::AI_Trojan_Sample()
{
	payload = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
AI_Trojan_Sample::~AI_Trojan_Sample()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool AI_Trojan_Sample::initializeInternalComponents()
{
	Logger::logItLn("[AI_Trojan_Sample::initializeInternalComponents] YOU HAVE TO OVERRIDE ME!");
	return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool AI_Trojan_Sample::run()
{
	GraVitoN::Logger::logItLn ( "Thinking..." );
	
	(payload)->run();
	
    return true;
}

#endif // _GVN_AI_TROJAN_SAMPLE_HEAD_
