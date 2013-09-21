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
 * @brief test file and directory classes
 *
*/

#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/memory.hpp>
#include <utils/file.hpp>
#include <utils/directory.hpp>

using namespace GraVitoN::Utils;

int main()
{
    Directory home = Directory::cwd().parent().parent().parent(), mdir("test-dir");
    cout << "Path: " << home.getPath() << endl;
    
    std::vector<File> files;
    
    home.findFiles(files);

    for(std::vector<File>::iterator fi = files.begin(); fi != files.end(); ++fi)
    {
        cout << fi->getPath() << endl;
    }

    mdir.create();
    File myfile(mdir.getPath() + "/test.txt");

    cout << "EX: " << myfile.exists() << endl;
    
    myfile.create();

    cout << "EX: " << myfile.exists() << endl;

    mdir.createSubDirectory("test2");
 
    mdir.remove();

    return 0;
}
