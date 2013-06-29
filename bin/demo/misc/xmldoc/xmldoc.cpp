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
 * @brief test xmldoc
 *
 */

#include <iostream>
#include <utils/xmldoc.hpp>
#include <fstream>
#include <iostream>
using namespace std;

using namespace GraVitoN::Utils;

int main()
{
    XML_Document pars;
    pars.addDecleration();
    XML_Node    files;
    XML_Attrib  files_attrib;

    pars.addNode(files, "Files");
    files.addAttribute(files_attrib, "dummy", "foobar.com");

    XML_Node file, path, name;
    for(int i=0; i<10; ++i)
    {
        files.addChild(file, "File");
        file.addChild(path, "Path", "File_Path");
        file.addChild(name, "Name", "File_Name");

        cout << file.name() << " -> " << file.value() << endl;
        cout << path.name() << " -> " << path.value() << endl;
        cout << name.name() << " -> " << name.value() << endl;
        
    }

    file.removeChild(name);
    pars.save("example2.xml");

    return 0;
    
    // xml_document<> doc;
    // xml_node<>* decl = doc.allocate_node(node_declaration);
    // decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    // decl->append_attribute(doc.allocate_attribute("encoding", "UTF-8"));
    // doc.append_node(decl);  



    // xml_node<> *files = doc.allocate_node(node_element, "Files");
    // doc.append_node(files);
    // xml_attribute<> *attr = doc.allocate_attribute("dummy", "google.com");
    // files->append_attribute(attr);

    // for(int i = 0;i<10;++i)
    // {
    //     xml_node<> *file = doc.allocate_node(node_element, "File");
    //     files->append_node(file);

    //     xml_node<> *path = doc.allocate_node(node_element, "Path","File_path");
    //     file->append_node(path);
    //     xml_node<> *name = doc.allocate_node(node_element, "Name","File_name");
    //     file->append_node(name);
    // }

    // std::ofstream myfile;
    // myfile.open ("example.xml");
    // myfile << doc;
    // //print(std::cout, doc, 0);
    // return 0;
};
