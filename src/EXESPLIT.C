/*
 * Borrowed from Extended Operating System Loader
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found at http://www.gnu.org
 */

#include <io.h>
#include <exehead.h>
#include <stdio.h>
#include <stdlib.h>

void Usage(void);
void DoExesplit(char *, char *);
void WriteHeader(int iInputFileHandle, char *);
void WriteData(int iInputFileHandle, char *);

static char baBuffer[32768];

void main(int argc, char **argv) {
	if( argc==3 ) DoExesplit(argv[1],argv[2]);
 	else Usage();

	exit(0);
}

void Usage(void) {
 	fprintf(stderr,	"Usage: exesplit part.exe xrpart00.xxf\n");
	exit(1);
}

void DoExesplit(char *strSourceFilename, char *strTargetFilename) {
	int iInputFileHandle;
	iInputFileHandle = open(strSourceFilename, O_RDONLY);
	WriteHeader(iInputFileHandle, strTargetFilename);
	WriteData(iInputFileHandle, strTargetFilename);
	close(iInputFileHandle);
}

void WriteHeader(int iInputFileHandle, char *strTargetFilename) {
	exehead_dos_sixteenbit ehdstbInputFileHeader;
	unsigned long ulOutputFileParts;
	unsigned long ulOutputFileSizeWithoutHeader;
	unsigned long ulOutputRestHeaderSize;

	int iOutputFileHandle = create(strTargetFilename);
	unsigned uInputFileHeaderSize = sizeof(ehdstbInputFileHeader);
	unsigned long ulInputFileSize = getSize(iInputFileHandle);
	
	read(iInputFileHandle, &ehdstbInputFileHeader, uInputFileHeaderSize);

	ulOutputRestHeaderSize = ehdstbInputFileHeader.e_cparhdr * 16 - uInputFileHeaderSize;
	ulOutputFileSizeWithoutHeader = (unsigned long)((unsigned long)ulInputFileSize - ((unsigned long)ehdstbInputFileHeader.e_cparhdr * 16));
	ulOutputFileParts = ulInputFileSize;
	ulOutputFileParts -= (unsigned long)(ehdstbInputFileHeader.e_cparhdr * 16);
	ulOutputFileParts = 1 + ulOutputFileParts / 32768 + !!(ulOutputFileParts % 32768);
	ulOutputFileParts--;
	ehdstbInputFileHeader.e_res2[8] = (unsigned short)(ulOutputFileSizeWithoutHeader - ((ulOutputFileParts - 1) * 32768));
	ehdstbInputFileHeader.e_res2[9] = (unsigned short)ulOutputFileParts;

	write(iOutputFileHandle,&ehdstbInputFileHeader,uInputFileHeaderSize);
	read(iInputFileHandle,baBuffer,ulOutputRestHeaderSize);
	write(iOutputFileHandle,baBuffer,ulOutputRestHeaderSize);
	close(iOutputFileHandle);
	++strTargetFilename[7];
}

void WriteData(int iInputFileHandle, char *strTargetFilename) {
	unsigned short Bytes;
	int iOutputFileHandle;

	while ((Bytes = read(iInputFileHandle,baBuffer,32768)) != 0) {
		iOutputFileHandle = create(strTargetFilename);
		write(iOutputFileHandle,baBuffer,Bytes);
		close(iOutputFileHandle);
		++strTargetFilename[7];
	}
}