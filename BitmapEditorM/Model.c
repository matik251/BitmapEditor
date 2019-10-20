#include "Model.h"
#include "View.h"
#include <stdio.h>
#include <string.h>


void loadImage(fileHead* fileHeader, infoHead* infoHeader, FILE* MYFILE)
{
	fread(&fileHeader->bfType, sizeof(USHORT), 1, MYFILE);
	fread(&fileHeader->bfSize, sizeof(ULONG), 1, MYFILE);
	fread(&fileHeader->bfReserved1, sizeof(USHORT), 1, MYFILE);
	fread(&fileHeader->bfReserved2, sizeof(USHORT), 1, MYFILE);
	fread(&fileHeader->bfOffBits, sizeof(ULONG), 1, MYFILE);
	fread(&infoHeader->biSize, sizeof(ULONG), 1, MYFILE);
	fread(&infoHeader->biWidth, sizeof(LONG), 1, MYFILE);
	fread(&infoHeader->biHeight, sizeof(LONG), 1, MYFILE);
	fread(&infoHeader->biPlanes, sizeof(USHORT), 1, MYFILE);
	fread(&infoHeader->biBitCount, sizeof(USHORT), 1, MYFILE);
	fread(&infoHeader->biCompression, sizeof(ULONG), 1, MYFILE);
	fread(&infoHeader->biSizeImage, sizeof(ULONG), 1, MYFILE);
	fread(&infoHeader->biXPelsPerMeter, sizeof(LONG), 1, MYFILE);
	fread(&infoHeader->biYPelsPerMeter, sizeof(LONG), 1, MYFILE);
	fread(&infoHeader->biClrUsed, sizeof(ULONG), 1, MYFILE);
	fread(&infoHeader->biClrImportant, sizeof(ULONG), 1, MYFILE); 
	FILE * TEMPFILE;
	if ((TEMPFILE = fopen("temp.bmp", "wb")) == NULL)
	{
		return 0;
	}
	else
	{
		int junk = infoHeader->biWidth % 4;
		int width = infoHeader->biWidth;
		if (junk != 0)
		{
			width = infoHeader->biWidth + (4 - junk);
		}

		infoHeader->biData = (char***)malloc(infoHeader->biHeight * sizeof(char**));
		for (int i = 0; i < infoHeader->biHeight; i++)
		{
			infoHeader->biData[i] = (char**)malloc(width * sizeof(char*));
			for (int j = 0; j < width; j++)
			{
				infoHeader->biData[i][j] = (char*)malloc(subPxCount * sizeof(char));
			}
		}

		for (int i = 0; i < infoHeader->biHeight; i++) {
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < subPxCount; k++)
				{
					fread(&infoHeader->biData[i][j][k], sizeof(char), 1, MYFILE);
				}
			}
		}
		MYFILE = fclose(MYFILE);
		return;
	}
}

int loadImageUserInput(fileHead* fileHeader, infoHead* infoHeader, FILE* MYFILE)
{
	char nameOfFile[MAX_STRING_LENGTH];

	CLS();
	showFilesInDirectory();
	scanf("%s", &nameOfFile);
	CLS();

	if (strstr(nameOfFile, imageTypeBMP) == NULL) 
	{
		printf("\nNiepoprawne rozszerzenie pliku\n");
		return 0;
	}
	else {
		if ((MYFILE = fopen(nameOfFile, "rb")) == NULL)
		{
			printf("\nNie odnaleziono pliku\n");
			return 0;
		}
		loadImage( fileHeader,  infoHeader, MYFILE);
		printf("\nWczytano plik\n");
		return 1;
	}
}

void transformToBlackWhite(fileHead * fileHeader, infoHead * infoHeader)
{
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int i = 0;
	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j <  width; j++)
		{
			result = (infoHeader->biData[i][j][0] + infoHeader->biData[i][j][1] + infoHeader->biData[i][j][2]) / 3;
			for (int k = 0; k < subPxCount; k++) {
				infoHeader->biData[i][j][k] = result;
			}
		}
	}
}

