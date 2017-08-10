#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <list>

using namespace std;

#include "ips.hpp"

InternalPatchSystem::InternalPatchSystem()
{
}

InternalPatchSystem::~InternalPatchSystem()
{
}

void InternalPatchSystem::createIPS(char *patch, char *originalFile, char*modifiedFile)
{
	// TODO
}

void InternalPatchSystem::applyIPS(char *patch, char *file)
{
	ifstream inputPatch;
	fstream outputFile;
	
	cout << "Applying patch to file..." << endl;
	
	inputPatch.open(patch, ios::in | ios::binary);
	if(!inputPatch.is_open()) {
		cout << "Failed to open patch file!" << endl;
		cout << "Check if the file has read permission" << endl;
		return;
	}
	
	char headerValue[5];
	inputPatch.read(headerValue, 5);
	
	if(!ipsHeaderCheck(headerValue)) {
		cout << "Invalid patch file!" << endl;
		return;
	}
	
	list<IpsRecord> recordsList;
	
	char nextValue[3];
	unsigned int offset;
	unsigned short size;
	char* data;
	unsigned short rleSize;
	unsigned char rleValue;
	while(!inputPatch.eof()) {
		inputPatch.read(nextValue, 3);
		if(!ipsEofCheck(nextValue)) {
			offset = convertToLittleEndianOffsetValue(nextValue);
			inputPatch.read(nextValue, 2);
			size = convertToLittleEndianSizeValue(nextValue);
			if(size==0) {
				inputPatch.read(nextValue, 2);
				rleSize = convertToLittleEndianSizeValue(nextValue);
				
				inputPatch.read(nextValue, 1);
				rleValue = nextValue[0];
				
				IpsRecord rle_record;
				rle_record.setType(RLE_RECORD);
				rle_record.setOffset(offset);
				rle_record.setRleData(rleSize, rleValue);
				
				recordsList.push_back(rle_record);
			} else {
				data = (char*) calloc(size, sizeof(char));
				inputPatch.read(data, size);
				
				IpsRecord record;
				record.setType(NON_RLE_RECORD);
				record.setOffset(offset);
				record.setData(data, size);
				
				recordsList.push_back(record);
			}
		} else {
			inputPatch.read(headerValue, 5);
		}
	}
	
	inputPatch.close();
	
	outputFile.open(file, ios::out | ios::in | ios::binary | ios::ate);
	if(!outputFile.is_open()) {
		cout << "Failed to open output file!" << endl;
		cout << "Check if the file has write permission" << endl;
		return;
	}
	
	unsigned long output_filesize = outputFile.tellg();
	if(output_filesize>0xFFFFFF) {
		cout << "Output file too big for IPS format!" << endl;
		return;
	}
	outputFile.seekp(0, ios_base::beg);
	
	list<IpsRecord>::iterator it = recordsList.begin();
	while(it != recordsList.end()) {
		unsigned int offset = 0;
		unsigned short size = 0;
		char* data = NULL;
		
		int status = it->isRle();
		if(status == NON_RLE_RECORD) {
			offset = it->getOffset();
			size = it->getSize();
			data = it->getData();
		} else {
			offset = it->getOffset();
			size = it->getRleSize();
			data = (char*) it->getRleData();
		}
		
		outputFile.seekp(offset, ios_base::beg);
		if(outputFile.fail()) {
			cout << "Stream failed for seek operation!" << endl;
		}
		
		outputFile.write(data, size);
		if(outputFile.fail()) {
			cout << "Stream failed for write operation!" << endl;
		}
		
		if(status && data!=NULL) {
			free(data);
		}
		
		++it;
	}
	
	outputFile.close();
	
	cout << "Patch applied successfully!" << endl;
}

int InternalPatchSystem::ipsHeaderCheck(char *string)
{
	if(string[0]=='P' && string[1]=='A' && string[2]=='T' &&
	   string[3]=='C' && string[4]=='H') {
		return 1;
	} else {
		return 0;
	}
}

int InternalPatchSystem::ipsEofCheck(char *string)
{
	if(string[0]=='E' && string[1]=='O' && string[2]=='F') {
		return 1;
	} else {
		return 0;
	}
}

unsigned int InternalPatchSystem::convertToLittleEndianOffsetValue(char *ByteArray)
{
	unsigned int littleEndianOffset = 0;
	
	littleEndianOffset = (ByteArray[0]<<16)&0x00FF0000;
	littleEndianOffset |= (ByteArray[1]<<8)&0x0000FF00;
	littleEndianOffset |= (ByteArray[2])&0x000000FF;
	
	return littleEndianOffset;
}

unsigned short InternalPatchSystem::convertToLittleEndianSizeValue(char *ByteArray)
{
	unsigned short littleEndianValue = 0;
	
	littleEndianValue = (ByteArray[0]<<8)&0xFF00;
	littleEndianValue |= (ByteArray[1])&0x00FF;
	
	return littleEndianValue;
}

IpsRecord::IpsRecord()
{
	rle_record = NON_RLE_RECORD;
	offset = 0;
	size = 0;
	data = NULL;
	rle_size = 0;
	rle_value = '\0';
}

IpsRecord::~IpsRecord()
{
	
}

void IpsRecord::setType(int type)
{
	rle_record = type;
}

int IpsRecord::isRle(void)
{
	return rle_record;
}

void IpsRecord::setOffset(unsigned int offset)
{
	this->offset = offset;
}

unsigned int IpsRecord::getOffset()
{
	return offset;
}

unsigned short IpsRecord::getSize()
{
	return size;
}

void IpsRecord::setData(char* data, unsigned short size)
{
	this->data = (char*) calloc(size, sizeof(char));
	memcpy(this->data, data, size);
	
	this->size = size;
}

char* IpsRecord::getData()
{
	return this->data;
}

unsigned short IpsRecord::getRleSize()
{
	return rle_size;
}

void IpsRecord::setRleData(unsigned short rleSize, unsigned char rleValue)
{
	this->size = 0;
	this->rle_size = rleSize;
	this->rle_value = rleValue;
}

char* IpsRecord::getRleData()
{
	if(rle_record == RLE_RECORD) {
		char* rleData = (char*) calloc(rle_size, sizeof(char));
		memset(rleData, (int)rle_value, rle_size);
		
		return rleData;
	} else {
		return NULL;
	}
}


