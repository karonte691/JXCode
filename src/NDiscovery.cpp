/*

	JXCode v 0.1 Alpha 

	Author: Luca Magistrelli(blackstorm010@gmail.com)

	This file is part of JXCode

    JXCode is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JXCode is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JXCode.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <WinSock.h>
#include <WindowsX.h>

#include "icmpdefs.h"
#include "NDiscovery.h"
#include "report.h"

//taken and adapted from http://tangentsoft.net/wskfaq/examples/dllping.html
void Find_Net(char *subnet, bool save)
{
	FILE *fp;
	HINSTANCE hIcmp = LoadLibrary((LPCWSTR)"ICMP.DLL");
	if (hIcmp == 0)
	{
		printf("Error: icmp.dll missing...\n");
		return;
	}
	typedef HANDLE (WINAPI* pfnHV)(VOID);
	typedef BOOL (WINAPI* pfnBH)(HANDLE);
	typedef DWORD (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
	pfnHV pIcmpCreateFile;
	pfnBH pIcmpCloseHandle;
	pfnDHDPWPipPDD pIcmpSendEcho;
	pIcmpCreateFile = (pfnHV)GetProcAddress(hIcmp, "IcmpCreateFile");
	pIcmpCloseHandle = (pfnBH)GetProcAddress(hIcmp, "IcmpCloseHandle");
    pIcmpSendEcho = (pfnDHDPWPipPDD)GetProcAddress(hIcmp,"IcmpSendEcho");
	if ((pIcmpCreateFile == 0) || (pIcmpCloseHandle == 0) || (pIcmpSendEcho == 0)) {
			 printf("unable to create the functions...");
			 return;		
	}
	for(int i=1; i <= 255; i++)
	{
		char *ip;
		ip = (char *)sprintf("%s.%c", subnet, (char)i);
		if(save == true) *fp = MakeReport(ip);
		struct hostent* phe;
		if((phe = gethostbyname(ip)) == 0)
		{
			printf("error to convert ip address..");
			return;
		}
		
		HANDLE hIP = pIcmpCreateFile();
		if (hIP == INVALID_HANDLE_VALUE)
		{
			printf("Unable to open ping service");
			return;
		}
		if(save == true) FileHeader(fp, ip);
		char acPingBuffer[64];
		memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
		PIP_ECHO_REPLY pIpe = (PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
		if (pIpe == 0) {
			printf("Failed to allocate global ping packet buffer.");
			return;
		}
		pIpe->Data = acPingBuffer;
		pIpe->DataSize = sizeof(acPingBuffer); 
		DWORD dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), acPingBuffer, sizeof(acPingBuffer), NULL, pIpe, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);
		if (dwStatus != 0) {
			//found
			printf("[FOUND] %s", ip);
			if(save == true) WriteOnReport(fp, ip);
		}
		else
			continue;
		GlobalFree(pIpe);
	}
	if(save == true) SaveReport(fp);
	FreeLibrary(hIcmp);
}