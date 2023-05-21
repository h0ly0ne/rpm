/*
 * Borrowed from Extended Operating System Loader
 * Copyright (c) 1999 by Geurt Vos
 * Adopted and extended for Ranish Partition Manager
 * Copyright (c) 2023 by Oliver Oswald
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found at http://www.gnu.org
 */

#include "IO.H"
#include "EXEHEAD.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Usage(void);
void Split(char *, char *);
void Merge(char *, char *);
void WriteHeader(int iInputFileHandle, char *);
void WriteData(int iInputFileHandle, char *);

static char *baBuffer;

void main(int argc, char **argv) {
    baBuffer = (char *)malloc(sizeof(char));

	if(argc == 4) {
        if(strcmpi(argv[1], "split")==0) {
            Split(argv[2], argv[3]);
        }
        else if(strcmpi(argv[1], "merge")==0) {
            Merge(argv[2], argv[3]);
        }
        else {
            Usage();
        }
    } 
 	else {
        Usage();
    }

	exit(0);
}

void Usage(void) {
 	fprintf(stderr,	"Usage: EXESPLIT.EXE SPLIT <SOURCEFILE> <TARGETFILE(S)>\n");
    fprintf(stderr,	"       EXESPLIT.EXE MERGE <SOURCEFILE(S)> <TARGETFILE>\n");
    fprintf(stderr,	"\n");
    fprintf(stderr,	"Example: EXESPLIT.EXE SPLIT PART.EXE XRPART00.XXF\n");
    fprintf(stderr,	"         EXESPLIT.EXE MERGE XRPART00.XXF PART.EXE\n");

	exit(1);
}

void Split(char *strSourceFilename, char *strTargetFilename) {
	int iInputFileHandle;
    unsigned long ulInputFileSize = 0;
	iInputFileHandle = open(strSourceFilename, O_RDONLY);
    ulInputFileSize = (unsigned long)getSize(iInputFileHandle);

    if(ulInputFileSize > 32768) {
	    WriteHeader(iInputFileHandle, strTargetFilename);
	    WriteData(iInputFileHandle, strTargetFilename);
    }
    else {
        WriteData(iInputFileHandle, strTargetFilename);
    }
	close(iInputFileHandle);
}

void Merge(char *strSourceFilename, char *strTargetFileName) {
    exehead_dos_sixteenbit ehdstbInputFileHeader;
    unsigned short ulInputFileParts;
    unsigned short ulInputLastPartSize;
    int iInputFileHandle;
    int iOutputFileHandle = create(strTargetFileName);
    unsigned long ulInputFileSize = 0;
    unsigned long ulOutputFileSize = 0;
    unsigned int uInputFileIndex = 0;

    iInputFileHandle = open(strSourceFilename, O_RDONLY);
    ulInputFileSize = (unsigned long)getSize(iInputFileHandle);
    read(iInputFileHandle, &ehdstbInputFileHeader, (unsigned int)sizeof(ehdstbInputFileHeader));
    ulInputLastPartSize = (unsigned short)ehdstbInputFileHeader.e_csum;
	ulInputFileParts = (unsigned short)ehdstbInputFileHeader.e_ovno;
    ehdstbInputFileHeader.e_csum = 0;
    ehdstbInputFileHeader.e_ovno = 0;
    write(iOutputFileHandle, &ehdstbInputFileHeader, (unsigned int)sizeof(ehdstbInputFileHeader));

    baBuffer = (char *)realloc(baBuffer, (unsigned long)(sizeof(char)*(ehdstbInputFileHeader.e_cparhdr*16-(unsigned int)sizeof(ehdstbInputFileHeader))));
    ulOutputFileSize += ulInputFileSize;
    read(iInputFileHandle, baBuffer, (ehdstbInputFileHeader.e_cparhdr*16-(unsigned int)sizeof(ehdstbInputFileHeader)));
    close(iInputFileHandle);
	write(iOutputFileHandle, baBuffer, (ehdstbInputFileHeader.e_cparhdr*16-(unsigned int)sizeof(ehdstbInputFileHeader)));

    ++strSourceFilename[7];

    for(uInputFileIndex = 1; uInputFileIndex <= ulInputFileParts; uInputFileIndex++) {
        if(uInputFileIndex < ulInputFileParts) {
            iInputFileHandle = open(strSourceFilename, O_RDONLY);
            ulInputFileSize = (unsigned long)getSize(iInputFileHandle);
            baBuffer = (char *)realloc(baBuffer, (unsigned long)(sizeof(char)*ulInputFileSize));
            ulOutputFileSize += ulInputFileSize;

            if(ulInputFileSize == 32768) {
                read(iInputFileHandle, baBuffer, ulInputFileSize);
                close(iInputFileHandle);
                write(iOutputFileHandle, baBuffer, ulInputFileSize);
            }
            else {
                exit(1);
            }
        }
        else {
            iInputFileHandle = open(strSourceFilename, O_RDONLY);
            ulInputFileSize = (unsigned long)getSize(iInputFileHandle);
            baBuffer = (char *)realloc(baBuffer, (unsigned long)(sizeof(char)*ulInputFileSize));
            ulOutputFileSize += ulInputFileSize;

            if(ulInputFileSize == ulInputLastPartSize) {
                read(iInputFileHandle, baBuffer, ulInputFileSize);
                close(iInputFileHandle);
                write(iOutputFileHandle, baBuffer, ulInputFileSize);
            }
            else {
                exit(1);
            }
        }

        ++strSourceFilename[7];
	}

    close(iOutputFileHandle);
}

