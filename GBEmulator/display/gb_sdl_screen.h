#ifndef INCLUDE_GB_SDL_SCREEN_HEADER
#define INCLUDE_GB_SDL_SCREEN_HEADER

#include <display/gb_screen_api.h>
#include <ram/gb_ram.h>
#include <SDL2/SDL.h>

struct DisplayTile {
    SDL_Texture* texture;
    SDL_Rect pos;
    AddressRange sourceRange;

    DisplayTile()
        : texture(NULL) {}

    DisplayTile(const DisplayTile& t)
        : texture(t.texture), pos(t.pos), sourceRange(t.sourceRange) {}

};

struct DisplayPalette {
    // Kinda nasty to have them predefined. Should make it all - including 
    // declarations config based
    // TODO: Drive declarations off of config file
    SDL_Color offVal;
    SDL_Color lowVal;
    SDL_Color medVal;
    SDL_Color highVal;

    DisplayPalette(const SDL_Color& off, const SDL_Color& low, const SDL_Color& med, const SDL_Color& hi) :
        offVal(off), lowVal(low), medVal(med), highVal(hi) {}
    
    DisplayPalette(const DisplayPalette& d) :
        offVal(d.offVal), lowVal(d.lowVal), medVal(d.medVal), highVal(d.highVal) {}

    SDL_Color mapIntensityToPalette(const GBScreenAPI::GBScreenPixelValue& intensity) const {
        switch (intensity) {
            case GBScreenAPI::GBScreenPixelValue::OFF:
                return offVal;
            case GBScreenAPI::GBScreenPixelValue::LOW:
                return lowVal;
            case GBScreenAPI::GBScreenPixelValue::MEDIUM:
                return medVal;
            case GBScreenAPI::GBScreenPixelValue::HIGH:
                return highVal;
            default:
                BOOST_LOG_TRIVIAL(warning) << "Palette color matching intensity " 
                        << intensity << " not found. Using OFF value";
                return offVal;
        }
    }
};

class SDLScreen
{
public:
    SDLScreen(RAM* ram, SDL_Window* window, DisplayPalette palette);
    void drawScreen();
    void loadBackgroundTileMap();
    void processVRAMUpdate(Address addr, RAM::SegmentUpdateData data);
    void processBTTUpdate(Address addr, RAM::SegmentUpdateData data);

private:
    std::pair<AddressRange, SDL_Texture*> lookupActiveTile(const Address& address);
    DisplayTile* findDisplayTile(Address addr, AddressRange range);


private:
    RAM* d_ram;
    SDL_Window* d_sdlWindow;
    SDL_Renderer* d_sdlRenderer;
    DisplayPalette d_colorPalette;

    AddressRange d_bttRange;
    AddressRange d_wttRange;
    Address d_tptRangeSignedIngress;
    Address d_tptRangeUnsignedIngress;
    std::vector< std::vector<DisplayTile*> > d_bttLayout;
    std::vector< std::vector<DisplayTile*> > d_wttLayout;

    // For search optimization purposes
    std::vector<AddressRange> d_activeRanges;
    std::map<AddressRange, SDL_Texture*> d_activeTileMap;
};

#endif