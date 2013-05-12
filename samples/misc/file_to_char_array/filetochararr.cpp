/// Activate/Deactivate GraVitoN logger
#define GVN_ACTIVATE_LOGGER

#include <cassert>
#include <iostream>
#include <graviton.hpp>
#include <utils/files.hpp>

int main(int argc, char *argv[]) 
{
	assert(argc == 2);
	string path(argv[1]);
	
    GraVitoN::Core::Logger::logItLn("Running...");
	
	unsigned char *buff = _null_;
	unsigned long buff_size;
	
    buff_size = GraVitoN::Utils::File::loadFile(path, &buff);
    GraVitoN::Core::Logger::logItLn(buff_size);
    GraVitoN::Utils::File::saveUChars(path+".bin", buff, buff_size);
    GraVitoN::Utils::File::printUCharsIntoFile(path+".cpp", buff, buff_size);
	
	return 0;
}