void transformToSepia(fileHead * fileHeader, infoHead * infoHeader)
{
	int tr = 0;
	int tg = 0;
	int tb = 0;
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int i = 0;
	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			tr = (int)( 0.393*(int)infoHeader->biData[i][j][0] + 0.769*(int)infoHeader->biData[i][j][1] + 0.189*(int)infoHeader->biData[i][j][2]);
			tg = (int)(0.349*(int)infoHeader->biData[i][j][0] + 0.686*(int)infoHeader->biData[i][j][1] + 0.168*(int)infoHeader->biData[i][j][2]);
			tb = (int)(0.272*(int)infoHeader->biData[i][j][0] + 0.534*(int)infoHeader->biData[i][j][1] + 0.131*(int)infoHeader->biData[i][j][2]);
			if (tr > maxPxValue)
			{
				tr = maxPxValue;
			}
			if (tg > maxPxValue)
			{
				tg = maxPxValue;
			}
			if (tb > maxPxValue)
			{
				tb = maxPxValue;
			}
			infoHeader->biData[i][j][0] = tr;
			infoHeader->biData[i][j][1] = tg;
			infoHeader->biData[i][j][2] = tb;
		}
	}
}

void invertColors(fileHead * fileHeader, infoHead * infoHeader)
{
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int i = 0;
	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			infoHeader->biData[i][j][0] = maxPxValue - infoHeader->biData[i][j][0];
			infoHeader->biData[i][j][1] = maxPxValue - infoHeader->biData[i][j][1];
			infoHeader->biData[i][j][2] = maxPxValue - infoHeader->biData[i][j][2];
		}
	}
}

double contrastForPixel(double pixel, double contrast)
{
	double temp = 0;
	temp = pixel / 255.0;
	temp = temp - 0.5;
	temp = temp * contrast;
	temp = temp + 0.5;
	temp = temp * 255.0;
	if (temp < minPxValue)
	{
		temp = minPxValue;
	}
	if (temp > maxPxValue)
	{
		temp = maxPxValue;
	}
	return temp;
}

void addContrast(fileHead * fileHeader, infoHead * infoHeader, double contrast)
{
	double tempRed, tempGreend, tempBlue;
	CLS();
	printf("Wpisz przeksztalcenie kontrastu (-100 do 100):");
	scanf("%d", &contrast);

	if(contrast < -100) contrast = -100;
	if (contrast > 100) contrast = 100;
	contrast = (100.0 + contrast) / 100.0;
	contrast = contrast * contrast;

	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int i = 0;
	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			infoHeader->biData[i][j][0] = contrastForPixel(infoHeader->biData[i][j][0], contrast);
			infoHeader->biData[i][j][1] = contrastForPixel(infoHeader->biData[i][j][1], contrast);
			infoHeader->biData[i][j][2] = contrastForPixel(infoHeader->biData[i][j][2], contrast);
		}
	}
}

int brightnessForPixel(int pixel, int brightness)
{
	int temp = 0;
	temp = pixel;
	//if (temp < 0)temp = 0 - temp;
	temp = temp + brightness;
	if (temp < minPxValue)
	{
		temp = 1;
	}
	if (temp > maxPxValue)
	{
		temp = maxPxValue;
	}
	return temp;
}

void setBrightness(fileHead * fileHeader, infoHead * infoHeader, int brightness)
{
	CLS();
	printf("Wpisz przeksztalcenie jasnosci (-225 do 255):");
	scanf("%d", &brightness);
	if (brightness < -maxPxValue) brightness = -maxPxValue;
	if (brightness > maxPxValue) brightness = maxPxValue;

	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int i = 0;
	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			infoHeader->biData[i][j][0] = brightnessForPixel(infoHeader->biData[i][j][0], brightness);
			infoHeader->biData[i][j][1] = brightnessForPixel(infoHeader->biData[i][j][1], brightness);
			infoHeader->biData[i][j][2] = brightnessForPixel(infoHeader->biData[i][j][2], brightness);
		}
	}
	return;
}

void transformRGB(fileHead * fileHeader, infoHead * infoHeader, int choice)
{
	enum choiceOfColor { Nothing, RED, GREEN, BLUE };

	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int i = 0;
	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			if (choice = RED)
			{
				infoHeader->biData[i][j][0];
				infoHeader->biData[i][j][1] = infoHeader->biData[i][j][1] + 255;
				infoHeader->biData[i][j][2] = infoHeader->biData[i][j][2] + 255;
			}
			if (choice = GREEN)
			{
				infoHeader->biData[i][j][0] = infoHeader->biData[i][j][0] + 255;
				infoHeader->biData[i][j][1];
				infoHeader->biData[i][j][2] = infoHeader->biData[i][j][2] + 255;
			}
			if (choice = BLUE)
			{
				infoHeader->biData[i][j][0] = infoHeader->biData[i][j][0] + 255;
				infoHeader->biData[i][j][1] = infoHeader->biData[i][j][1] + 255;
				infoHeader->biData[i][j][2];
			}
			infoHeader->biData[i][j][0] = MAX(infoHeader->biData[i][j][0], minPxValue);
			infoHeader->biData[i][j][0] = MIN(maxPxValue, infoHeader->biData[i][j][0]);
									    
			infoHeader->biData[i][j][1] = MAX(infoHeader->biData[i][j][1], minPxValue);
			infoHeader->biData[i][j][1] = MIN(maxPxValue, infoHeader->biData[i][j][1]);
									   
			infoHeader->biData[i][j][2] = MAX(infoHeader->biData[i][j][2], minPxValue);
			infoHeader->biData[i][j][2] = MIN(maxPxValue, infoHeader->biData[i][j][2]);
			//infoHeader->biData[i][j][0] = 0;
			//infoHeader->biData[i][j][2] = 0;
		}
	}
}

