#pragma once

struct vector{
    int column;
    int row;
};

void clearScreen();
void setPosition(unsigned int column, unsigned int row);
vector getTerminalSize();
void setForegroundColor(unsigned int color);
void setBackgroundColor(unsigned int color);
void clearLine();
void setLineStart();
