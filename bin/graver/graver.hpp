#ifndef _GRAVER_HEAD_
#define _GRAVER_HEAD_

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#include <string>
using namespace std;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
const char BANNER[] = 
"                        ______                  \n"
"     ___________________)     \\                \n"
"     \\      __________________ \\              \n"
"      \\    /           __     \\ \\            \n"
"      /   /            \\ \\_    \\ \\          \n" 
"     /   /             \\ \\ \\__  \\)_______   \n"
"    /   /               \\----/   /_/     \\    \n"
"   /   /                        /         \\    \n"
"  /   /   \\_____               /________   \\  \n"
" /    |    \\___ \\_            //        \\  / \n"
"/__   |        \\_ \\_____     ((          \\/  \n"
"   \\  |          \\_____ \\__   \\\\           \n" 
"   /  |                \\   \\__ \\\\           \n"
"  /   |                 \\__   \\ \\\\_______   \n"
"  \\   \\                    \\   \\            \n"
"   \\   \\                    \\   \\           \n"
"    \\   \\____                \\   \\          \n"
"     \\  ___  \\                \\   \\         \n"
"      \\/   \\  \\________________\\   \\       \n"
"            \\______________________/           \n"
"                                                \n"  
"            GraVitoN: Scream of Pain            \n"
"                                                \n"
"                      http:\\www.thegraviton.org\n"
;

const char VERSION[] =
    "Graver - Version: 0.2 EXPERIMENTAL";
    
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
const string SIGN           = "graver# ";

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
const int ERR_INV_VAL       = 0;
const int ERR_UNK_CMD       = 1;
const int ERR_INV_FILE      = 2;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
const string CMD[]=
{
    "new",
    "build",
    "load",
    "exit",
    "build-dep",
    "save",
    "help"
};

const int N_CMD             = 7;

const int CMD_NEW           = 0;
const int CMD_BUILD         = 1;
const int CMD_LOAD          = 2;
const int CMD_EXIT          = 3;
const int CMD_BUILD_DEP     = 4;
const int CMD_SAVE          = 5;
const int CMD_HELP          = 6;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
const string GRAV_LIB_PATH  = "libs/";
const string GRAV_CODE_PATH = "code/";

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
const string BUILD_TYPE_EXEC        = "exec";
const string BUILD_TYPE_STATLIB     = "statlib";
const string BUILD_TYPE_DYNLIB      = "dynlib";
const string BUILD_TYPE_OBJ         = "obj";
const string BUILD_DIRECTORY        = "build";

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/*
  Project Sample:

  <project>
    <info>
    </info>
  </project>
 */
const char XML_TAG_CONF[]           = "conf";
const char XML_TAG_LIBRARY[]        = "library";
const char XML_TAG_COMPILER[]       = "compiler";
const char XML_TAG_COMMAND[]        = "command";
const char XML_TAG_OBJ_EXTENSION[]  = "obj_extension";
const char XML_TAG_DEP[]            = "depend";
const char XML_TAG_FILE[]           = "file";
const char XML_TAG_FLAG[]           = "flag";
const char XML_TAG_MODE[]           = "mode";
const char XML_TAG_INC[]            = "include";
const char XML_TAG_PLAT[]           = "platform";
const char XML_TAG_INCPATH[]        = "incpath";
const char XML_TAG_LIBPATH[]        = "libpath";
const char XML_TAG_LIB[]            = "lib";
const char XML_TAG_BUILDOBJ[]       = "build_object";
const char XML_TAG_OUTPUT[]         = "output";
const char XML_TAG_GENERAL[]        = "general";
const char XML_TAG_EXTRA_LIBS[]     = "extra_libs";
const char XML_TAG_GUI[]            = "gui";
const char XML_TAG_CONSOLE[]        = "console";
const char XML_TAG_SOCK[]           = "sock";
const char XML_TAG_VERSION[]        = "version";
const char XML_TAG_HACKER[]         = "hacker";
const char XML_TAG_DATE[]           = "date";
const char XML_TAG_CODE[]           = "code";
const char XML_TAG_BUILD[]          = "build";
const char XML_TAG_PROJECT[]        = "project";
const char XML_TAG_INFO[]           = "info";
const char XML_TAG_SOURCE[]         = "source";
const char XML_TAG_SRCPATH[]        = "srcpath";
const char XML_TAG_NAME[]           = "name";
const char XML_TAG_DEFINE[]         = "define";
const char XML_TAG_LINKER[]         = "linker";
const char XML_TAG_TYPE[]           = "type";
const char XML_TAG_COMPILER_OPT[]   = "compiler_option";

const char XML_ATTR_OS[]            = "os";
const char XML_ATTR_TARGET[]        = "target";
const char XML_ATTR_ARCH[]          = "arch";
const char XML_ATTR_COMPILER[]      = "compiler";
const char XML_ATTR_NAME[]          = "name";

#endif // end of _GRAVER_HEAD_
