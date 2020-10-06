#ifndef INCLUDE_GB_SDL_SCREEN_HEADER
#define INCLUDE_GB_SDL_SCREEN_HEADER

#include <display/gb_screen_api.h>
#include <ram/gb_ram.h>
#include <SDL2/SDL.h>

struct DisplayTile {
    SDL_Texture* texture;
    AddressRange sourceRange;
    std::map<SDL_Color, std::vector<SDL_Point> > redrawMap;

    DisplayTile()
        : texture(NULL) {}

    DisplayTile(const DisplayTile& t)
        : texture(t.texture), sourceRange(t.sourceRange) {}

};



struct DisplayGridItem {
    DisplayTile* tile;
    SDL_Rect* pos;
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
    DisplayGridItem* findDisplayTile(Address addr, AddressRange range);


private:
    RAM* d_ram;
    SDL_Window* d_sdlWindow;
    SDL_Renderer* d_sdlRenderer;
    DisplayPalette d_colorPalette;
    std::map<SDL_Color, std::vector<SDL_Point> > d_redrawMap;

    AddressRange d_bttRange;
    AddressRange d_wttRange;
    Address d_tptRangeSignedIngress;
    Address d_tptRangeUnsignedIngress;
    std::vector< std::vector<DisplayGridItem*> > d_bttLayout;
    std::vector< std::vector<DisplayGridItem*> > d_wttLayout;
};

#endif