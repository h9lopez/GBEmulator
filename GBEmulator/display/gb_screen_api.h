#ifndef GB_SCREEN_API_HEADER
#define GB_SCREEN_API_HEADER

#include <memory>

#include "gb_typeutils.h"
#include "gb_ram.h"

class IDisplay
{
    public:
        virtual void drawScreen();
        
        // Signal handlers
        
    private:
        std::auto_ptr<RAM> memPtr;
};

#endif