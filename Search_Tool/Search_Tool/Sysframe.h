#pragma once

#include"common.h"

void SetCurPos(int x, int y);
void HideCursor();
void DrawCol(int x, int y);
void DrawRow(int x, int y);

void DrawFrame(const char *title);
void DrawMenu();
void SystemEnd();
