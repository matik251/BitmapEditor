#include <stdio.h>
#include "Model.h"
#include "View.h"
#include <malloc.h>


int main() {

	fileHead fileHeader = { NULL };
	infoHead infoHeader = { NULL };
	FILE* MYFILE = NULL;
	int choice;
	
	do {
		CLS();
		showMenu();
		scanf("%d", &choice);
		switch (choice)
		{
		case 1: loadImageUserInput(&fileHeader, &infoHeader, MYFILE); break;
		case 2: saveTemporaryImage(&fileHeader, &infoHeader, MYFILE); showTempBMP(); break;
		case 3: addFilterMenu(&fileHeader, &infoHeader); break;
		case 4: addRGBTransformMenu(&fileHeader, &infoHeader); break;
		case 5: rotateHorizontally(&fileHeader, &infoHeader);  break;
		case 6: rotateVertically(&fileHeader, &infoHeader);  break;
		case 7: crop(&fileHeader, &infoHeader);  break;
		case 8: insertBMP2BMP(&fileHeader, &infoHeader);  break;
		case 9: saveImage(&fileHeader, &infoHeader, MYFILE);  break;
		case 10: saveImageUserInput(&fileHeader, &infoHeader, MYFILE);  break;
		}
		PAUSE();
	} while (choice != 11);
	
	avoidMemoryLeaks(&infoHeader);

	return 0;
}