#include "GunCraftExtractor.h"

#if defined(_WIN32)
#include <windows.h>
#endif

using namespace std;

void setConsoleTitle(const char *title)
{
#if defined(_WIN32)
	SetConsoleTitleA(title);
#else
	// set title terminal command.
<<<<<<< HEAD
	printf("%c[0;%s%c", '\033', title, '\007');
=======
	printf("%c[0;%s%c", '/033', title, '/007');
>>>>>>> e46ed88057cde723a5b17d5dafc0a3e1d7e03c9e
#endif
}

int main(int argc, char *argv[]) 
{
	setConsoleTitle("GunCraft Extractor - v1.1");

	if (argc < 2) {
		cout << "You need to start this program by dropping a file(s) on the executable." << endl;
		cin.ignore();
		exit(0);
	}

	string fileName;
	string ext;
	for (int i = 1; i < argc; ++i) {
		fileName = argv[i];
		ext = fileName.substr(fileName.length() - 4, 4);

		if (ext.compare(".xnb") == 0) {
			cout << "Converting xnb file " << argv[i] << endl;
			convertXnb(fileName);
		} 
		else if (ext.compare(".png") == 0) {
			cout << "Converting png file " << argv[i] << " to xnb" << endl;
			PngToXnb(fileName);
		}
		else if (ext.compare(".wav") == 0) {
			cout << "Converting wav file " << argv[i] << " to xnb" << endl;
			WavToXnb(fileName);
		}
		else {
			cout << "Unsupported file " << fileName << endl;
		}

		cout << endl;
	}

	cin.ignore();
	
	return 0;
}

void convertXnb(string fileName)
{
	XNB *xnb = new XNB();
	string error = xnb->openRead(fileName);
	if (!error.empty()) {
		cout << error << endl;
		return;
	}

	if (xnb->readerType == 1) {
		cout << "It's a texture file, converting to png" << endl;
		XnbToPng(xnb);
	}
	else if (xnb->readerType == 2) {
		cout << "It's a sound effect file, converting to wav" << endl;
		XnbToWav(xnb);
	}
	else {
		cout << "This xnb file type isn't supported" << endl;
	}

	delete xnb;
}

void XnbToPng(XNB *xnb)
{

	unsigned int format = xnb->readInt();
	if (format != 0) {
		cout << "Unsupported surface format " << to_string(format) << endl;
		return;
	}

	unsigned int width = xnb->readInt();
	unsigned int height = xnb->readInt();

	cout << "Width: " << width << "; Height: " << height << endl;

	unsigned int mipCount = xnb->readInt();

	//cout << "Mip count: " << to_string(mipCount) << endl;

	unsigned int size = xnb->readInt();
	if (size != width * height * 4) {
		cout << "Size " << size << " isn't correct, it should be " << width * height * 4 << endl;
		return;
	}

	string filename = xnb->filename;
	string filenamePng = filename.substr(0, filename.length() - 4) + ".png";

	char* imageData = xnb->readBytes(size);
	vector<unsigned char> image(imageData, imageData + size);

	unsigned error = lodepng::encode(filenamePng, image, width, height);

	cout << "Successfull converted the xnb texture file " << filename << " to png file " << filenamePng << endl;
}

void XnbToWav(XNB *xnb)
{
	if (xnb->readInt() != 18) {
		cout << "Wrong format chunk size" << endl;
		return;
	}

	if (xnb->readShort() != 1) {
		cout << "Wrong wav codec" << endl;
		return;
	}

	unsigned short channels = xnb->readShort();
	unsigned int samplesPerSec = xnb->readInt();
	unsigned int avgBytesPerSec = xnb->readInt();
	unsigned short blockAlign = xnb->readShort();
	unsigned short bitsPerSample = xnb->readShort();

	if (avgBytesPerSec != (samplesPerSec * channels * (bitsPerSample / 8))) {
		cout << "Average bytes per second is incorrect" << endl;
		return;
	}

	if (blockAlign != (channels * (bitsPerSample / 8))) {
		cout << "Block align is incorrect" << endl;
		return;
	}

	xnb->movePointer(2);

	unsigned int dataSize = xnb->readInt();
	char* waveData = xnb->readBytes(dataSize);

	string filename = xnb->filename;
	string filenameWav = filename.substr(0, filename.length() - 4) + ".wav";
	
	WAV* wav = new WAV();

	wav->format = 18;
	wav->channels = channels;
	wav->samplesPerSec = samplesPerSec;
	wav->avgBytesPerSec = avgBytesPerSec;
	wav->blockAlign = blockAlign;
	wav->bitsPerSample = bitsPerSample;
	wav->fmtChunkSize = 16;
	wav->dataChunkSize = dataSize;
	wav->waveData = waveData;

	wav->openWrite(filenameWav);

	delete wav;

	cout << "Successfull converted the xnb soundeffect file " << filename << " to wav file" << filenameWav << endl;
}

