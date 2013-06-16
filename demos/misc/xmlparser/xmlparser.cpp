#include <iostream>
#include <utils/xmlparser.hpp>
#include <fstream>
#include <iostream>
using namespace std;

using namespace GraVitoN::Utils;

int main()
{
    XML_Parser pars;
    pars.addDecleration();
    XML_Node    files;
    XML_Attrib  files_attrib;
    
    pars.addNode(files, "Files");
    pars.addAttribute(files, files_attrib, "dummy", "foobar.com");

    XML_Node file, path, name;
    for(int i=0; i<10; ++i)
    {
        pars.addChild(files, file, "File");
        pars.addChild(file, path, "Path", "File_Path");
        pars.addChild(file, name, "Name", "File_Name");
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
