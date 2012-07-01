/*
+-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
| This file is part of Graviton.                                       |
|                                                                      |
| Graviton is free software: you can redistribute it and/or modify     |
| it under the terms of the GNU General Public License as published by |
| the Free Software Foundation, either version 3 of the License, or    |
| (at your option) any later version.                                  |
|                                                                      |
| Graviton is distributed in the hope that it will be useful,          |
| but WITHOUT ANY WARRANTY; without even the implied warranty of       |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
| GNU General Public License for more details.                         |
|                                                                      |
| You should have received a copy of the GNU General Public License    |
| along with Graviton.  If not, see <http://www.gnu.org/licenses/>.    |
+-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
*/


#ifndef _GVN_HEAD_
#define _GVN_HEAD_

// PRE-DEFINED OPTIONS
/*! My Lovely Define! */
#define _null_ 0x00 //NULL

/*! Activate/Deactivate GraVitoN logger */
#define GVN_ACTIVATE_LOGGER 1
/*! Print logs into a file */
#define GVN_LOG_INTO_FILE 0
/*! Path to Log File */
#define GVN_LOG_FILE "graviton_logs.txt"

// ------------ HEADERS ----------------- //
#include "gvn_utils/gvn_logger.hpp"
#include "gvn_utils/gvn_optparser.hpp"
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdio>
using namespace std;

//! Informations About A Component
class Gvn_Info
{
public:
	//! Component's name
    string name;
	//! Component's version
    string version;
	//! Component's compatible OS
    string os;
	//! Component's compatible CPU architecture
    string architecture;
	//! Component's compatible harware architecture
    string hardware;
	//! More detailed information about component
    string about;

    Gvn_Info ( string _name = "",
               string _version = "",
               string _os = "",
               string _architecture = "",
               string _hardware = "",
               string _about = "" );
};

class Gvn_Component
{
protected:
    Gvn_Info info;
    string options;

public:
    Gvn_Component();
    virtual ~Gvn_Component();

    virtual bool initialize ( const string &_DO_NOT_USE_THIS_INITIALIZER_ = "" );
    virtual Gvn_Info getInfo() const;
};

Gvn_Info::Gvn_Info ( string _name,
                     string _version,
                     string _os,
                     string _architecture,
                     string _hardware,
                     string _about )
{
    name = _name;
    version = _version;
    os = _os;
    architecture = _architecture;
    hardware = _hardware;
    about = _about;
}

Gvn_Component::Gvn_Component()
{
    options = "<no_options>";
    info.about = "<component_about>";
    info.architecture = "<component_art>";
    info.hardware = "<component_hw";
    info.name = "<component_name>";
    info.os = "<component_os>";
    info.version = "<component_ver";
}

Gvn_Component::~Gvn_Component()
{
}

bool Gvn_Component::initialize ( const string &_DO_NOT_USE_THIS_INITIALIZER_ )
{
    options = _DO_NOT_USE_THIS_INITIALIZER_;
	
    return 0;
}

Gvn_Info Gvn_Component::getInfo() const
{
    return info;
}

#endif // _GVN_HEAD_