void PngToXnb(string filename)
{
	string filenameXnb = filename.substr(0, filename.length() - 4) + ".xnb";
	XNB *xnb = new XNB();
	xnb->readers.push_back(make_pair("Microsoft.Xna.Framework.Content.Texture2DReader, Microsoft.Xna.Framework.Graphics, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553", 0));
	string errorXnb = xnb->openWrite(filenameXnb);
	if (!errorXnb.empty()) {
		cout << errorXnb << endl;
		delete xnb;
		return;
	}

	vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, filename);
	if (error == 1) {
		cout << "Error opening png file" << endl;
		return;
	}

	xnb->writeInt(0); // Surface format = 0 (color)
	xnb->writeInt(width);
	xnb->writeInt(height);

	unsigned int size = width * height * 4;

	unsigned int mipCount = 12;
	if (size < 4194304) { // mipmapping breaks non block textures
		mipCount = 1;
	}
	else if (size == 4194304) {
		mipCount = 11;
	}
	xnb->writeInt(mipCount);

	unsigned char* imageBuffer = new unsigned char[size];
	unsigned char* shrinkedImage = new unsigned char[size];

	copy(image.begin(), image.end(), imageBuffer);

	for (unsigned int i = 0; i < mipCount; i++) {
		xnb->writeInt(size);
		xnb->writeBytes(reinterpret_cast<char*>(imageBuffer), size);

		if (i == mipCount - 1) // don't calculate a mipmap for the last one
			break;

		unsigned int newWidth = max(ceil((double)width / 2.0), 1);
		unsigned int newHeight = max(ceil((double)height / 2.0), 1);

		size = newWidth * newHeight * 4;

		unsigned int x2, y2, index, color1, color2, color3, color4;

		for (unsigned int y = 0; y < newHeight; y++) {
			y2 = y * 2;
			for (unsigned int x = 0; x < newWidth; x++) {
				x2 = x * 2;

				color1 = (y2*width*4) + (x2*4);
				color2 = (y2*width * 4) + ((x2+1) * 4);
				color3 = ((y2+1)*width * 4) + (x2 * 4);
				color4 = ((y2+1)*width * 4) + ((x2+1) * 4);

				index = (y*newWidth * 4) + (x * 4);

				shrinkedImage[index] = (imageBuffer[color1] + imageBuffer[color2] + imageBuffer[color3] + imageBuffer[color4]) / 4;
				shrinkedImage[index + 1] = (imageBuffer[color1 + 1] + imageBuffer[color2 + 1] + imageBuffer[color3 + 1] + imageBuffer[color4 + 1]) / 4;
				shrinkedImage[index + 2] = (imageBuffer[color1 + 2] + imageBuffer[color2 + 2] + imageBuffer[color3 + 2] + imageBuffer[color4 + 2]) / 4;
				shrinkedImage[index + 3] = (imageBuffer[color1 + 3] + imageBuffer[color2 + 3] + imageBuffer[color3 + 3] + imageBuffer[color4 + 3]) / 4;
			}
		}

		width = newWidth;
		height = newHeight;

		swap(imageBuffer, shrinkedImage);
	}

	delete xnb;

	cout << "Successfull converted the png file " << filename << " to xnb texture file" <<  filenameXnb << endl;
}

void WavToXnb(string filename)
{
	WAV* wav = new WAV();
	string error = wav->openRead(filename);
	if (!error.empty()) {
		cout << error << endl;
		delete wav;
		return;
	}

	string filenameXnb = filename.substr(0, filename.length() - 4) + ".xnb";
	XNB* xnb = new XNB();
	xnb->readers.push_back(make_pair("Microsoft.Xna.Framework.Content.SoundEffectReader", 0));
	error = xnb->openWrite(filenameXnb);
	if (!error.empty()) {
		cout << error << endl;
		delete wav;
		delete xnb;
		return;
	}

	xnb->writeInt(18);
	xnb->writeShort(1);
	xnb->writeShort(wav->channels);
	xnb->writeInt(wav->samplesPerSec);
	xnb->writeInt(wav->avgBytesPerSec);
	xnb->writeShort(wav->blockAlign);
	xnb->writeShort(wav->bitsPerSample);
	xnb->writeShort(0);
	
	xnb->writeInt(wav->dataChunkSize);
	xnb->writeBytes(wav->waveData, wav->dataChunkSize);

	xnb->writeInt(0);
	xnb->writeInt(wav->dataChunkSize/4);
	unsigned int duration = ((float)wav->dataChunkSize / (float)wav->avgBytesPerSec)*1000.0f;
	xnb->writeInt(duration);

	delete wav;
	delete xnb;

	cout << "Successfull converted the wav file " << filename << " to xnb soundeffect file" << filenameXnb << endl;
}
