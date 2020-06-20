#include <orcus.h>
#include <stdio.h>
#include <stdarg.h>
#include "gp2xtypes.h"

#define CHARS 95
#define MAGENTA 0xF81F

uint16_t* _font;
int _charWidth;
int _charHeight;

void rgbSetFont(uint16_t* font, int charWidth, int charHeight) {
  _font = font;
  _charWidth = charWidth;
  _charHeight = charHeight;
}

void rgbPutc(uint16_t* fb, int x, int y, uint16_t colour, char c) {
  if(c < ' ' || c > '~') {
    return;
  }
  
  for(int j = 0 ; j < _charHeight ; j++) {
    for(int i = 0 ; i < _charWidth ; i++) {
      uint16_t cPx = _font[j*(CHARS*_charWidth)+i+((c-' ')*_charWidth)];
      if(cPx != MAGENTA && (x+i) < 320) {
	fb[x+i+((y+j)*320)] = colour;
      }
    }
  }
}

void rgbPutcBg(uint16_t* fb, int x, int y, uint16_t colour, uint16_t bgColour, char c) {
  if(c < ' ' || c > '~') {
    return;
  }
  
  for(int j = 0 ; j < _charHeight ; j++) {
    for(int i = 0 ; i < _charWidth ; i++) {
      uint16_t cPx = _font[j*(CHARS*_charWidth)+i+((c-' ')*_charWidth)];
      if((x+i) < 320) {
	fb[x+i+((y+j)*320)] = cPx != MAGENTA ? colour : bgColour;
      }
    }
  }
}

static void _rgbPrintf(uint16_t* fb, int x, int y, uint16_t colour, bool applyBg, uint16_t bgColour, const char* format, va_list args) {
  const int bufferSize = 256;
  char buffer[bufferSize];
  vsprintf(buffer, format, args);
  int currentX = x;
  int currentY = y;

  uartPrintf("buffer is: %s\n", buffer);

  for(int i = 0 ; i < bufferSize ; i++) {
    if(buffer[i] == '\0') return;
    else if(currentX >= 320) { continue; }
    else if(buffer[i] == '\n') {
      currentY += _charHeight;
      currentX = x;
    } else {
      if(applyBg) {
	rgbPutcBg(fb, currentX, currentY, colour, bgColour, buffer[i]);
      } else {
	rgbPutc(fb, currentX, currentY, colour, buffer[i]);
      }
      currentX += _charWidth;
    }
  }
}


void rgbPrintfBg(uint16_t* fb, int x, int y, uint16_t colour, uint16_t bgColour, const char* format, ...) {
  va_list args;
  va_start(args, format);
  _rgbPrintf(fb, x, y, colour, true, bgColour, format, args);
  va_end(args);
}

void rgbPrintf(uint16_t* fb, int x, int y, uint16_t colour, const char* format, ...) {
  va_list args;
  va_start(args, format);
  _rgbPrintf(fb, x, y, colour, false, 0x0, format, args);
  va_end(args);
}

