/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#include "graver.hpp"

#include <external/rapidxml/rapidxml.hpp>
#include <external/rapidxml/rapidxml_utils.hpp>
// #include <external/rapidxml/rapidxml_print.hpp>

#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct PROJECT
{
    bool unsaved_proj;
    string path;
    string compiler;
    string target_os;
    string arch;

    rapidxml::xml_document<> project_xml;
    
    PROJECT();
} glob_proj;

PROJECT::PROJECT()
{
    unsaved_proj = false;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
struct CONF
{
    const string library;
    const string compiler;

    rapidxml::xml_document<> compiler_xml;
    rapidxml::xml_document<> library_xml;

    CONF();
} glob_conf;

CONF::CONF():library("library.conf"), compiler("compiler.conf")
{
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
void parse(const string &input)
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
void initLibs()
{
    rapidxml::file<char> libf(glob_conf.library.c_str());
    glob_conf.library_xml.parse<0>( libf.data() );

    rapidxml::xml_node<> *node = glob_conf.library_xml.first_node(XML_TAG_CONF);

    if( !node )
    {
        cout << "Failed to load " << glob_conf.library <<", which is fuckin' necessary!" << endl;
        exit(0);
    }

    /// Parse library
    node = node->first_node(XML_TAG_LIBRARY);
    while (node)
    {
        rapidxml::xml_attribute<> *sub_attr;
        rapidxml::xml_node<> *sub_node;

        sub_attr = node->first_attribute(XML_ATTR_NAME);
        cout << "LIBRARY\t\t" <<  sub_attr->name() << ": " << sub_attr->value() << endl;
            
        /// Parse include tags  
        sub_node = node->first_node(XML_TAG_INC);
        while( sub_node )
        {
            cout << "\t" << sub_node->name() << ": " << sub_node->value() << endl;

            sub_node = sub_node->next_sibling(XML_TAG_INC);
        }

        /// Parse platform tags
        sub_node = node->first_node(XML_TAG_PLAT);
        while( sub_node )
        {
            cout << "\t" << sub_node->name() << ": ";

            for (sub_attr = sub_node->first_attribute(); sub_attr; sub_attr = sub_attr->next_attribute())
            {
                cout << sub_attr->name() << "= " << sub_attr->value() << " \t";
            }
            cout << endl;
                
            sub_node = sub_node->next_sibling(XML_TAG_PLAT);
        }

        /// Parse deps  
        sub_node = node->first_node(XML_TAG_DEP);
        while( sub_node )
        {
            cout << "\t" << sub_node->name() << ": " << sub_node->value() << endl;

            sub_node = sub_node->next_sibling(XML_TAG_DEP);
        }
        
        node = node->next_sibling(XML_TAG_LIB);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void initCompilers()
{
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
        rapidxml::xml_attribute<> *sub_attr;
        rapidxml::xml_node<> *sub_node, *child;

        cout << "COMPILER\t";
        for (sub_attr = node->first_attribute();
             sub_attr; sub_attr = sub_attr->next_attribute())
        {
            cout <<  sub_attr->name() << ": " << sub_attr->value() << "\t";
        }
        cout << endl;
        
        /// Parse command tag
        sub_node = node->first_node(XML_TAG_COMMAND);
        if( sub_node )
        {
            cout << "\t" << sub_node->name() << ": " << sub_node->value() << endl;

            sub_node = sub_node->next_sibling(XML_TAG_COMMAND);
        }

        /// Parse flag
        sub_node = node->first_node(XML_TAG_FLAG);
        if( sub_node )
        {
            cout << "\t" << XML_TAG_FLAG << ":" << endl;

            child = sub_node->first_node(XML_TAG_INCPATH);
            if(child) cout << "\t\t" << child->name() << ": " << child->value() << endl;
            child = sub_node->first_node(XML_TAG_LIBPATH);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_DEFINE);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_LIB);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_BUILDOBJ);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_OUTPUT);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_GENERAL);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
        }

        /// Parse modes
        sub_node = node->first_node(XML_TAG_MODE);
        if( sub_node )
        {
            cout << "\t" << XML_TAG_MODE << ":" << endl;

            child = sub_node->first_node(XML_TAG_GENERAL);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_GUI);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
            child = sub_node->first_node(XML_TAG_SOCK);
            if(child) cout << "\t\t" << child->name() << ": "<< child->value() << endl;
        }
        
        node = node->next_sibling(XML_TAG_COMPILER);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void init()
{
    initCompilers();
    initLibs();
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/// TODO: Interactive Mode
void interactive()
{
    string input;

    init();

    cout << "[ There are rules... to be broken. ]" << endl;
    cout << VERSION << endl << endl;
    
    while( (cout << SIGN, getline(cin, input)) )
    {
        parse(input);
    }
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void loadProject()
{
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

    cout << "PROJECT:" << endl;
        
    /// Parse info tag
    sub_node = node->first_node(XML_TAG_INFO);
    if( sub_node )
    {
        cout << "\t" << XML_TAG_INFO << ":" << endl;

        child = sub_node->first_node(XML_TAG_NAME);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        child = sub_node->first_node(XML_TAG_VERSION);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        child = sub_node->first_node(XML_TAG_HACKER);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        child = sub_node->first_node(XML_TAG_DATE);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
    }

    /// Parse code tag
    sub_node = node->first_node(XML_TAG_CODE);
    if( sub_node )
    {
        cout << "\t" << XML_TAG_MODE << ":" << endl;

        child = sub_node->first_node(XML_TAG_INCPATH);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        child = sub_node->first_node(XML_TAG_SRCPATH);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
        child = sub_node->first_node(XML_TAG_SOURCE);
        if(child) cout << "\t\t" << child->name() << ": \t" << child->value() << endl;
    }

    /// Parse build tag
    sub_node = node->first_node(XML_TAG_BUILD);
    cout << "\t" << XML_TAG_BUILD << ":" << endl;

    child = sub_node->first_node(XML_TAG_PLAT);
    while(child)
    {
        cout << "\t\t" << XML_TAG_PLAT << ": \t";
        for (sub_attr = child->first_attribute();
             sub_attr; sub_attr = sub_attr->next_attribute())
        {
            cout <<  sub_attr->name() << ": \t" << sub_attr->value() << "\t";
        }
        cout << endl;

        child = child->next_sibling(XML_TAG_PLAT);
    }
        
    child = sub_node->first_node(XML_TAG_DEP);
    while(child)
    {
        cout << "\t\t" << XML_TAG_DEP << ": \t" << child->value() << endl;
        child = child->next_sibling(XML_TAG_DEP);
    }
        
    node = node->next_sibling(XML_TAG_COMPILER);
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
void normal(int argc, char **argv)
{
    cout << VERSION << endl << endl;
    
    if( argc != 9 )
    {
        cout << "Usage: " << argv[0] << "-c <compiler> -o <target os> -a <architecture> -p <project file>" << endl;
        cout << "For more information about -c, -o and -a valid options you can look at compiler.conf file, inside graver folder" << endl;
    }

    for(int i=1; i<argc; i+=2)
    {
        string opt = argv[i];

        if( opt == "-c" || opt == "-C" )
        {
            glob_proj.compiler = argv[i+1];
        }
        else if ( opt == "-o" || opt == "-O" )
        {
            glob_proj.target_os = argv[i+1];
        }
        else if ( opt == "-a" || opt == "-A" )
        {
            glob_proj.arch = argv[i+1];
        }
        else if ( opt == "-p" || opt == "-P" )
        {
            glob_proj.path = argv[i+1];
        }
        else
        {
            printError(ERR_UNK_CMD);
        }
    }

    cout << "Loading project..." << endl;
    loadProject();
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
int main(int argc, char **argv)
{
    cout << BANNER << endl;

    normal(argc, argv);
    
    return 0;
}