void save(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE)
{
	fwrite(&fileHeader->bfType, sizeof(USHORT), 1, MYFILE);
	fwrite(&fileHeader->bfSize, sizeof(ULONG), 1, MYFILE);
	fwrite(&fileHeader->bfReserved1, sizeof(USHORT), 1, MYFILE);
	fwrite(&fileHeader->bfReserved2, sizeof(USHORT), 1, MYFILE);
	fwrite(&fileHeader->bfOffBits, sizeof(ULONG), 1, MYFILE);
	fwrite(&infoHeader->biSize, sizeof(ULONG), 1, MYFILE);
	fwrite(&infoHeader->biWidth, sizeof(LONG), 1, MYFILE);
	fwrite(&infoHeader->biHeight, sizeof(LONG), 1, MYFILE);
	fwrite(&infoHeader->biPlanes, sizeof(USHORT), 1, MYFILE);
	fwrite(&infoHeader->biBitCount, sizeof(USHORT), 1, MYFILE);
	fwrite(&infoHeader->biCompression, sizeof(ULONG), 1, MYFILE);
	fwrite(&infoHeader->biSizeImage, sizeof(ULONG), 1, MYFILE);
	fwrite(&infoHeader->biXPelsPerMeter, sizeof(LONG), 1, MYFILE);
	fwrite(&infoHeader->biYPelsPerMeter, sizeof(LONG), 1, MYFILE);
	fwrite(&infoHeader->biClrUsed, sizeof(ULONG), 1, MYFILE);
	fwrite(&infoHeader->biClrImportant, sizeof(ULONG), 1, MYFILE);

	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}

	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < subPxCount; k++)
			{
				fwrite(&infoHeader->biData[i][j][k], sizeof(char), 1, MYFILE);
			}
		}
	}
	
	MYFILE = fclose(MYFILE);
	return;
}

void saveImage(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE)
{
	if ((MYFILE = fopen("save.bmp", "wb")) == NULL)
	{
		return 0;
	}
	else
	{
		save(fileHeader, infoHeader, MYFILE);
	}
	return;
}

void saveTemporaryImage(fileHead* fileHeader, infoHead* infoHeader, FILE * MYFILE)
{
	FILE * TEMPFILE;
	if ((TEMPFILE = fopen("temp.bmp", "wb")) == NULL)
	{
		return 0;
	}
	else
	{
		fwrite(&fileHeader->bfType, sizeof(USHORT), 1, TEMPFILE);
		fwrite(&fileHeader->bfSize, sizeof(ULONG), 1, TEMPFILE);
		fwrite(&fileHeader->bfReserved1, sizeof(USHORT), 1, TEMPFILE);
		fwrite(&fileHeader->bfReserved2, sizeof(USHORT), 1, TEMPFILE);
		fwrite(&fileHeader->bfOffBits, sizeof(ULONG), 1, TEMPFILE);
		fwrite(&infoHeader->biSize, sizeof(ULONG), 1, TEMPFILE);
		fwrite(&infoHeader->biWidth, sizeof(LONG), 1, TEMPFILE);
		fwrite(&infoHeader->biHeight, sizeof(LONG), 1, TEMPFILE);
		fwrite(&infoHeader->biPlanes, sizeof(USHORT), 1, TEMPFILE);
		fwrite(&infoHeader->biBitCount, sizeof(USHORT), 1, TEMPFILE);
		fwrite(&infoHeader->biCompression, sizeof(ULONG), 1, TEMPFILE);
		fwrite(&infoHeader->biSizeImage, sizeof(ULONG), 1, TEMPFILE);
		fwrite(&infoHeader->biXPelsPerMeter, sizeof(LONG), 1, TEMPFILE);
		fwrite(&infoHeader->biYPelsPerMeter, sizeof(LONG), 1, TEMPFILE);
		fwrite(&infoHeader->biClrUsed, sizeof(ULONG), 1, TEMPFILE);
		fwrite(&infoHeader->biClrImportant, sizeof(ULONG), 1, TEMPFILE);

		int junk = infoHeader->biWidth % 4;
		int width = infoHeader->biWidth;
		if (junk != 0)
		{
			width = infoHeader->biWidth + (4 - junk);
		}

		for (int i = 0; i < infoHeader->biHeight; i++) {
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < subPxCount; k++)
				{
					fwrite(&infoHeader->biData[i][j][k], sizeof(char), 1, TEMPFILE);
				}
			}
		}
	}
	TEMPFILE = fclose(TEMPFILE);
	return;
}

