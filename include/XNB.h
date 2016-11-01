#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class XNB
{
public:
	XNB();
	~XNB();

	std::string openRead(std::string);
	std::string openWrite(std::string);

	std::string error;
	unsigned int readerType;
	std::string filename;
	unsigned int filesize; // size of the whole file
	std::vector< std::pair<std::string, unsigned int> > readers; // <reader name, version number>
	bool writeMode = false;

	unsigned char readByte();
	unsigned int readShort();
	unsigned int readInt();
	unsigned int read7BitEncodedInt();
	std::string readString(unsigned int);
	std::string readString();
	char* readBytes(unsigned int);

	void writeByte(unsigned char);
	void writeShort(unsigned short);
	void writeInt(unsigned int);
	void write7BitEncodedInt(unsigned int);
	void writeString(std::string);
	void writeEncodedString(std::string); // same as writeString(), but with a 7bitencodedint for the size
	void writeBytes(char*, unsigned int);

	void setPointer(int);
	unsigned int getPointer();
	void movePointer(int);
private:
	std::fstream file;
};

