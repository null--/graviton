#ifndef _LUAVITON_ZLIB_HEAD_
#define _LUAVITON_ZLIB_HEAD_

#include "graviton.hpp"
#include "core/luaviton.hpp"
#include <external/luazlib.hpp>

namespace GraVitoN
{

namespace Core
{

// #ifdef GVN_DEBUG
// #endif

class Luaviton_ZLib : public Luaviton_Module<Luaviton_ZLib>
{
protected:
    string MODULE_GZIP;

public:
    Luaviton_ZLib (); // (Luaviton &_luaviton_instance);

    virtual ~Luaviton_ZLib() {}

    void registerModule()
    {
        luaviton.preloadModule("zlib", luaopen_zlib);
    }

    bool loadEmAll()
    {
        return luaviton.loadModuleString(MODULE_GZIP);
    }
};

Luaviton_ZLib::Luaviton_ZLib () // (Luaviton &_luaviton_instance) : Luaviton_Module(_luaviton_instance)
{
    MODULE_GZIP =
            "--[===================================================================[\n"
            "TODO:\n"
            "	- add a nice header with license and stuff..\n"
            "	- detect plain text files\n"
            "	- proper testing\n"
            "	- improve implementation?\n"
            "	- check gzopen flags for consistency (ex: strategy flag)\n"
            "--]===================================================================]\n"
            "\n"
            "local io = require 'io'\n"
            "local zlib = require 'zlib'\n"
            "\n"
            "local error, assert, setmetatable, tostring = error, assert, setmetatable, tostring\n"
            "\n"
            "module('gzip')\n"
            "\n"
            "function open(filename, mode)\n"
            "	mode = mode or 'r'\n"
            "	local r = mode:find('r', 1, true) and true\n"
            "	local w = mode:find('w', 1, true) and true\n"
            "	local level = -1\n"
            "\n"
            "	local lstart, lend = mode:find('%d')\n"
            "	if (lstart and lend) then\n"
            "		level = mode:sub(lstart, lend)\n"
            "	end\n"
            "\n"
            "	if (not (r or w)) then\n"
            "		error('file open mode must specify read or write operation')\n"
            "	end\n"
            "\n"
            "	local f, z\n"
            "\n"
            "	local mt = {\n"
            "		__index = {\n"
            "			read = function(self, ...)\n"
            "				return z:read(...)\n"
            "			end,\n"
            "			write = function(self, ...)\n"
            "				return z:write(...)\n"
            "			end,\n"
            "			seek = function(self, ...)\n"
            "				error 'seek not supported on gzip files'\n"
            "			end,\n"
            "			lines = function(self, ...)\n"
            "				return z:lines(...)\n"
            "			end,\n"
            "			flush = function(self, ...)\n"
            "				return z:flush(...) and f:flush()\n"
            "			end,\n"
            "			close = function(self, ...)\n"
            "				return z:close() and f:close()\n"
            "			end,\n"
            "		},\n"
            "		__tostring = function(self)\n"
            "			return 'gzip object (' .. mode .. ') [' .. tostring(z) .. '] [' .. tostring(f) .. ']'\n"
            "		end,\n"
            "	}\n"
            "\n"
            "	if r then\n"
            "		f = assert(io.open(filename, 'rb'))\n"
            "		z = assert(zlib.inflate(f))\n"
            "	else\n"
            "		f = assert(io.open(filename, 'wb'))\n"
            "		z = assert(zlib.deflate(f, level, nil, 15 + 16))\n"
            "	end\n"
            "\n"
            "	return setmetatable({}, mt)\n"
            "end\n"
            "\n"
            "function lines(filename)\n"
            "	local gz = open(filename, 'r')\n"
            "	return function()\n"
            "		local line = gz and gz:read()\n"
            "		if line == nil then\n"
            "			gz:close()\n"
            "		end\n"
            "		return line\n"
            "	end\n"
            "end\n"
            ;
}

}
}
#endif