void rotateHorizontally(fileHead* fileHeader, infoHead* infoHeader)
{
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	infoHead tInfoHeader = { NULL };
	infoHead* tempInfoHeader = &tInfoHeader;

	tempInfoHeader->biData = (char***)malloc(infoHeader->biHeight * sizeof(char**));
	for (int i = 0; i < infoHeader->biHeight; i++)
	{
		tempInfoHeader->biData[i] = (char**)malloc(width * sizeof(char*));
		for (int j = 0; j < width; j++)
		{
			tempInfoHeader->biData[i][j] = (char*)malloc(subPxCount * sizeof(char));
		}
	}

	int i = 0;

	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			tempInfoHeader->biData[i][width - 1 - j][0] = infoHeader->biData[i][j][0];
			tempInfoHeader->biData[i][width - 1 - j][1] = infoHeader->biData[i][j][1];
			tempInfoHeader->biData[i][width - 1 - j][2] = infoHeader->biData[i][j][2];
		}
	}

	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			infoHeader->biData[i][j][0] = tempInfoHeader->biData[i][j][0];
			infoHeader->biData[i][j][1] = tempInfoHeader->biData[i][j][1];
			infoHeader->biData[i][j][2] = tempInfoHeader->biData[i][j][2];
		}
	}
	avoidMemoryLeaks(tempInfoHeader);
	return;
}

void rotateVertically(fileHead* fileHeader, infoHead* infoHeader)
{
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	infoHead tInfoHeader = { NULL };
	infoHead* tempInfoHeader = &tInfoHeader;

	tempInfoHeader->biData = (char***)malloc(infoHeader->biHeight * sizeof(char**));
	for (int i = 0; i < infoHeader->biHeight; i++)
	{
		tempInfoHeader->biData[i] = (char**)malloc(width * sizeof(char*));
		for (int j = 0; j < width; j++)
		{
			tempInfoHeader->biData[i][j] = (char*)malloc(subPxCount * sizeof(char));
		}
	}

	int i = 0;

	char result;
	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			tempInfoHeader->biData[infoHeader->biHeight - 1 - i][j][0] = infoHeader->biData[i][j][0];
			tempInfoHeader->biData[infoHeader->biHeight - 1 - i][j][1] = infoHeader->biData[i][j][1];
			tempInfoHeader->biData[infoHeader->biHeight - 1 - i][j][2] = infoHeader->biData[i][j][2];
		}
	}

	for (int i = 0; i < infoHeader->biHeight; i++) {
		for (int j = 0; j < width; j++)
		{
			infoHeader->biData[i][j][0] = tempInfoHeader->biData[i][j][0];
			infoHeader->biData[i][j][1] = tempInfoHeader->biData[i][j][1];
			infoHeader->biData[i][j][2] = tempInfoHeader->biData[i][j][2];
		}
	}
	avoidMemoryLeaks(tempInfoHeader);
	return;
}

