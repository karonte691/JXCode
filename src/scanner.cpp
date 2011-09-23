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

#include <WinSock.h>
#include <stdio.h>

#include "scanner.h"
#include "report.h"

char *getIp(char *domain)
{
	if(!strstr(domain, "www"))
	{	
		return domain;
	}
	else
	{
		return (char *)gethostbyname(domain);
	}
	return NULL;
}

void ScanIp(char *ip, int start, int end, bool save)
{
	int s = 0, e = 0;
	SOCKET sock;
	SOCKADDR_IN sock_addr;
	FD_SET fds;
	int exist = 0;
	timeval timeout;
	u_long host;
	FILE *fp;

	sock_addr.sin_family = AF_INET;
	host = inet_addr(ip);
	if(host == INADDR_NONE)
	{
		printf("Invalid hostname...");
		return;
	}
	if(save == true) *fp = MakeReport(ip);
	sock_addr.sin_addr = (const in_addr &)host;
	timeout.tv_sec = 0;
	timeout.tv_usec = 320000;
	if(save == true) FileHeader(fp, ip);
	if(start == 0 && end == 0)
	{
		s = 1;
		e = 1000;
	}
	for(int i = s; i <= e; i++)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0); 
		sock_addr.sin_port = i;
		ioctlsocket(sock, FIONBIO, (u_long*)1);
		FD_ZERO(&fds);
		FD_SET(sock, &fds);
		connect(sock, (PSOCKADDR)&sock_addr,sizeof(struct sockaddr));
		exist = select(sock, 0, &fds, 0, (const timeval *)&timeout);
		if(exist > 1)
		{
			printf("[FOUND] %d is open\n", i);
			if(save == true) WriteOnReport(fp, (char *)("[FOUND] %d is open\n", i));
		}
	}
	if(save == true) SaveReport(fp);
	closesocket(sock); 
}

