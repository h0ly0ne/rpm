/*
 * Borrowed from Extended Operating System Loader
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found at http://www.gnu.org
 */

#include <io.h>
#include <exestruc.h>

void WriteHeader(int ifh);
void WriteData(int ifh);

static const char *SrcFile = "part.exe";
static char *DestFile = "xrpart00.xxf";

static char Buffer[32768];

void main()
{
	int ifh;
	TExeHeader ExeHeader;

	ifh = open(SrcFile,O_RDWR);
	WriteHeader(ifh);
	WriteData(ifh);
	close(ifh);

}

void WriteHeader(int ifh)
{
	TExeHeader ExeHeader;
	int ofh;
	unsigned short Count;
	unsigned long Chunks = getSize(ifh);


	ofh = creat(DestFile);
	read(ifh,&ExeHeader,sizeof (ExeHeader));
	Count = ExeHeader.HeaderSize * 16 - sizeof (TExeHeader);

	/** Add missing header (number of segments) */
	Chunks -= Count;
	Chunks = 1 + Chunks / 32768 + !!(Chunks % 32768);
	Chunks--;
	write(ofh,&Chunks,sizeof(unsigned short));

	write(ofh,&ExeHeader,sizeof (ExeHeader));
	read(ifh,Buffer,Count);
	write(ofh,Buffer,Count);
	close(ofh);
	++DestFile[7];
}

void WriteData(int ifh)
{
	unsigned short Bytes;
	int ofh;

	while ((Bytes = read(ifh,Buffer,32768)) != 0) {
		ofh = creat(DestFile);
		write(ofh,Buffer,Bytes);
		close(ofh);
		++DestFile[7];
	}
}