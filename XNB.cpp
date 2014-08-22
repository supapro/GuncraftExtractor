#include "XNB.h"

XNB::XNB(){}

XNB::~XNB()
{
	if (writeMode) {
		file.seekp(0, file.end);
		unsigned int length = file.tellp();
		file.seekp(6);
		writeInt(length);
	}
	file.close();
}

string XNB::openRead(string fileName)
{
	filename = fileName;
	file.open(filename, ios::in | ios::binary);
	if (!file.good())
		return "Can't open file";

	string strXNB = readString(3);
	if (strXNB.compare("XNB") != 0)
		return "File is not a XNB file";

	if (readByte() != 'w')
		return "Only XNB files for windows are supported";

	if (readByte() != 5)
		return "Only XNB files from XNA gamestudio 4.0 are supported";

	unsigned char flags = readByte();
	if ((flags & 0x01) != 0x01)
		return "Only HiDef profile is supported";

	if (flags & 0x80)
		return "Only non-compressed data is supported";

	filesize = readInt();

	unsigned int readerCount = readByte();

	for (unsigned int i = 0; i < readerCount; i++) {
		string reader = readString();
		unsigned int readerVersion = readInt();
		readers.push_back(make_pair(reader, readerVersion));
	}

	readerType = 0;

	if (readerCount == 1) {
		string reader = readers[0].first;

		if (reader.compare(0, 47, "Microsoft.Xna.Framework.Content.Texture2DReader") == 0)
			readerType = 1;
		else if (reader.compare(0, 49, "Microsoft.Xna.Framework.Content.SoundEffectReader") == 0)
			readerType = 2;
	}

	read7BitEncodedInt();
	movePointer(1);

	return "";
}

string XNB::openWrite(string fileName)
{
	filename = fileName;
	if (readers.empty())
		return "Readers not set";

	file.open(filename, ios::out | ios::binary);
	if (!file.good())
		return "Can't create file";

	writeMode = true;

	writeString("XNBw");
	writeByte(5);
	writeByte(0x01);
	writeInt(0);

	unsigned int readerCount = readers.size();
	write7BitEncodedInt(readerCount);

	for (unsigned int i = 0; i < readerCount; i++) {
		writeEncodedString(readers[i].first);
		writeInt(readers[i].second);
	}

	write7BitEncodedInt(0);
	writeByte(1);

	return "";
}


/*********** Read methods *********/
unsigned char XNB::readByte()
{
	unsigned char byte = file.get();
	return byte;
}

unsigned int XNB::readShort()
{
	unsigned char n1, n2;
	n1 = file.get();
	n2 = file.get();
	return (n1 | (n2 << 8));
}

unsigned int XNB::readInt()
{
	unsigned char n1, n2, n3, n4;
	n1 = file.get();
	n2 = file.get();
	n3 = file.get();
	n4 = file.get();
	return (n1 | (n2 << 8) | (n3 << 16) | (n4 << 24));
}

unsigned int XNB::read7BitEncodedInt()
{
	unsigned int result = 0;
	char bitsRead = 0;
	unsigned char value;
	do{
		value = file.get();
		result |= (value & 0x7f) << bitsRead;
		bitsRead += 7;
	} while ((value & 0x80) == 0x80);
	return result;
}

char* XNB::readBytes(unsigned int length)
{
	char* data = new char[length];
	file.read(data, length);
	return data;
}

string XNB::readString(unsigned int length)
{
	string str;
	str.resize(length);
	file.read(&str[0], str.size());
	return str;
}

string XNB::readString()
{
	return readString(read7BitEncodedInt());
}

/*********** Write methods *********/
void XNB::writeByte(unsigned char byte)
{
	file.put(byte);
}

void XNB::writeShort(unsigned short int16)
{
	file.put(int16 & 0xff);
	file.put(int16 >> 8);
}

void XNB::writeInt(unsigned int int32)
{
	file.put(int32 & 0xff);
	file.put(int32 >> 8);
	file.put(int32 >> 16);
	file.put(int32 >> 24);
}

void XNB::writeString(string str)
{
	file << str;
}

void XNB::writeEncodedString(string str)
{
	write7BitEncodedInt(str.length());
	file << str;
}

void XNB::writeBytes(char* bytes, unsigned int length)
{
	file.write(bytes, length);
}

void XNB::write7BitEncodedInt(unsigned int number)
{
	unsigned char value;
	unsigned char bitsWriten = 0;
	do {
		bitsWriten += 7;
		value = number & 0x7f;
		number = number >> bitsWriten;
		if (number > 0)
			value |= 0x80;
		file.put(value);
	} while (number > 0);
}


/*********** File pointer methods *********/
void XNB::setPointer(int pos)
{
	if (writeMode) {
		file.seekp(pos);
	}
	else {
		file.seekg(pos);
	}
}

void XNB::movePointer(int amount)
{
	if (writeMode) {
		file.seekp((int)file.tellp() + amount);
	}
	else {
		file.seekg((int)file.tellg() + amount);
	}
}

unsigned int XNB::getPointer()
{
	unsigned int pos = file.tellg();
	return pos;
}
