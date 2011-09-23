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
#include <Windows.h>

#include "report.h"



FILE MakeReport(char *nome_host)
{
	FILE *fp;
	fp = fopen(nome_host, "w");
	return *fp;
}

void FileHeader(FILE *fp, char *host)
{
	fprintf(fp, "===========================================\n");
	fprintf(fp, "               JXCode Report               \n");
	fprintf(fp, "===========================================\n\n");
	fprintf(fp, "Host: %s\n", host);
	fprintf(fp, "Generated on: \n\n");
}

void WriteOnReport(FILE *fp, char *line)
{
	fprintf(fp, line);
}

void SaveReport(FILE *fp)
{
	fclose(fp);
}
