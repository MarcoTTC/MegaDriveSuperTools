#ifndef INTERNALPATCHSYSTEM_HPP
#define INTERNALPATCHSYSTEM_HPP

class InternalPatchSystem
{
public:
	InternalPatchSystem();
	~InternalPatchSystem();
	
	static void createIPS(char *patch, char *originalFile, char *modifiedFile);
	static void applyIPS(char *patch, char *file);
	
private:
	static int ipsHeaderCheck(char* string);
	static int ipsEofCheck(char *string);
	static unsigned int convertToLittleEndianOffsetValue(char* ByteArray);
	static unsigned short convertToLittleEndianSizeValue(char* ByteArray);
};

#define NON_RLE_RECORD 0
#define RLE_RECORD 1

class IpsRecord
{
public:
	IpsRecord();
	~IpsRecord();
	
	void setType(int type);
	int isRle(void);
	
	void setOffset(unsigned int offset);
	unsigned int getOffset(void);
	
	unsigned short getSize(void);
	
	void setData(char* data, unsigned short size);
	char* getData(void);
	
	unsigned short getRleSize(void);
	
	void setRleData(unsigned short rleSize, unsigned char rleValue);
	
	char* getRleData(void);
	
private:
	int rle_record;
	
	unsigned int offset;
	unsigned short size;
	char *data;
	
	unsigned short rle_size;
	unsigned char rle_value;
};

#endif // INTERNALPATCHSYSTEM_HPP
