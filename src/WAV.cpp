#include "../include/WAV.h"

WAV::WAV(){}

WAV::~WAV()
{
	delete[] waveData;
	file.close();
}

std::string WAV::openRead(const std::string &fileName)
{
	filename = fileName;

	file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return "Can't open file";

	if (file.tellg() < 45)
		return "Not a valid wav file";
	file.seekg(0);

	if (readString(4).compare("RIFF"))
		return "Not a valid wav file.";

	dataChunkSize = readInt() - 36;

	if (readString(4).compare("WAVE"))
		return "Not a valid wav file.";

	if (readString(4).compare("fmt "))
		return "Not a valid wav file.";

	fmtChunkSize = readInt();
	if (fmtChunkSize < 16)
		return "Not a valid wav file.";

	format = readShort();
	if (format != 1)
		return "Format " + std::to_string(format) + " not supported.";

	channels = readShort();
	samplesPerSec = readInt();
	avgBytesPerSec = readInt();
	blockAlign = readShort();
	bitsPerSample = readShort();

	if (avgBytesPerSec != (samplesPerSec * channels * (bitsPerSample / 8)))
		return "Average bytes per second is incorrect.";

	if (blockAlign != (channels * (bitsPerSample / 8)))
		return "Block align is incorrect.";

	if (fmtChunkSize != 16)
		file.seekg((int)file.tellg() + (int)(fmtChunkSize - 16));

	if (readString(4).compare("data"))
		return "Not a valid wav file.";

	if (readInt() != dataChunkSize)
		return "Data chunk size is not correct.";

	waveData = new char[dataChunkSize];
	file.read(waveData, dataChunkSize);

	return "";
}

std::string WAV::openWrite(const std::string &fileName)
{
	filename = fileName;

	if (format == 0)
		return "Format not set";

	if (channels == 0)
		return "Channels not set";

	if (samplesPerSec == 0)
		return "Samples per second not set";

	if (avgBytesPerSec == 0)
		return "Average bytes per second not set";

	if (blockAlign == 0)
		return "Block align not set";

	if (bitsPerSample == 0)
		return "Bits per sample not set";

	if (fmtChunkSize < 16)
		return "Fmt chunk size not set correctly";

	if (dataChunkSize == 0)
		return "Data chunk size not set correctly";

	if (waveData == NULL)
		return "Wave data not set";
	
	file.open(filename, std::ios::out | std::ios::binary);
	if (!file.good())
		return "Can't create file";

	writeString("RIFF");
	writeInt(dataChunkSize + 36);
	writeString("WAVEfmt ");
	writeInt(fmtChunkSize);
	writeShort(1);
	writeShort(channels);
	writeInt(samplesPerSec);
	writeInt(avgBytesPerSec);
	writeShort(blockAlign);
	writeShort(bitsPerSample);
	writeString("data");
	writeInt(dataChunkSize);
	file.write(waveData, dataChunkSize);

	return "";
}


//*************** Read methods ***************
unsigned char WAV::readByte()
{
	unsigned char byte = file.get();
	return byte;
}

unsigned int WAV::readShort()
{
	unsigned char n1, n2;
	n1 = file.get();
	n2 = file.get();
	return (n1 | (n2 << 8));
}

unsigned int WAV::readInt()
{
	unsigned char n1, n2, n3, n4;
	n1 = file.get();
	n2 = file.get();
	n3 = file.get();
	n4 = file.get();
	return (n1 | (n2 << 8) | (n3 << 16) | (n4 << 24));
}

char* WAV::readBytes(unsigned int length)
{
	char* data = new char[length];
	file.read(data, length);
	return data;
}

std::string WAV::readString(unsigned int length)
{
	std::string str;
	str.resize(length);
	file.read(&str[0], str.size());
	return str;
}


//*************** Write methods ***************
void WAV::writeByte(unsigned char byte)
{
	file.put(byte);
}

void WAV::writeShort(unsigned short int16)
{
	file.put(int16 & 0xff);
	file.put(int16 >> 8);
}

void WAV::writeInt(unsigned int int32)
{
	file.put(int32 & 0xff);
	file.put(int32 >> 8);
	file.put(int32 >> 16);
	file.put(int32 >> 24);
}

void WAV::writeString(const std::string &str)
{
	file << str;
}
