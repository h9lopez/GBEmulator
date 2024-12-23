#if !defined(GB_SCREEN_DISPLAY_TILE_INCLUDED)
#define GB_SCREEN_DISPLAY_TILE_INCLUDED

#include <SDL2/SDL.h>
#include <utils/addressrange.h>

struct DisplayTile {
    SDL_Texture* texture;
    AddressRange sourceRange;
    // The tile number to which GB programs will refer to
    unsigned int referenceNum;

    DisplayTile()
        : texture(NULL) {}

    DisplayTile(const DisplayTile& t)
        : texture(t.texture), sourceRange(t.sourceRange), referenceNum(t.referenceNum) {}

};


#endif // GB_SCREEN_DISPLAY_TILE_INCLUDED