void crop(fileHead* fileHeader, infoHead* infoHeader)
{
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}
	int bottom; int left; int cropWidth; int cropHeight;
	CLS();
	printf("Odleglosc od dolu (Pikseli): ");
	scanf("%d", &bottom);
	printf("Odleglosc od lewej (Pikseli): ");
	scanf("%d", &left);
	printf("Szerokosc (Pikseli): ");
	scanf("%d", &cropWidth);
	printf("Wyskosc (Pikseli): ");
	scanf("%d", &cropHeight);

	if (((bottom + cropHeight) < infoHeader->biHeight) && ((left + cropWidth) < width))
	{
		infoHead tInfoHeader = { NULL };
		infoHead* tempInfoHeader = &tInfoHeader;

		tempInfoHeader->biData = (char***)malloc(infoHeader->biHeight * sizeof(char**));
		for (int i = 0; i < infoHeader->biHeight; i++)
		{
			tempInfoHeader->biData[i] = (char**)malloc(width * sizeof(char*));
			for (int j = 0; j < width; j++)
			{
				tempInfoHeader->biData[i][j] = (char*)malloc(subPxCount * sizeof(char));
			}
		}

		for (int i = bottom; i < (bottom+cropHeight); i++) {
			for (int j = left; j < (left+cropWidth); j++)
			{
				tempInfoHeader->biData[i - bottom][j - left][0] = infoHeader->biData[i][j][0];
				tempInfoHeader->biData[i - bottom][j - left][1] = infoHeader->biData[i][j][1];
				tempInfoHeader->biData[i - bottom][j - left][2] = infoHeader->biData[i][j][2];
			}
		}

		infoHeader->biWidth = cropWidth;
		infoHeader->biHeight = cropHeight;
		for (int i = 0; i < cropHeight; i++) {
			for (int j = 0; j < cropWidth; j++)
			{
				infoHeader->biData[i][j][0] = tempInfoHeader->biData[i][j][0];
				infoHeader->biData[i][j][1] = tempInfoHeader->biData[i][j][1];
				infoHeader->biData[i][j][2] = tempInfoHeader->biData[i][j][2];
			}
		}
		avoidMemoryLeaks(tempInfoHeader);
	}
	else {
		printf("\nZa duzy obszar wyciecia\n");
	}
	return;
}

void insertBMP2BMP(fileHead* fileHeader, infoHead* infoHeader)
{
	int junk = infoHeader->biWidth % 4;
	int width = infoHeader->biWidth;
	if (junk != 0)
	{
		width = infoHeader->biWidth + (4 - junk);
	}

	int bottom = 0; int left = 0;


	fileHead fileHeader2BMP = { NULL };
	infoHead infoHeader2BMP = { NULL };
	infoHead* pInfoHeader2BMP = NULL;
	FILE* MYFILE2BMP = NULL;
	pInfoHeader2BMP = &infoHeader2BMP;
	if (loadImageUserInput(&fileHeader2BMP, &infoHeader2BMP, MYFILE2BMP) == 1)
	{

		printf("Odleglosc od dolu (Pikseli): ");
		scanf("%d", &bottom);
		printf("Odleglosc od lewej (Pikseli): ");
		scanf("%d", &left);

		if (((bottom + pInfoHeader2BMP->biHeight) <= infoHeader->biHeight) && ((left + pInfoHeader2BMP->biWidth) <= width))
		{
			infoHead tInfoHeader = { NULL };
			infoHead* tempInfoHeader = &tInfoHeader;

			for (int i = bottom; i < (bottom + pInfoHeader2BMP->biHeight); i++) {
				for (int j = left; j < (left + pInfoHeader2BMP->biWidth); j++)
				{
					infoHeader->biData[i][j][0] = pInfoHeader2BMP->biData[i - bottom][j - left][0];
					infoHeader->biData[i][j][1] = pInfoHeader2BMP->biData[i - bottom][j - left][1];
					infoHeader->biData[i][j][2] = pInfoHeader2BMP->biData[i - bottom][j - left][2];
				}
			}
		}
		else {
			printf("\nNowy obraz wiekszy niz podstawowy\n");
		}
	}
	avoidMemoryLeaks(pInfoHeader2BMP);
	return;
}

void avoidMemoryLeaks(infoHead* InfoHeader)
{
	int junk = InfoHeader->biWidth % 4;
	int width = InfoHeader->biWidth;
	if (junk != 0)
	{
		width = InfoHeader->biWidth + (4 - junk);
	}

	for (int i = 0; i < InfoHeader->biHeight; i++)
	{
		
		for (int j = 0; j < width; j++)
		{
			free(InfoHeader->biData[i][j]);
		}
		free(InfoHeader->biData[i]);
	}
	free(InfoHeader->biData);
}

void saveImageUserInput(fileHead* fileHeader, infoHead* infoHeader, FILE* MYFILE)
{
	char nameOfFile[MAX_STRING_LENGTH];

	CLS();
	scanf("%s", &nameOfFile);
	CLS();

	if (strstr(nameOfFile, imageTypeBMP) == NULL)
	{
		printf("\nNiepoprawne rozszerzenie pliku\n");
		return;
	}
	else {
		if ((MYFILE = fopen(nameOfFile, "wb")) == NULL)
		{
			printf("\nNie odnaleziono pliku\n");
			return;
		}
		save(fileHeader, infoHeader, MYFILE);
		printf("\Zapisano plik\n");
		return;
	}
	return;
}
