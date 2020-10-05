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

class SDLScreen
{
public:
    SDLScreen(RAM* ram, SDL_Window* window);
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