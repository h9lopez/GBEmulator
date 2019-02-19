#ifndef INCLUDE_GB_ASCII_SCREEN_HEADER
#define INCLUDE_GB_ASCII_SCREEN_HEADER

#include "gb_screen_api.h"
#include "gb_ram.h"

class ASCIIScreen : public IDisplay
{
    public:
        ASCIIScreen(RAM &ram);
        void drawScreen();
};


#endif