#include "View.h"
#include "Model.h"
#include <stdio.h>

void CLS()
{
	system("CLS");
}

void PAUSE()
{
	system("PAUSE");
}

void showMenu()
{
	printf("Menu\n1.Zaladuj bitmape\n2.Wyswietl Bitmape\n3.Dodaj filtr\n4.Zmien balans kolorow\n5.Obrot w poziomie\n6.Obrot w pionie\n7.Przytnij\n8.Wstaw 2 obraz\n9.Zapisz\n10.Zapisz jako\n11.Wyjscie\n");
	return;
}

void showFilesInDirectory()
{
	system("DIR");
	printf("\nPodaj nazwe pliku (rozszerzenie .bmp): \n");
	return;
}

void showTempBMP()
{
	system("start temp.bmp");
}

void showFilterMenu()
{
	printf("Wybor filtra\n1.Czarno-bialy\n2.Sepia\n3.Negatyw\n4.Kontrast\n5.Ustaw jasnosc\n6.Powrot\n");
	return;
}

void showRGBTransformMenu()
{
	CLS();
	printf("Wpisz:\n1 : dla czerwonego \n2 :dla zielonego \n3 : dla niebieskiego\n");
	return;
}

void addRGBTransformMenu(fileHead* fileHeader, infoHead* infoHeader)
{
	int choice = 0;
	showRGBTransformMenu();
	scanf("%d", &choice);
	transformRGB(fileHeader, infoHeader, choice);
	return;
}

void addFilterMenu(fileHead* fileHeader, infoHead* infoHeader)
{
	int choice;

	do {
		CLS();
		showFilterMenu();
		scanf("%d", &choice);
		switch (choice)
		{
		case 1: transformToBlackWhite(fileHeader, infoHeader);  break;
		case 2: transformToSepia(fileHeader, infoHeader); break;
		case 3: invertColors(fileHeader, infoHeader); break;
		case 4: addContrast(fileHeader, infoHeader, 0.1); break;
		case 5: setBrightness(fileHeader, infoHeader, 100); break;
		}
	} while (choice != 6);
}
