/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/files.hpp>
#include <utils/xmlparser.hpp>

#include "graver.hpp"

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#define vcout if(glob_conf.verbose)cout

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct CONF
{
    bool verbose;
    
    const string library;
    const string compiler;

    const string os;

    string graviton_path;
    
    rapidxml::xml_document<> compiler_xml;
    rapidxml::xml_document<> library_xml;

    CONF();
} glob_conf;

CONF::CONF():library("library.conf"), compiler("compiler.conf")
            ,os(
#if defined(INFO_OS_WINDOWS)
                "windows"
#elif defined(INFO_OS_LINUX)
                "linux"
#elif defined(INFO_OS_OSX)
                "osx"
#else
                "unknown os"
#endif                
                )
{
    verbose = false;
    graviton_path = "../../";
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct PLATFORM
{
    string arch;
    string os;
    string target;
    string compiler;

    string compiler_option;
    vector<string> files;
    
    PLATFORM(){}
};

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct COMPILER
{
    string name;

    string arch;
    string os;
    string target;
    
    string command;
    string linker;

    string flag_incpath;
    string flag_libpath;
    string flag_lib;
    string flag_build_object;
    string flag_define;
    string flag_output;
    string flag_general;

    string mode_general;
    string mode_gui;
    // string mode_sock;

    COMPILER(){}
};
vector<COMPILER> glob_compiler;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct LIBRARY
{
    string name;

    vector<string> include;
    vector<string> depend;
    vector<PLATFORM> platform;

    LIBRARY(){}
};
vector<LIBRARY> glob_library;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct PROJECT
{
    bool unsaved_proj;
    string path;
    string compiler;
    string target_os;
    string arch;
    
    string info_name;
    string info_date;
    string info_version;
    string info_hacker;

    vector<string> incpath;
    vector<string> srcpath;
    vector<string> source;

    int compiler_id;
    
    string build_type;
    // vector<PLATFORM> build_platform;
    vector<string>   build_depend;
    /// This vector, will initialize on verifyProject
    vector<string>   build_depend_file;
    string           build_depend_option;   
    
    rapidxml::xml_document<> project_xml;
    
    PROJECT();
} glob_proj;

PROJECT::PROJECT()
{
    unsaved_proj = false;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void printError(const int id)
{
    cout << "[ERROR] ";
    
    switch(id)
    {
    case ERR_INV_VAL:
        cout << "That fuckin' value is bloody wrong!";
        break;
    case ERR_UNK_CMD:
        cout << "What the fuck?!";
        break;
    case ERR_INV_FILE:
        cout << "Give me a fuckin' valid file name";
        break;
    default:
        cout << "Speak english, man!";
        break;
    }

    cout << "" << endl;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void initLibs()
{
    glob_library.clear();
    
    cout << "Loading Libraries..." << endl;

    GraVitoN::Utils::XML_Parser pars;
    GraVitoN::Utils::XML_Node master, node, sub_node, child;
    GraVitoN::Utils::XML_Attrib attr, sub_attr;
        
    /// Load library.conf
    // rapidxml::file<char> libf(glob_conf.library.c_str());
    // glob_conf.library_xml.parse<0>( libf.data() );
    pars.load(glob_conf.library.c_str());
    
    /// <conf>
    // rapidxml::xml_node<> *node = glob_conf.library_xml.first_node(XML_TAG_CONF);
    master = pars.firstNode(XML_TAG_CONF);

    if( !master.isValid() )
    {
        cout << "Failed to load " << glob_conf.library <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// <library name>
    // node = node->first_node(XML_TAG_LIBRARY);
    node = master.firstChild(XML_TAG_LIBRARY);
    while (node.isValid())
    {
        LIBRARY mlib;
        // rapidxml::xml_attribute<> *sub_attr;
        // rapidxml::xml_node<> *sub_node, *child;

        /// name
        // sub_attr = node->first_attribute(XML_ATTR_NAME);
        sub_attr = node.firstAttribute(XML_ATTR_NAME);
        if(!sub_attr)
        {
            cout << "BAD LIBRARY DEFINITION (no 'name' attribute)" << endl;
            goto NEXT;
        }
        
        vcout << "LIBRARY\t\t" <<  sub_attr.name() << ": " << sub_attr.value() << endl;
        // mlib.name = sub_attr->value();
        mlib.name = sub_attr.value(); // sub_attr->value();
        
        /// <include>  
        // sub_node = node->first_node(XML_TAG_INC);
        sub_node = node.firstChild(XML_TAG_INC);
        while( sub_node.isValid() )
        {
            vcout << "\t" << sub_node.name() << ": " << sub_node.value() << endl;
            mlib.include.push_back( sub_node.value() );
            // sub_node = sub_node->next_sibling(XML_TAG_INC);
            sub_node = sub_node.next(XML_TAG_INC);
        }

        /// <platform arch os compiler>
        // sub_node = node->first_node(XML_TAG_PLAT);
        sub_node = node.firstChild(XML_TAG_PLAT);
        while( sub_node )
        {
            PLATFORM plat;
            
            vcout << "\t" << sub_node.name() << ": " << endl;

            /// os
            // sub_attr = sub_node->first_attribute(XML_ATTR_OS);
            sub_attr = sub_node.firstAttribute(XML_ATTR_OS);
            if(!sub_attr)
            {
                cout << "BAD PLATFORM: no 'os' attr" << endl;
                goto NEXT;
            }
            plat.os = sub_attr.value(); // sub_attr->value();
            vcout << "\t\t" << XML_ATTR_OS << ": \t" << plat.os << endl;

            /// arch
            // sub_attr = sub_node->first_attribute(XML_ATTR_ARCH);
            sub_attr = sub_node.firstAttribute(XML_ATTR_ARCH);
            if(!sub_attr.isValid())
            {
                cout << "BAD PLATFORM: no 'arch' attr" << endl;
                goto NEXT;
            }
            // plat.arch = sub_attr->value();
            plat.arch = sub_attr.value();
            vcout << "\t\t" << XML_ATTR_ARCH << ": \t" << plat.arch << endl;

            /// compiler
            // sub_attr = sub_node->first_attribute(XML_ATTR_COMPILER);
            sub_attr = sub_node.firstAttribute(XML_ATTR_COMPILER);
            if(!sub_attr.isValid())
            {
                cout << "BAD PLATFORM: no 'compiler' attr" << endl;
                goto NEXT;
            }
            // plat.compiler = sub_attr->value();
            plat.compiler = sub_attr.value();
            vcout << "\t\t" << XML_ATTR_COMPILER << ": \t" << plat.os << endl;

            /// <file>
            // plat.files.clear();
            // child = sub_node->first_node(XML_TAG_FILE);
            child = sub_node.firstChild(XML_TAG_FILE);
            while(child.isValid())
            {
                string tmp = child.value();// child->value();
                tmp = glob_conf.graviton_path + GRAV_LIB_PATH + tmp;
                
                if( !GraVitoN::Utils::File::pathExists(tmp) )
                {
                    cout << "SHIT HAPPENED: " << tmp << " not found." << endl;
                    exit(0);
                }
                
                plat.files.push_back( child.value() );//child->value());
                // child = child->next_sibling(XML_TAG_FILE);
                child = child.next(XML_TAG_FILE);
            }

            /// <compiler>
            child = sub_node.firstChild(XML_TAG_COMPILER_OPT);
            if(child.isValid()) plat.compiler_option = child.value();

            mlib.platform.push_back(plat);
            sub_node = sub_node.next(XML_TAG_PLAT); //->next_sibling(XML_TAG_PLAT);
        }

        /// <depend>
        sub_node = node.firstChild(XML_TAG_DEP);
        while( sub_node.isValid() )
        {
            vcout << "\t" << XML_TAG_DEP << ": " << sub_node.value() << endl;
            mlib.depend.push_back(sub_node.value());

            // sub_node = sub_node->next_sibling(XML_TAG_DEP);
            sub_node = sub_node.next(XML_TAG_DEP);
        }

        glob_library.push_back(mlib);
NEXT:        
        // node = node->next_sibling(XML_TAG_LIBRARY);
        node = node.next(XML_TAG_LIBRARY);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void initCompilers()
{
    glob_compiler.clear();

    cout << "Loading Compilers..." << endl;

    GraVitoN::Utils::XML_Parser pars;
    GraVitoN::Utils::XML_Node master, node, sub_node, child;
    GraVitoN::Utils::XML_Attrib attr, sub_attr;

    pars.load(glob_conf.compiler.c_str());
    
    // rapidxml::file<char> compf(glob_conf.compiler.c_str());
    // glob_conf.compiler_xml.parse<0>( compf.data() );

    // rapidxml::xml_node<> *node = glob_conf.compiler_xml.first_node(XML_TAG_CONF);
    node = pars.firstNode(XML_TAG_CONF);

    if( !node.isValid() )
    {
        cout << "Failed to load " << glob_conf.compiler <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// Parse compiler
    node = node.firstChild(XML_TAG_COMPILER);
    while (node.isValid())
    {
        COMPILER mcom;
        
        vcout << "COMPILER\t";
        sub_attr = node.firstAttribute(XML_ATTR_NAME);
        if(!sub_attr.isValid())
        {
            cout << "BAD COMPILER: no 'name' attr" << endl;
            goto NEXT;
        }
        mcom.name = sub_attr.value();
        vcout << "\t" << XML_ATTR_NAME << ":\t" << sub_attr.value() << endl;

        sub_attr = node.firstAttribute(XML_ATTR_ARCH);
        if(!sub_attr.isValid())
        {
            cout << "BAD COMPILER: no 'arch' attr" << endl;
            goto NEXT;
        }
        mcom.arch = sub_attr.value();
        vcout << "\t" << XML_ATTR_ARCH << ":\t" << sub_attr.value() << endl;

        sub_attr = node.firstAttribute(XML_ATTR_OS);
        if(!sub_attr.isValid())
        {
            cout << "BAD COMPILER: no 'os' attr" << endl;
            goto NEXT;
        }
        mcom.os = sub_attr.value();
        vcout << "\t" << XML_ATTR_OS << ":\t" << sub_attr.value() << endl;

        sub_attr = node.firstAttribute(XML_ATTR_TARGET);
        if(!sub_attr.isValid())
        {
            cout << "BAD COMPILER: no 'target' attr" << sub_attr.value() << endl;
            goto NEXT;
        }
        mcom.target = sub_attr.value();
        vcout << "\t" << XML_ATTR_TARGET << ":\t" << sub_attr.value() <<endl;
        
        /// Parse command tag
        sub_node = node.firstChild(XML_TAG_COMMAND);
        if( !sub_node.isValid() )
        {
            cout << "BAD COMPILER: no 'command' tag" << endl;
            goto NEXT;
        }
        vcout << "\t" << XML_TAG_COMMAND << ": " << sub_node.value() << endl;
        mcom.command = sub_node.value();

        /// Parse command tag
        sub_node = node.firstChild(XML_TAG_LINKER);
        if( !sub_node.isValid() )
        {
            cout << "BAD COMPILER: no 'linker' tag" << endl;
            goto NEXT;
        }
        vcout << "\t" << XML_TAG_LINKER << ": " << sub_node.value() << endl;
        mcom.linker = sub_node.value();
        
        /// Parse flag
        sub_node = node.firstChild(XML_TAG_FLAG);
        if( !sub_node.isValid() )
        {
            cout << "BAD COMPILER: no 'flag' tag" << endl;
            goto NEXT;
        }
        vcout << "\t" << XML_TAG_FLAG << ":" << endl;

        child = sub_node.firstChild(XML_TAG_INCPATH);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'incpath' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_incpath = child.value();
        vcout << "\t\t" << child.name() << ": " << child.value() << endl;

        child = sub_node.firstChild(XML_TAG_LIBPATH);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'libpath' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_libpath = child.value();
        vcout << "\t\t" << child.name() << ": "<< child.value() << endl;
        
        child = sub_node.firstChild(XML_TAG_DEFINE);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'define' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_define = child.value();
        vcout << "\t\t" << child.name() << ": "<< child.value() << endl;

        child = sub_node.firstChild(XML_TAG_LIB);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'lib' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_lib = child.value();
        vcout << "\t\t" << child.name() << ": "<< child.value() << endl;

        child = sub_node.firstChild(XML_TAG_BUILDOBJ);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'build_object' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_build_object  = child.value();
        vcout << "\t\t" << child.name() << ": "<< child.value() << endl;

        child = sub_node.firstChild(XML_TAG_OUTPUT);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'output' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_output = child.value();
        vcout << "\t\t" << child.name() << ": "<< child.value() << endl;

        child = sub_node.firstChild(XML_TAG_GENERAL);
        if(!child.isValid())
        {
            cout << "BAD COMPILER: no flag > 'general' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_general = child.value();
        vcout << "\t\t" << child.name() << ": "<< child.value() << endl;

        /// Parse modes
        sub_node = node.firstChild(XML_TAG_MODE);
        if( sub_node.isValid() )
        {
            vcout << "\t" << XML_TAG_MODE << ":" << endl;

            child = sub_node.firstChild(XML_TAG_GENERAL);
            if(child.isValid())
            {
                mcom.mode_general = child.value();
                vcout << "\t\t" << child.name() << ": "<< child.value() << endl;
            }
            child = sub_node.firstChild(XML_TAG_GUI);
            if(child.isValid())
            {
                mcom.mode_gui = child.value();
                vcout << "\t\t" << child.name() << ": "<< child.value() << endl;
            }

            // child = sub_node->first_node(XML_TAG_SOCK);
            // if(child)
            // {
            //    mcom.mode_sock = child->value();
            //     vcout << "\t\t" << child->name() << ": "<< child->value() << endl;
            // }
        }

        glob_compiler.push_back(mcom);
NEXT:
        node = node.next(XML_TAG_COMPILER);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
bool loadProject()
{
    cout << "Loading Project..." << endl;

    GraVitoN::Utils::XML_Parser pars;
    GraVitoN::Utils::XML_Node master, node, sub_node, child;
    GraVitoN::Utils::XML_Attrib attr, sub_attr;

    pars.load(glob_proj.path.c_str());
    // rapidxml::file<char> projf(glob_proj.path.c_str());
    // glob_proj.project_xml.parse<0>( projf.data() );

    // rapidxml::xml_node<> *node = glob_proj.project_xml.first_node(XML_TAG_PROJECT);
    node = pars.firstNode(XML_TAG_PROJECT);
    
    if( !node.isValid() )
    {
        cout << "Failed to load " << glob_proj.path <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// Parse Project       
    // rapidxml::xml_attribute<> *sub_attr;
    // rapidxml::xml_node<> *sub_node, *child;

    vcout << "PROJECT:" << endl;
    
    /// Parse info tag
    sub_node = node.firstChild(XML_TAG_INFO);
    if( sub_node.isValid() )
    {
        vcout << "\t" << XML_TAG_INFO << ":" << endl;

        child = sub_node.firstChild(XML_TAG_NAME);
        if(!child.isValid())
        {
            glob_proj.info_name = child.value();
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
        }
        child = sub_node.firstChild(XML_TAG_VERSION);
        if(child.isValid())
        {
            glob_proj.info_version = child.value();
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
        }
        child = sub_node.firstChild(XML_TAG_HACKER);
        if(child.isValid()) 
        {
            glob_proj.info_hacker = child.value();
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
        }
        child = sub_node.firstChild(XML_TAG_DATE);
        if(child.isValid()) 
        {
            glob_proj.info_date = child.value();
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
        }
    }

    /// Parse code tag
    sub_node = node.firstChild(XML_TAG_CODE);
    if( sub_node.isValid() )
    {
        vcout << "\t" << XML_TAG_MODE << ":" << endl;

        child = sub_node.firstChild(XML_TAG_INCPATH);
        while(child.isValid())
        {
            glob_proj.incpath.push_back(child.value());
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
            child = child.next(XML_TAG_INCPATH);
        }

        child = sub_node.firstChild(XML_TAG_SRCPATH);
        while(child.isValid())
        {
            glob_proj.srcpath.push_back(child.value());
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
            child = child.next(XML_TAG_SRCPATH);
        }
        
        child = sub_node.firstChild(XML_TAG_SOURCE);
        while(child.isValid())
        {
            glob_proj.source.push_back(child.value());
            vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
            child = child.next(XML_TAG_SOURCE);
        }
    }

    /// Parse build tag
    sub_node = node.firstChild(XML_TAG_BUILD);
    vcout << "\t" << XML_TAG_BUILD << ":" << endl;
    child = sub_node.firstChild(XML_TAG_TYPE);
    if(child.isValid())
    {
        glob_proj.build_type = child.value();
        vcout << "\t\t" << child.name() << ": \t" << child.value() << endl;
    }
        
    /// Parse platform tags
    /*
    child = sub_node->first_node(XML_TAG_PLAT);
    while( child )
    {
        PLATFORM plat;
            
        vcout << "\t" << child->name() << ": ";

        sub_attr = child->first_attribute(XML_ATTR_OS);
        if(!sub_attr)
        {
            cout << "BAD PLATFORM: no 'os' attr" << endl;
        }
        plat.os = sub_attr->value();
        vcout << "\t\t" << XML_ATTR_OS << ": \t" << plat.os << endl;
            
        sub_attr = child->first_attribute(XML_ATTR_ARCH);
        if(!sub_attr)
        {
            cout << "BAD PLATFORM: no 'arch' attr" << endl;
        }
        plat.arch = sub_attr->value();
        vcout << "\t\t" << XML_ATTR_ARCH << ": \t" << plat.arch << endl;

        sub_attr = child->first_attribute(XML_ATTR_COMPILER);
        if(!sub_attr)
        {
            cout << "BAD PLATFORM: no 'compiler' attr" << endl;
        }
        plat.compiler = sub_attr->value();
        vcout << "\t\t" << XML_ATTR_COMPILER << ": \t" << plat.os << endl;

        glob_proj.build_platform.push_back(plat);

        child = child->next_sibling(XML_TAG_PLAT);
    }
    */
        
    child = sub_node.firstChild(XML_TAG_DEP);
    while(child.isValid())
    {
        vcout << "\t\t" << XML_TAG_DEP << ": \t" << child.value() << endl;
        glob_proj.build_depend.push_back(child.value());
        child = child.next(XML_TAG_DEP);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
bool verifyProject()
{
    int i;
    bool res = false;
    
    /// 1. check compiler/project
    for(i=0; i<glob_compiler.size(); ++i)
    {
        vcout << glob_compiler[i].name      << " | " << glob_proj.compiler  << endl
              << glob_compiler[i].os        << " | " << glob_conf.os        << endl
              << glob_compiler[i].target    << " | " << glob_proj.target_os << endl
              << glob_compiler[i].arch      << " | " << glob_proj.arch      << endl
              << endl;
        if( glob_compiler[i].name      == glob_proj.compiler  &&
            glob_compiler[i].os        == glob_conf.os        &&
            glob_compiler[i].target    == glob_proj.target_os &&
            glob_compiler[i].arch      == glob_proj.arch
            )
        {
            glob_proj.compiler_id = i;
            res = true;
        }
    }

    if( res )
    {
        res = false;
    }
    else
    {
        cout << "NO COMPATIBLE COMPILER FOUND" << endl;
        return false;
    }
    
    /// 2. check library/project

    glob_proj.build_depend_option = "";
    glob_proj.build_depend.clear();
    glob_proj.build_depend_file.clear();
    for(i=0; i<glob_proj.build_depend.size(); ++i)
    {
        bool found = false;

        for(int j=0; j<glob_library.size() && !found; ++j)
        {
            if( glob_library[j].name == glob_proj.build_depend[i] )
            {
                for(int k=0; k < glob_library[i].platform.size() && !found; ++k)
                {
                    if( glob_library[j].platform[k].arch       == glob_proj.arch &&
                        glob_library[j].platform[k].os         == glob_proj.target_os &&
                        glob_library[j].platform[k].compiler   == glob_proj.compiler )
                    {
                        found = true;
                        // vcout << glob_library[i].name << "|" << glob_proj.build_depend[j] << endl;
                        for(int m=0; m<glob_library[j].platform[k].files.size(); ++m)
                            glob_proj.build_depend_file.push_back( glob_library[j].platform[k].files[m] );
                        glob_proj.build_depend_option += "";
                        glob_proj.build_depend_option += glob_library[j].platform[k].compiler_option;
                        glob_proj.build_depend_option += "";
                    }
                }
            }

        }

        if( !found && GraVitoN::Utils::File::pathExists( glob_proj.build_depend[i] ) )
        {
            found = true;
            glob_proj.build_depend_file.push_back( glob_proj.build_depend[i] );
        }

        if( !found )
        {
            cout << "UNKNOWN/INCOMPATIBLE DEPENDENY: " << glob_proj.build_depend[i] << endl;
            return false;
        }
    }
    
    return true;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void buildProject()
{
    COMPILER mc = glob_compiler[glob_proj.compiler_id];
    string command =
        mc.command + " " +
        mc.flag_general + " " +
        mc.flag_build_object + " " +
        mc.flag_incpath + glob_conf.graviton_path + GRAV_CODE_PATH + " ";

    int i;

    /// Adding Project Includes
    vcout << "Adding includes: " << glob_proj.incpath.size() << endl;
    for(i=0; i<glob_proj.incpath.size(); ++i)
        if( !glob_proj.incpath[i].empty() )
            command += mc.flag_incpath + " " + glob_proj.incpath[i] + " ";

    /// Adding Library Includes
    vcout << "Adding libraries: " << glob_proj.build_depend_file.size() << endl;
    for(i=0; i<glob_proj.build_depend_file.size(); ++i)
        if( !glob_proj.build_depend_file[i].empty() )
            command += mc.flag_incpath + " " + glob_proj.build_depend_file[i] + " ";

    /// Get sources
    vcout << "Adding sources: " << glob_proj.srcpath.size() << endl;
    vector<string> sources;
    for(int i=0; i<glob_proj.srcpath.size(); ++i)
    {
        GraVitoN::Utils::File::findAllFiles(glob_proj.srcpath[i], sources, ".*\\.[c][p][p]$", true);
    }

    for(int i=0; i<sources.size(); ++i)
        vcout << "SOURCE: " << sources[i] << endl;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void init()
{
    initCompilers();
    initLibs();
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doExit()
{
    if( glob_proj.unsaved_proj )
    {
        string dec;
        cout << "There is an unsaved project, Are you drunk? (yes/no)" << endl;
        cin >> dec;

        if( dec != "yes" )
        {
            cout << "OK! You're not drunk!" << endl;
            return;
        }
    }

    cout << endl ;
    cout << "[ I used to roll the dice, feel the fear in my enemy's eyes... ]" << endl;

    exit(0);
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doHelp()
{
    cout << CMD[ CMD_HELP ]     << "\t\t\t: Print help banner"                  << endl;
    cout << CMD[ CMD_EXIT ]     << "\t\t\t: Exit"                               << endl;
    cout << CMD[ CMD_NEW  ]     << "\t\t\t: Create new project"                 << endl;
    cout << CMD[ CMD_LOAD ]     << "\t\t\t: Load an exsisting project"          << endl;
    cout << CMD[ CMD_SAVE ]     << "\t\t\t: Save project"                       << endl;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doLoad()
{
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doSave()
{
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doNew()
{
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doBuild()
{
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void doBuildDep()
{
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void interact(const string &input)
{
    bool valid = false;
    int cmd;
    for(cmd=0; cmd<N_CMD; ++cmd)
    {
        if( input == CMD[cmd] )
        {
            valid = true;
            break;
        }
    }
    
    if(!valid)
    {
        printError(ERR_UNK_CMD);
        return;
    }

    switch (cmd)
    {
    case CMD_HELP:
        doHelp();
        break;
    case CMD_NEW:
        doNew();
        break;
    case CMD_SAVE:
        doSave();
        break;
    case CMD_BUILD:
        doBuild();
        break;
    case CMD_LOAD:
        doLoad();
        break;
    case CMD_BUILD_DEP:
        doBuildDep();
        break;
    default: /// It's definitely CMD_EXIT
        doExit();
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/// TODO: Interactive Mode
void interactive()
{
    string input;

    cout << "[ There are rules... to be broken. ]" << endl;
    
    while( (cout << SIGN, getline(cin, input)) )
    {
        interact(input);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void normal(int argc, char **argv)
{
    if( argc != 9 && argc != 10 )
    {
        cout << "Usage: " << argv[0] << "-v -c <compiler> -o <target os> -a <architecture> -p <project file>" << endl << endl;
        cout << "\t-v:        verbose" << endl;
        cout << "\n\tFor more information about -c, -o and -a valid options you can take a look at \n\tcompiler.conf file, inside graver folder" << endl;
        exit(0);
    }

    for(int i=1; i<argc-1; ++i)
    {
        string opt = argv[i];

        if( opt == "-c" || opt == "-C" )
        {
            glob_proj.compiler = argv[++i];
        }
        else if ( opt == "-o" || opt == "-O" )
        {
            glob_proj.target_os = argv[++i];
        }
        else if ( opt == "-a" || opt == "-A" )
        {
            glob_proj.arch = argv[++i];
        }
        else if ( opt == "-p" || opt == "-P" )
        {
            glob_proj.path = argv[i+1];
        }
        else if ( opt == "-v" )
        {
            glob_conf.verbose = true;
            vcout << "[VERBOSE MODE]" << endl;
        }
        else
        {
            printError(ERR_UNK_CMD);
            exit(0);
        }
    }

    loadProject();

    if( !verifyProject() )
    {
        cout << "Graver is unable to compile your project on this system. (Quick Note: change your -c -o -a options and try again)" << endl;
        exit(0);
    }
    else
    {
        cout << "It seems that your project is compilable but beware that SHIT ALWAYS HAPPEN!" << endl;
        buildProject();
    }
    
    cout << "[ There are rules... to be broken. ]" << endl;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
int main(int argc, char **argv)
{
    // GraVitoN::Utils::Regex rex;
    // rex.compile(".*\\.[c][p][p]$");

    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    // cout << rex.match("/usr/include/webkitgtk-1.0/webkit/WebKitDOMEventTarget.h") << endl;
    
    // return 0;
    
    cout << BANNER << endl;
    cout << VERSION << " for " << glob_conf.os << endl << endl;

    for(int i=0; i<argc; ++i)
        if( string(argv[i]) == "-v" )
            glob_conf.verbose = true;
    
    init();
    
    normal(argc, argv);
    
    return 0;
}
