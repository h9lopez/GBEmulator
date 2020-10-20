#ifndef INCLUDE_GB_SDL_SCREEN_HEADER
#define INCLUDE_GB_SDL_SCREEN_HEADER

#include <display/gb_screen_api.h>
#include <ram/gb_ram.h>
#include <SDL2/SDL.h>

struct SDL_Color_Comp : public std::binary_function<SDL_Color, SDL_Color, bool> {
    bool operator()(const SDL_Color& a, const SDL_Color& b) const { 
        return std::tie(a.r, a.g, a.b, a.a) < std::tie(b.r, b.g, b.b, b.a);
    }
};

struct DisplayTile {
    SDL_Texture* texture;
    AddressRange sourceRange;
    std::map<SDL_Color, std::vector<SDL_Point>, SDL_Color_Comp > redrawMap;

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

struct ActivePalette {
    GBScreenAPI::GBScreenPixelValue color0;
    GBScreenAPI::GBScreenPixelValue color1;
    GBScreenAPI::GBScreenPixelValue color2;
    GBScreenAPI::GBScreenPixelValue color3;

    ActivePalette(GBScreenAPI::GBScreenPixelValue c0, GBScreenAPI::GBScreenPixelValue c1,
                 GBScreenAPI::GBScreenPixelValue c2, GBScreenAPI::GBScreenPixelValue c3)
        : color0(c0), color1(c1), color2(c2), color3(c3)
    {}

    ActivePalette(const ActivePalette& o) 
        : color0(o.color0), color1(o.color1), color2(o.color2), color3(o.color3)
    {}

    ActivePalette()
    {}
};

class SDLScreen
{
public:
    SDLScreen(RAM* ram, SDL_Window* window, DisplayPalette palette);
    void drawScreen() const;
    void loadBackgroundTileMap();
    void processVRAMUpdate(Address addr, RAM::SegmentUpdateData data);
    void processBTTUpdate(Address addr, RAM::SegmentUpdateData data);
    void processWTTUpdate(Address addr, RAM::SegmentUpdateData data);

    // attached to FF47 IO port
    void processBGPUpdate(Address addr, RAM::SegmentUpdateData data);
    void processOBP0Update(Address addr, RAM::SegmentUpdateData data);
    void processOBP1Update(Address addr, RAM::SegmentUpdateData data);
    void processLCDCUpdate(Address addr, RAM::SegmentUpdateData data);

private:
    std::pair<AddressRange, SDL_Texture*> lookupActiveTile(const Address& address);
    DisplayGridItem* findDisplayTile(Address addr) const;
    void _initTileTable(const AddressRange& addrRange, 
                        std::vector< std::vector<DisplayGridItem*> >&layoutTable,
                        std::map<Address, DisplayGridItem*>& lookupMap );


private:
    RAM* d_ram;
    SDL_Window* d_sdlWindow;
    SDL_Renderer* d_sdlRenderer;
    DisplayPalette d_colorPalette;
    std::map<SDL_Color, std::vector<SDL_Point>, SDL_Color_Comp> d_redrawMap;

    AddressRange d_bttRange;
    AddressRange d_wttRange;
    Address d_tptRangeSignedIngress;
    Address d_tptRangeUnsignedIngress;
    std::vector< std::vector<DisplayGridItem*> > d_bttLayout;
    std::vector< std::vector<DisplayGridItem*> > d_wttLayout;

    std::map<Address, DisplayGridItem*> d_bttLookupGrid;
    std::map<Address, DisplayGridItem*> d_wttLookupGrid;

    // Active palettes
    ActivePalette d_backgroundPalette;
    ActivePalette d_spritePalette0;
    ActivePalette d_spritePalette1;

    // Active LCDC State
    GBScreenAPI::LCDCState d_lcdcState;
};

#endif