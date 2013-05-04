/// Activate/Deactivate GraVitoN logger
#define GVN_ACTIVATE_LOGGER

#include <cassert>
#include <iostream>
#include "../../../graviton.hpp"
#include "../../../gvn_utils/gvn_files.hpp"

int main(int argc, char *argv[]) 
{
	assert(argc == 2);
	string path(argv[1]);
	
	GraVitoN::Logger::logItLn("Running...");
	
	unsigned char *buff = _null_;
	unsigned long buff_size;
	
	buff_size = GraVitoN::File::loadFile(path, &buff);
	GraVitoN::Logger::logItLn(buff_size);
	GraVitoN::File::saveUChars(path+".bin", buff, buff_size);
	GraVitoN::File::printUCharsIntoFile(path+".cpp", buff, buff_size);
	
	return 0;
}
