#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class XNB
{
public:
	XNB();
	~XNB();

	string openRead(string);
	string openWrite(string);

	string error;
	unsigned int readerType;
	string filename;
	unsigned int filesize; // size of the whole file
	vector< pair<string, unsigned int> > readers; // <reader name, version number>
	bool writeMode = false;

	unsigned char readByte();
	unsigned int readShort();
	unsigned int readInt();
	unsigned int read7BitEncodedInt();
	string readString(unsigned int);
	string readString();
	char* readBytes(unsigned int);

	void writeByte(unsigned char);
	void writeShort(unsigned short);
	void writeInt(unsigned int);
	void write7BitEncodedInt(unsigned int);
	void writeString(string);
	void writeEncodedString(string); // same as writeString(), but with a 7bitencodedint for the size
	void writeBytes(char*, unsigned int);

	void setPointer(int);
	unsigned int getPointer();
	void movePointer(int);
private:
	fstream file;
};

