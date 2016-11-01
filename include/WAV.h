#pragma once
#include <iostream>
#include <string>
#include <fstream>

class WAV
{
public:
	WAV();
	~WAV();

	std::string filename;
	bool writeMode = false;

	std::string openRead(std::string);
	std::string openWrite(std::string);

	unsigned char readByte();
	unsigned int readShort();
	unsigned int readInt();
	std::string readString(unsigned int);
	char* readBytes(unsigned int);

	void writeByte(unsigned char);
	void writeShort(unsigned short);
	void writeInt(unsigned int);
	void writeString(std::string);

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
	std::fstream file;
};