/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#include <graviton.hpp>

#include "graver.hpp"

#include <external/rapidxml/rapidxml.hpp>
#include <external/rapidxml/rapidxml_utils.hpp>
// #include <external/rapidxml/rapidxml_print.hpp>

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
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct PLATFORM
{
    string arch;
    string os;
    string target;
    string compiler;
    
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
    string mode_sock;

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

    string build_type;
    // vector<PLATFORM> build_platform;
    vector<string>   build_depend;
    vector<int>      build_depend_idx;
    
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

    /// Load library.conf
    rapidxml::file<char> libf(glob_conf.library.c_str());
    glob_conf.library_xml.parse<0>( libf.data() );

    /// <conf>
    rapidxml::xml_node<> *node = glob_conf.library_xml.first_node(XML_TAG_CONF);

    if( !node )
    {
        cout << "Failed to load " << glob_conf.library <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// <library name>
    node = node->first_node(XML_TAG_LIBRARY);
    while (node)
    {
        LIBRARY mlib;
        rapidxml::xml_attribute<> *sub_attr;
        rapidxml::xml_node<> *sub_node;

        /// name
        sub_attr = node->first_attribute(XML_ATTR_NAME);
        if(!sub_attr)
        {
            cout << "BAD LIBRARY DEFINITION (no 'name' attribute)" << endl;
            goto NEXT;
        }
        
        vcout << "LIBRARY\t\t" <<  sub_attr->name() << ": " << sub_attr->value() << endl;
        mlib.name = sub_attr->value();       
            
        /// <include>  
        sub_node = node->first_node(XML_TAG_INC);
        while( sub_node )
        {
            vcout << "\t" << sub_node->name() << ": " << sub_node->value() << endl;
            mlib.include.push_back(sub_node->value());
            sub_node = sub_node->next_sibling(XML_TAG_INC);
        }

        /// <platform arch os compiler>
        sub_node = node->first_node(XML_TAG_PLAT);
        while( sub_node )
        {
            PLATFORM plat;
            
            vcout << "\t" << sub_node->name() << ": ";

            /// os
            sub_attr = sub_node->first_attribute(XML_ATTR_OS);
            if(!sub_attr)
            {
                cout << "BAD PLATFORM: no 'os' attr" << endl;
                goto NEXT;
            }
            plat.os = sub_attr->value();
            vcout << "\t\t" << XML_ATTR_OS << ": \t" << plat.os << endl;

            /// arch
            sub_attr = sub_node->first_attribute(XML_ATTR_ARCH);
            if(!sub_attr)
            {
                cout << "BAD PLATFORM: no 'arch' attr" << endl;
                goto NEXT;
            }
            plat.arch = sub_attr->value();
            vcout << "\t\t" << XML_ATTR_ARCH << ": \t" << plat.arch << endl;

            /// compiler
            sub_attr = sub_node->first_attribute(XML_ATTR_COMPILER);
            if(!sub_attr)
            {
                cout << "BAD PLATFORM: no 'compiler' attr" << endl;
                goto NEXT;
            }
            plat.compiler = sub_attr->value();
            vcout << "\t\t" << XML_ATTR_COMPILER << ": \t" << plat.os << endl;

            mlib.platform.push_back(plat);
            sub_node = sub_node->next_sibling(XML_TAG_PLAT);
        }

        /// <depend>
        sub_node = node->first_node(XML_TAG_DEP);
        while( sub_node )
        {
            vcout << "\t" << XML_TAG_DEP << ": " << sub_node->value() << endl;
            mlib.depend.push_back(sub_node->value());

            sub_node = sub_node->next_sibling(XML_TAG_DEP);
        }

        glob_library.push_back(mlib);
NEXT:        
        node = node->next_sibling(XML_TAG_LIBRARY);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void initCompilers()
{
    glob_compiler.clear();

    cout << "Loading Compilers..." << endl;
    
    rapidxml::file<char> compf(glob_conf.compiler.c_str());
    glob_conf.compiler_xml.parse<0>( compf.data() );

    rapidxml::xml_node<> *node = glob_conf.compiler_xml.first_node(XML_TAG_CONF);

    if( !node )
    {
        cout << "Failed to load " << glob_conf.compiler <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// Parse compiler
    node = node->first_node(XML_TAG_COMPILER);
    while (node)
    {
        COMPILER mcom;
        
        rapidxml::xml_attribute<> *sub_attr;
        rapidxml::xml_node<> *sub_node, *child;

        vcout << "COMPILER\t";
        sub_attr = node->first_attribute(XML_ATTR_NAME);
        if(!sub_attr)
        {
            cout << "BAD COMPILER: no 'name' attr" << endl;
            goto NEXT;
        }
        mcom.name = sub_attr->value();
        vcout << "\t" << XML_ATTR_NAME << ":\t" << sub_attr->value() << endl;

        sub_attr = node->first_attribute(XML_ATTR_ARCH);
        if(!sub_attr)
        {
            cout << "BAD COMPILER: no 'arch' attr" << endl;
            goto NEXT;
        }
        mcom.arch = sub_attr->value();
        vcout << "\t" << XML_ATTR_ARCH << ":\t" << sub_attr->value() << endl;

        sub_attr = node->first_attribute(XML_ATTR_OS);
        if(!sub_attr)
        {
            cout << "BAD COMPILER: no 'os' attr" << endl;
            goto NEXT;
        }
        mcom.os = sub_attr->value();
        vcout << "\t" << XML_ATTR_OS << ":\t" << sub_attr->value() << endl;

        sub_attr = node->first_attribute(XML_ATTR_TARGET);
        if(!sub_attr)
        {
            cout << "BAD COMPILER: no 'target' attr" << sub_attr->value() << endl;
            goto NEXT;
        }
        mcom.target = sub_attr->value();
        vcout << "\t" << XML_ATTR_TARGET << ":\t" << sub_attr->value() <<endl;
        
        /// Parse command tag
        sub_node = node->first_node(XML_TAG_COMMAND);
        if( !sub_node )
        {
            cout << "BAD COMPILER: no 'command' tag" << endl;
            goto NEXT;
        }
        vcout << "\t" << XML_TAG_COMMAND << ": " << sub_node->value() << endl;
        mcom.command = sub_node->value();

        /// Parse command tag
        sub_node = node->first_node(XML_TAG_LINKER);
        if( !sub_node )
        {
            cout << "BAD COMPILER: no 'linker' tag" << endl;
            goto NEXT;
        }
        vcout << "\t" << XML_TAG_LINKER << ": " << sub_node->value() << endl;
        mcom.linker = sub_node->value();
        
        /// Parse flag
        sub_node = node->first_node(XML_TAG_FLAG);
        if( !sub_node )
        {
            cout << "BAD COMPILER: no 'flag' tag" << endl;
            goto NEXT;
        }
        vcout << "\t" << XML_TAG_FLAG << ":" << endl;

        child = sub_node->first_node(XML_TAG_INCPATH);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'incpath' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_incpath = child->value();
        vcout << "\t\t" << child->name() << ": " << child->value() << endl;

        child = sub_node->first_node(XML_TAG_LIBPATH);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'libpath' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_libpath = child->value();
        vcout << "\t\t" << child->name() << ": "<< child->value() << endl;
        
        child = sub_node->first_node(XML_TAG_DEFINE);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'define' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_define = child->value();
        vcout << "\t\t" << child->name() << ": "<< child->value() << endl;

        child = sub_node->first_node(XML_TAG_LIB);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'lib' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_lib = child->value();
        vcout << "\t\t" << child->name() << ": "<< child->value() << endl;

        child = sub_node->first_node(XML_TAG_BUILDOBJ);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'build_object' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_build_object  = child->value();
        vcout << "\t\t" << child->name() << ": "<< child->value() << endl;

        child = sub_node->first_node(XML_TAG_OUTPUT);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'output' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_output = child->value();
        vcout << "\t\t" << child->name() << ": "<< child->value() << endl;

        child = sub_node->first_node(XML_TAG_GENERAL);
        if(!child)
        {
            cout << "BAD COMPILER: no flag > 'general' tag" << endl;
            goto NEXT;
        }   
        mcom.flag_general = child->value();
        vcout << "\t\t" << child->name() << ": "<< child->value() << endl;

        /// Parse modes
        sub_node = node->first_node(XML_TAG_MODE);
        if( sub_node )
        {
            vcout << "\t" << XML_TAG_MODE << ":" << endl;

            child = sub_node->first_node(XML_TAG_GENERAL);
            if(child)
            {
                mcom.mode_general = child->value();
                vcout << "\t\t" << child->name() << ": "<< child->value() << endl;
            }
            child = sub_node->first_node(XML_TAG_GUI);
            if(child)
            {
                mcom.mode_gui = child->value();
                vcout << "\t\t" << child->name() << ": "<< child->value() << endl;
            }
            child = sub_node->first_node(XML_TAG_SOCK);
            if(child)
            {
                mcom.mode_sock = child->value();
                vcout << "\t\t" << child->name() << ": "<< child->value() << endl;
            }
        }

        glob_compiler.push_back(mcom);
NEXT:
        node = node->next_sibling(XML_TAG_COMPILER);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
bool loadProject()
{
    cout << "Loading Project..." << endl;
    
    rapidxml::file<char> projf(glob_proj.path.c_str());
    glob_proj.project_xml.parse<0>( projf.data() );

    rapidxml::xml_node<> *node = glob_proj.project_xml.first_node(XML_TAG_PROJECT);

    if( !node )
    {
        cout << "Failed to load " << glob_proj.path <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// Parse Project       
    rapidxml::xml_attribute<> *sub_attr;
    rapidxml::xml_node<> *sub_node, *child;

    vcout << "PROJECT:" << endl;
        
    /// Parse info tag
    sub_node = node->first_node(XML_TAG_INFO);
    if( sub_node )
    {
        vcout << "\t" << XML_TAG_INFO << ":" << endl;

        child = sub_node->first_node(XML_TAG_NAME);
        if(!child)
        {
            glob_proj.info_name = child->value();
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        }
        child = sub_node->first_node(XML_TAG_VERSION);
        if(child)
        {
            glob_proj.info_version = child->value();
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        }
        child = sub_node->first_node(XML_TAG_HACKER);
        if(child) 
        {
            glob_proj.info_hacker = child->value();
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        }
        child = sub_node->first_node(XML_TAG_DATE);
        if(child) 
        {
            glob_proj.info_date = child->value();
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        }
    }

    /// Parse code tag
    sub_node = node->first_node(XML_TAG_CODE);
    if( sub_node )
    {
        vcout << "\t" << XML_TAG_MODE << ":" << endl;

        child = sub_node->first_node(XML_TAG_INCPATH);
        while(child)
        {
            glob_proj.incpath.push_back(child->value());
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
            child = child->next_sibling(XML_TAG_INCPATH);
        }

        child = sub_node->first_node(XML_TAG_SRCPATH);
        while(child)
        {
            glob_proj.srcpath.push_back(child->value());
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
            child = child->next_sibling(XML_TAG_SRCPATH);
        }
        
        child = sub_node->first_node(XML_TAG_SOURCE);
        while(child)
        {
            glob_proj.source.push_back(child->value());
            vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
            child = child->next_sibling(XML_TAG_SOURCE);
        }
    }

    /// Parse build tag
    sub_node = node->first_node(XML_TAG_BUILD);
    vcout << "\t" << XML_TAG_BUILD << ":" << endl;
    child = sub_node->first_node(XML_TAG_TYPE);
    if(child)
    {
        glob_proj.build_type = child->value();
        vcout << "\t\t" << child->name() << ": \t" << child->value() << endl;
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
        
    child = sub_node->first_node(XML_TAG_DEP);
    while(child)
    {
        vcout << "\t\t" << XML_TAG_DEP << ": \t" << child->value() << endl;
        glob_proj.build_depend.push_back(child->value());
        child = child->next_sibling(XML_TAG_DEP);
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
            res = true;
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
    for(i=0; i<glob_library.size(); ++i)
    {
        for(int j=0; j<glob_proj.build_depend.size(); ++j)
        {
            if( glob_library[i].name == glob_proj.build_depend[j] )
            {
                for(int k=0; k < glob_library[i].platform.size(); ++k)
                {
                    if( glob_library[i].platform[k].arch       == glob_proj.arch &&
                        glob_library[i].platform[k].os         == glob_proj.target_os &&
                        glob_library[i].platform[k].compiler   == glob_proj.compiler )
                    {
                        vcout << glob_library[i].name << "|" << glob_proj.build_depend[j] << endl;
                        glob_proj.build_depend_idx.push_back(i);
                        break;
                    }
                }
                break;
            }
        }
    }

    if( glob_proj.build_depend.size() == glob_proj.build_depend_idx.size() )
    {
        return true;
    }

    cout << "UNKNOWN/INCOMPATIBLE DEPENDENIES." << endl;
    return false;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void buildProject()
{
    /// TODO
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
        cout << "Usage: " << argv[0] << "-v -c <compiler> -o <target os> -a <architecture> -p <project file>" << endl;
        cout << "-v:        verbose" << endl;
        cout << "For more information about -c, -o and -a valid options you can look at compiler.conf file, inside graver folder" << endl;
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
        else if ( opt != "-v" )
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
    for(int i=0; i<argc; ++i)
    {
        if( string(argv[i]) == string("-v") )
        {
            glob_conf.verbose = true;
            break;
        }
    }
    cout << BANNER << endl;
    cout << VERSION << " for " << glob_conf.os << endl << endl;
    
    init();
    
    normal(argc, argv);
    
    return 0;
}
