#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <math.h>

#include "lodepng/lodepng.h"

#include "XNB.h"
#include "WAV.h"

void convertXnb(string);
void XnbToWav(XNB*);
void XnbToPng(XNB*);
void PngToXnb(string);
void WavToXnb(string);