void WriteHeader(int iInputFileHandle, char *strTargetFilename) {
	exehead_dos_sixteenbit ehdstbInputFileHeader;
	unsigned long ulOutputFileParts;

	int iOutputFileHandle = create(strTargetFilename);
	unsigned long ulInputFileSize = getSize(iInputFileHandle);
	
	read(iInputFileHandle, &ehdstbInputFileHeader, sizeof(ehdstbInputFileHeader));

	ulOutputFileParts = ulInputFileSize;
	ulOutputFileParts -= (unsigned long)(ehdstbInputFileHeader.e_cparhdr*16);
	ulOutputFileParts = 1+ulOutputFileParts/32768+!!(ulOutputFileParts%32768);
	ulOutputFileParts--;
	ehdstbInputFileHeader.e_csum = (unsigned short)(((unsigned long)((unsigned long)ulInputFileSize-((unsigned long)ehdstbInputFileHeader.e_cparhdr*16)))-((ulOutputFileParts-1)*32768));
	ehdstbInputFileHeader.e_ovno = (unsigned short)ulOutputFileParts;
    
	write(iOutputFileHandle, &ehdstbInputFileHeader, sizeof(ehdstbInputFileHeader));
    baBuffer = (char *)realloc(baBuffer, (unsigned long)(sizeof(char)*ulInputFileSize));
	read(iInputFileHandle, baBuffer, ehdstbInputFileHeader.e_cparhdr*16-sizeof(ehdstbInputFileHeader));
	write(iOutputFileHandle, baBuffer, ehdstbInputFileHeader.e_cparhdr*16-sizeof(ehdstbInputFileHeader));
	close(iOutputFileHandle);
	++strTargetFilename[7];
}

void WriteData(int iInputFileHandle, char *strTargetFilename) {
	unsigned short Bytes;
	int iOutputFileHandle;

    baBuffer = (char *)realloc(baBuffer, (unsigned long)(sizeof(char)*32768));
	while((Bytes = read(iInputFileHandle, baBuffer, 32768)) != 0) {
		iOutputFileHandle = create(strTargetFilename);
		write(iOutputFileHandle, baBuffer, Bytes);
		close(iOutputFileHandle);
		++strTargetFilename[7];
	}
}