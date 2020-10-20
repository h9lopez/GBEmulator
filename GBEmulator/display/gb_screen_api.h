#ifndef GB_SCREEN_API_HEADER
#define GB_SCREEN_API_HEADER

#include <memory>

#include <utils/gb_typeutils.h>
#include <ram/gb_ram.h>
#include <utils/addressrange.h>
#include <SDL2/SDL_rect.h>

namespace GBScreenAPI {

struct LCDCState {
    bool lcdOn;
    bool windowOn;
    bool spritesOn;
    bool backgroundOn;
    std::pair<int, int> spriteDimension;
    AddressRange windowTileMapSelect;
    AddressRange backgroundTileMapSelect;
    AddressRange tileDataSelect;

    // static defaults
    std::vector<AddressRange> windowTileMapRanges{
                AddressRange(0x9800, 0x9BFF),
                AddressRange(0x9C00, 0x9FFF)
    };

    std::vector<AddressRange> tileDataRanges{
        AddressRange(0x8800, 0x97FF),
        AddressRange(0x8000, 0x8FFF)
    };

    std::vector<AddressRange> backgroundTileMapRanges{
        AddressRange(0x9800, 0x9BFF),
        AddressRange(0x9C00, 0x9FFF)
    };

    std::vector< std::pair<int, int> > spriteSizeRanges{
        {8, 8},
        {8, 16}
    };

    LCDCState()
        : lcdOn(false), windowOn(false), spritesOn(false), backgroundOn(false)
    {

    }

    LCDCState(const LCDCState& o)
        : lcdOn(o.lcdOn), windowOn(o.windowOn), spritesOn(o.spritesOn), backgroundOn(o.backgroundOn),
          spriteDimension(o.spriteDimension), windowTileMapSelect(o.windowTileMapSelect),
          backgroundTileMapSelect(o.backgroundTileMapSelect), tileDataSelect(o.tileDataSelect)
    {}

    LCDCState(bool lOn, bool wOn, bool sOn, bool bOn, std::pair<int, int> sD, AddressRange wTMS, AddressRange bTMS, AddressRange tDS)
        : lcdOn(lOn), windowOn(wOn), spritesOn(sOn), backgroundOn(bOn), spriteDimension(sD), 
          windowTileMapSelect(wTMS), backgroundTileMapSelect(bTMS), tileDataSelect(tDS)
    {}

    LCDCState(ByteType rawMemoryState)
    {
        auto rawBitset = std::bitset<BYTETYPE_SIZE>(rawMemoryState);
        backgroundOn =              rawBitset[0];
        spritesOn =                 rawBitset[1];
        spriteDimension =           spriteSizeRanges[rawBitset[2]];
        backgroundTileMapSelect =   backgroundTileMapRanges[rawBitset[3]];
        tileDataSelect =            tileDataRanges[rawBitset[4]];
        windowOn =                  rawBitset[5];
        windowTileMapSelect =       windowTileMapRanges[rawBitset[6]];
        lcdOn =                     rawBitset[7];
    }
};

typedef enum {
    SIGNED_MODE = 0,
    UNSIGNED_MODE = 1
} TileAddressingMode;

typedef enum {
    OFF = 0b11, // 0% aka black
    LOW = 0b10, // 33% aka dark grey
    MEDIUM = 0b01, // 66% aka light grey
    HIGH = 0b00// 100% aka white
} GBScreenPixelValue;

class IDisplay
{
    public:
        virtual void drawScreen() const;
        
        // Signal handlers
        
    private:
        std::unique_ptr<RAM> memPtr;
};



}

#endif