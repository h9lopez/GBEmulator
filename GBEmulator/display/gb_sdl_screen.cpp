#include "gb_sdl_screen.h"

#include <SDL2/SDL.h>
#include <boost/log/trivial.hpp>


SDLScreen::SDLScreen(RAM* ram, SDL_Window* window)
    : d_ram(ram), d_sdlWindow(window)
{

    d_sdlRenderer = SDL_CreateRenderer(d_sdlWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    BOOST_LOG_TRIVIAL(debug) << "any error? " << SDL_GetError();

    d_bttLayout.resize(32);
    d_wttLayout.resize(32);
    for (auto i = 0; i < 32; ++i) {
        d_bttLayout[i].resize(32);
        d_wttLayout[i].resize(32);
    }

    // Populate BTT and WTT layouts
    d_bttRange.start = 0x9800;
    d_bttRange.end = 0x9BFF;
    d_wttRange.start = 0x9C00;
    d_wttRange.end = 0x9FFF;
    d_tptRangeSignedIngress = 0x9000;
    d_tptRangeUnsignedIngress = 0x8000;

    int row = 0;
    int col = 0;
    for (auto i = d_bttRange.start; i < d_bttRange.end; i += sizeof(ByteType)) {
        BOOST_LOG_TRIVIAL(debug) << "Creating SDLTexture for BTT tile starting at " << std::hex << i;
        DisplayTile* tile = new DisplayTile();
        SDL_Rect pos;
        AddressRange range;
        range.start = i;
        range.end = i + sizeof(ByteType);

        pos.h = 8;
        pos.w = 8;
        pos.x = col * 8;
        pos.y = row * 8;

        tile->texture = SDL_CreateTexture(d_sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 8, 8);
        tile->pos = pos;
        d_bttLayout[row][col] = tile;
        BOOST_LOG_TRIVIAL(debug) << "Laid out new texture for displaytile at:\n"
            << "\tsourceRange: 0x" << std::hex << tile->sourceRange.start << " - " << std::hex << tile->sourceRange.end << "\n"
            << "\tpos: " << std::dec << pos.x << ", " << pos.y;

        if (d_bttLayout[row][col]->texture == NULL ) {
            BOOST_LOG_TRIVIAL(debug) << "INVALID texture found!!";
        }

        row++;
        if (row >= 32) {
            row = 0;
            col++;
        }
        BOOST_LOG_TRIVIAL(debug) << "Populated row=" << row << ", col=" << col;
    }
}

void SDLScreen::drawScreen()
{
    // Get window surface
    auto sdlSurface = SDL_GetWindowSurface(d_sdlWindow);

}

DisplayTile* SDLScreen::findDisplayTile(Address addr, AddressRange range) {
    // Calculate for now.. Cache later

    int row = 0;
    int col = 0;
    // TODO: Assumes BTT
    Address properStart = d_bttRange.start;
    for (auto i = range.start; i < range.end; ++i)
    {
        // For each unit, it takes up one byte each
        // Use that fact to calculate the address based on row&col
        if (addr >= properStart) {
            BOOST_LOG_TRIVIAL(debug) << "Found display tile match for addr " << std::hex << " at " << std::dec << row << ", " << col;
            return d_bttLayout[row][col];
        }

        properStart += sizeof(ByteType);
        row ++;

        if (row >= 32) {
            row = 0;
            col++;
        }   
    }


    return NULL;
}

void SDLScreen::processBTTUpdate(Address addr, RAM::SegmentUpdateData data)
{
    // A value in the BTT has changed, so we need to free an existing resource
    // and create a new SDL_Texture resource if necessary

    // Find the associate tile data range in memory
    auto renderer = d_sdlRenderer;

    AddressRange newTileData;
    int tileNumber = (int)d_ram->readByte(addr);
    BOOST_LOG_TRIVIAL(debug) << "Looking up tile number " << tileNumber;
    newTileData.start = d_tptRangeSignedIngress + (tileNumber * 16);
    newTileData.end = newTileData.start + 16;
    BOOST_LOG_TRIVIAL(debug) << "Determined new tile data to be at: " << std::hex << newTileData.start << ", " << std::hex << newTileData.end;

    DisplayTile* tile = findDisplayTile(addr, d_bttRange);
    if (tile == NULL) {
        BOOST_LOG_TRIVIAL(error) << "NO display tile exists for address " << std::hex << addr;
        return;
    }
    BOOST_LOG_TRIVIAL(debug) << "Found display tile for address range " << std::hex << tile->sourceRange.start << ", " << std::hex << tile->sourceRange.end;


    BOOST_LOG_TRIVIAL(debug) << "Changing source range of display tile from " 
                << std::hex << tile->sourceRange.start << " - " << std::hex << tile->sourceRange.end
                << " to " 
                << std::hex << newTileData.start << " - " << std::hex << newTileData.end;
    tile->sourceRange = newTileData;


    SDL_Texture *texture = tile->texture;
    if (tile->texture == NULL) { 
        BOOST_LOG_TRIVIAL(error) << "INVALID texture within DisplayTile";
        return;
    }


    SDL_SetRenderTarget(renderer, texture);
    BOOST_LOG_TRIVIAL(debug) << "any error? " << SDL_GetError();

    int totalPixelsPainted = 0;
    int row = 0;
    int col = 0;
    for (Address addr = tile->sourceRange.start; addr < tile->sourceRange.end; addr += sizeof(WordType))
    {
        WordType memWord = d_ram->readWord(addr);
        // Construct a line from the two bytes in RAM then put in texture
        size_t i = 0;
        for (i = 0; i < sizeof(WordType)*4; ++i) {
            auto ramPixelValue = memWord & 0x3;

            SDL_Color color;
            if (ramPixelValue > 0) {
                // White
                color.r = 255;
                color.g = 255;
                color.b = 255;
                color.a = 0;
                BOOST_LOG_TRIVIAL(debug) << "ACTIVE PIXEL";

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                BOOST_LOG_TRIVIAL(debug) << "Putting point at " << row << ", " << col;
                // TODO: Don't know if col and row are actually col and row or should be reversed...
                SDL_RenderDrawPoint(renderer, col, row);
                BOOST_LOG_TRIVIAL(debug) << "any error? " << SDL_GetError();
            }
            else {
                color.r = 0;
                color.g = 0;
                color.b = 0;
                color.a = 0;

            }



            col++;
            if (col >= 8) {
                col = 0;
                row++;
            }
            memWord = memWord >> 2;
            totalPixelsPainted++;
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "Rendering texture at " << tile->pos.x << ", " << tile->pos.y;
    //SDL_UnlockTexture(texture);

    SDL_SetRenderTarget(renderer, NULL);
    BOOST_LOG_TRIVIAL(debug) << "any error? srt" << SDL_GetError();

    SDL_RenderCopy(renderer, texture, NULL, &tile->pos);
    BOOST_LOG_TRIVIAL(debug) << "any error? rcp" << SDL_GetError();

    SDL_RenderPresent(renderer);
    BOOST_LOG_TRIVIAL(debug) << "any error? rp" << SDL_GetError();


    BOOST_LOG_TRIVIAL(debug) << "Went through " << std::dec << totalPixelsPainted << " pixels";

}

