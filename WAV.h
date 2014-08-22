#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class WAV
{
public:
	WAV();
	~WAV();

	string filename;
	bool writeMode = false;

	string openRead(string);
	string openWrite(string);

	unsigned char readByte();
	unsigned int readShort();
	unsigned int readInt();
	string readString(unsigned int);
	char* readBytes(unsigned int);

	void writeByte(unsigned char);
	void writeShort(unsigned short);
	void writeInt(unsigned int);
	void writeString(string);

	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int avgBytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	unsigned int fmtChunkSize;

	unsigned int dataChunkSize;
	char* waveData;
private:
	fstream file;
};