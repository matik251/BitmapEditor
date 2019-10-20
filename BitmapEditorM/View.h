#pragma once
#include "Model.h"
#include <stdio.h>

void CLS();

void PAUSE();

void showMenu();

void showFilesInDirectory();

void showTempBMP();

void showFilterMenu();

void showRGBTransformMenu();

void addRGBTransformMenu(fileHead * fileHeader, infoHead * infoHeader);

void addFilterMenu(fileHead * fileHeader, infoHead * infoHeader);
