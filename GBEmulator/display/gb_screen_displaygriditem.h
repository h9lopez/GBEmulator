#if !defined(GB_SCREEN_DISPLAYGRIDITEM_INCLUDED)
#define GB_SCREEN_DISPLAYGRIDITEM_INCLUDED
#include <SDL2/SDL.h>
#include <gb_screen_displaytile.h>

struct DisplayGridItem {
    DisplayTile* tile;
    SDL_Rect* pos;
};

#endif // GB_SCREEN_DISPLAYGRIDITEM_INCLUDED
