#pragma once
#include <stdio.h>
#include <malloc.h>

#define MAX_STRING_LENGTH 100
#define imageTypeBMP ".bmp"
#define maxPxValue 255
#define minPxValue 0
#define subPxCount 3
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef unsigned long ULONG;
typedef long LONG;
typedef unsigned short USHORT;

typedef struct tagBITMAPINFOHEADER {
	ULONG biSize;
	LONG  biWidth;
	LONG  biHeight;
	USHORT  biPlanes;
	USHORT  biBitCount;
	ULONG biCompression;
	ULONG biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	ULONG biClrUsed;
	ULONG biClrImportant;
	char ***biData;

} BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
	USHORT bfType;
	ULONG bfSize;
	USHORT bfReserved1;
	USHORT bfReserved2;
	ULONG bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPFILEHEADER fileHead;

typedef struct tagBITMAPINFOHEADER infoHead;

void loadImage(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE);

int loadImageUserInput(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE);

void saveTemporaryImage(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE);

void rotateHorizontally(fileHead * fileHeader, infoHead * infoHeader);

void rotateVertically(fileHead * fileHeader, infoHead * infoHeader);

void crop(fileHead * fileHeader, infoHead * infoHeader);

void insertBMP2BMP(fileHead * fileHeader, infoHead * infoHeader);

void avoidMemoryLeaks(infoHead * InfoHeader);

void saveImageUserInput(fileHead * fileHeader, infoHead * infoHeader, FILE * MYFILE);

void transformRGB(fileHead * fileHeader, infoHead * infoHeader, int choice);

void save(fileHead * fileHeader, infoHead * infoHeader, FILE * MYFILE);

void saveImage(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE);

void transformToBlackWhite(fileHead* fileHeader, infoHead* infoHeader);

void transformToSepia(fileHead * fileHeader, infoHead * infoHeader);

void invertColors(fileHead * fileHeader, infoHead * infoHeader);

double contrastForPixel(double pixel, double contrast);

int brightnessForPixel(int pixel, int brightness);

