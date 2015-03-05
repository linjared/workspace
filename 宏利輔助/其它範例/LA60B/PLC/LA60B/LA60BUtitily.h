//---------------------------------------------------------------------------

#ifndef LA60BUtitilyH
#define LA60BUtitilyH
//---------------------------------------------------------------------------

char IntToBCD(int integer);
int BCDToInt(char value);
unsigned short TransBufferBCDData(char *Data,int GetPos,int Length);		// unsigned short = WORD
unsigned char LRCGeneration(unsigned char *Data,int DataLen);
int InsData(unsigned char *Data,int InsPos,unsigned short Value,int Length);
int InsLA60BAddrData(unsigned char *Data,int InsPos,unsigned short Value,int BCDs);

//---------------------------------------------------------------------------
#endif
