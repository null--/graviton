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
 * @brief Various file-based operations
 *
*/

#ifndef _GVN_FILES_HEAD_
#define _GVN_FILES_HEAD_

#include "gvn_logger.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

namespace GraVitoN
{

/// Various file-based operations
namespace File
{

const unsigned long MAX_FILE_SIZE = 32 * 1024 * 1024 * sizeof(unsigned char); // 64 MB

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Load a binary file into an unsigned char array
 * 
 * @param [i] path
 * Path to binary file
 *
 * @param [out] buffer
 * Pointer to your buffer pointer (double pointer). we needs your pointer to buffer pointer
 * because after return of this function c++ will free your allocated buffer memory.
 * see C++ refrence for more information. \n
 * if buffer == NULL then loadFile is failed
 *
 * @return
 * Size of our buffer, or 0 and if it's failed
 *
 */
unsigned long loadFile(const string &path, unsigned char **buffer)
{
	unsigned long size = 0;
	
	if( *buffer != NULL)
	  free(*buffer);
	
	FILE *file;
	
	Logger::logIt("Reading Binary File... ");
	file = fopen(path.c_str(), "rb");
	if( file )
	{
		*buffer = (unsigned char*)malloc(MAX_FILE_SIZE);
		size = 0;
		
		while( !feof(file) )
		{
			unsigned char ch = fgetc(file);
			//printf("%.9d < %.2x\n", size, (*buffer)[size]);
			if(!feof(file))
			{
				*(*buffer + size) = ch;
				size = size + 1;
			}
		}
		Logger::logIt("[Size: "); Logger::logIt(size);
		Logger::logItLn("] DONE");
		
		fclose(file);
	}
	else
	{
		*buffer = NULL;
	}
	return size;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Save your buffer as a binary file
 *
 * @param [in] opath
 * Path to output file
 *
 * @param [in] buffer
 * Pointer to your buffer
 *
 * @param [in] size
 * Size of your buffer
 *
 * @return
 * True if buffer was saved to file successfully
 */
bool saveUChars(const string opath, const unsigned char *buffer, const unsigned long &size, const bool append = false)
{
	FILE *outf;
	outf = (append)?fopen(opath.c_str(), "ab+"):fopen(opath.c_str(), "wb+");

	if( !outf )
		return false;
	
	GraVitoN::Logger::logIt("Writing to binary file... ");
	for(register unsigned long i = 0; i < size; ++i)
	{
		fputc(*(buffer + i), outf);
		//printf("%.9d > %.2x\n", i, buffer[i]);
	}
	Logger::logItLn("DONE");
	
	fclose(outf);

	return true;

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Save your buffer as a cpp file
 *
 * @param [in] opath
 * Path to output file
 *
 * @param [in] buffer
 * Pointer to your buffer
 *
 * @param [in] size
 * Size of your buffer
 *
 * @return
 * True if buffer was saved to file successfully
 */
bool printUCharsIntoFile(const string opath, const unsigned char *buffer, const unsigned long &size)
{
	FILE *outf;
	outf = fopen(opath.c_str(), "w+");

	if( !outf )
		return false;

	int mod = 0;
	fprintf(outf, "const unsigned long buf_len = %lu;\n"
					"unsigned char buf[] = \n",
					size);
	Logger::logIt("Writing to cpp file... ");
	Logger::logIt("Size: ");
	Logger::logIt("Size: ");
	for(unsigned long i = 0; i < size; ++i)
	{
		//printf(".2x\n", buffer[i]);
		if( mod % 16 == 0 )
		{
			fprintf(outf, "\"");
			mod = 0;
		}
		fprintf(outf, "\\x%.2x", buffer[i]);

		if( mod % 16 == 15 )
		{
			fprintf(outf, "\"\n");
			mod = 0;
		}
		else
		{
			++mod;
		}
	}
	if( (mod > 0) && (mod % 16 != 15) )
	{
		fprintf(outf, "\"");
		mod = 0;
	}
	Logger::logItLn("DONE");

	fprintf(outf, "\n;");
	fclose(outf);

	return true;
}

}

}

#endif // _GVN_FILES_HEAD_
