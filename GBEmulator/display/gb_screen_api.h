#ifndef GB_SCREEN_API_HEADER
#define GB_SCREEN_API_HEADER

#include <memory>

#include <utils/gb_typeutils.h>
#include <ram/gb_ram.h>

namespace GBScreenAPI {

typedef enum {
    SIGNED_MODE = 0,
    UNSIGNED_MODE = 1
} TileAddressingMode;

typedef enum {
    OFF = 0, // 0% aka black
    LOW = 33, // 33% aka dark grey
    MEDIUM = 66, // 66% aka light grey
    HIGH = 100// 100% aka white
} GBScreenPixelValue;

class IDisplay
{
    public:
        virtual void drawScreen();
        
        // Signal handlers
        
    private:
        std::auto_ptr<RAM> memPtr;
};



}

#endif