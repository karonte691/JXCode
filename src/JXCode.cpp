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
#include <string.h>
#include <WinSock.h>


#include "scanner.h"
#include "NDiscovery.h"
#include "report.h"

void Banner()
{
	printf("JXCode v 0.1\n\n");
	printf("Network discovery and port scanner tool\n");

}
void usage()
{
		printf("Usage:\n");
		printf("jxcode.exe -option\n");
		printf("Options:\n");
		printf("-scan_network (perform a subnet network discovery)\n");
		printf("-scan_ip ip [PORT_START] [PORT END]\n");
		printf("   Example: scan_ip 192.168.0.1 1 1000\n");
		printf("   Example 2: scan_ip www.google.it\n");
}
int main(int argc, char *argv[])
{
	bool range, saveit;
	char *sc,save;
	WSADATA wsdata;
	if(argc < 2)
	{
		usage();

		return 1;
	}
	Banner();
	printf("Init socket...\n");
	//init wsadata
	if(WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
	{
		printf("Error init socket :(");
		return 1;
	}
	printf("Socket has been loaded!\n");
	sc = argv[1];
	
	if(strstr(sc,"-scan_ip"))
	{
		if(strlen(argv[2]) == 0)
		{
			printf("error: missing ip parameter...\n");
			usage();
			return 1;
		}
		//check if the start port and the end port are valid
		range = strlen(argv[3]) != 0 && strlen(argv[4]) != 0 
				&& (int)argv[3] > 0 && (int)argv[3] <= 65535 
				&& (int)argv[4] > 0 && (int)argv[4] <= 65535 ? true : false; 

		printf("do you want to save the results(y/n): ");
		scanf("%c", &saveit);
		saveit = save == 'y' || save == 'Y' ? true : false;
		printf("init scan on %s", argv[2]);
		if(range)
			ScanIp(argv[2], (int)argv[3], (int)argv[4], saveit);
		else
			ScanIp(argv[2], 0, 0, saveit);
	}
	else if(strstr(sc,"-scan_network"))
	{
		char *subnet;
		printf("Insert the subdomain address whitout the class D(e.g 192.168.1): ");
		scanf("%s", &subnet);
		printf("do you want to save the results(y/n): ");
		scanf("%c", &save);
		saveit = save == 'y' || save == 'Y' ? true : false;
		printf("Init search...");
		Find_Net(subnet, saveit);
	}
	else
		usage();
	WSACleanup();
	return 0;
}

