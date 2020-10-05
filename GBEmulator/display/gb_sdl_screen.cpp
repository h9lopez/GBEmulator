#include "gb_sdl_screen.h"

#include <SDL2/SDL.h>
#include <boost/log/trivial.hpp>

// bool operator <(const AddressRange& x, const AddressRange& y) {
//     return std::tie(x.start, x.end) < std::tie(y.start, y.end);
// }

// bool operator==(const AddressRange& x, const AddressRange& y) {
//     return std::tie(x.start, x.end) == std::tie(y.start, y.end);
// }


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

void SDLScreen::loadBackgroundTileMap()
{
    // Read the section of RAM determining pointers to tile maps and load them as textures
    // BTT is at 9800-9BFF
    
    // For now, only read index number 1 and load that as an active tile
    AddressRange range;
    range.start = 0x9800;
    range.end = 0x9BFF;

    Address addr = range.start;
    auto mappedTileNumber = d_ram->readByte(addr + sizeof(ByteType));

    AddressRange tileData;
    tileData.start = 0x9000 + (sizeof(ByteType)*mappedTileNumber);
    tileData.end = tileData.start + 16;

    BOOST_LOG_TRIVIAL(debug) << "Registering and creating texture for tile data range, start=" << std::hex << tileData.start << " - " << std::hex << tileData.end;
    // Register it as an active tile
    SDL_Texture* texture = SDL_CreateTexture(d_sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 8, 8);
    auto insertRes = d_activeTileMap.insert(std::pair<AddressRange, SDL_Texture*>(tileData, texture));
}


std::pair<AddressRange, SDL_Texture*> SDLScreen::lookupActiveTile(const Address& address)
{
    BOOST_LOG_TRIVIAL(debug) << "Finding " << std::hex << address << " in active on-screen tiles";
    auto res = std::find_if(d_activeTileMap.begin(), d_activeTileMap.end(), [address](const std::pair<AddressRange, SDL_Texture*>& item) {
        BOOST_LOG_TRIVIAL(debug) << "Comparing " << std::hex << address << " with " << std::hex << item.first.start << ", " << std::hex << item.first.end;
        return address >= item.first.start && address <= item.first.end;
    });
    
    if (res != d_activeTileMap.end()) {
        return *res;
    }
    BOOST_LOG_TRIVIAL(debug) << "No active tile found for address " << std::hex << address;
    return std::pair<AddressRange, SDL_Texture*>();
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

    

    void *pixels = NULL;
    int pitch;
    /*if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
        BOOST_LOG_TRIVIAL(error) << "Could not lock SDL texture for writing. Not writing for VRAM update to " << addr;
        return;
    }*/
    BOOST_LOG_TRIVIAL(debug) << "Read pixel pitch of: " << pitch;

    uint32_t* pixelPos = (uint32_t*)pixels;
    uint32_t format;
    SDL_QueryTexture(texture, &format, NULL, NULL, NULL);
    auto pixelFormat = SDL_AllocFormat(format);


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
                SDL_RenderDrawPoint(renderer, row, col);
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


void SDLScreen::processVRAMUpdate(Address addr, RAM::SegmentUpdateData data)
{
    // Associate the address with an active on-screen tile.
    // If none is found, then we do nothing.
    auto lookupResult = lookupActiveTile(addr);
    if (lookupResult.second == NULL) {
        BOOST_LOG_TRIVIAL(debug) << "Did not find applicable texture for address " << std::hex << addr;
        return;
    }

    // Caught something, so create an SDL Surface to quickly fill it from the address range
    SDL_Texture* texture = lookupResult.second;
    void *pixels = NULL;
    int pitch;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
        BOOST_LOG_TRIVIAL(error) << "Could not lock SDL texture for writing. Not writing for VRAM update to " << addr;
        return;
    }

    BOOST_LOG_TRIVIAL(debug) << "Read pixel pitch of: " << pitch;


    // Now, we have to map the updates address value with whatever we need to overwrite in the buffer
    // TODO: Optimize to specifically modify a portion of the buffer instead of the raw copy?
    // For now the raw copy of the entire address range should be okay
    uint32_t* pixelPos = (uint32_t*)pixels;
    for (Address addr = lookupResult.first.start; addr < lookupResult.first.end; addr += sizeof(ByteType))
    {
        BOOST_LOG_TRIVIAL(debug) << "Copying data from address " << std::hex << addr;
        ByteType memVal = d_ram->readByte(addr);

        uint32_t format;
        SDL_QueryTexture(texture, &format, NULL, NULL, NULL);
        auto pixelFormat = SDL_AllocFormat(format);
        *pixelPos = SDL_MapRGBA(pixelFormat, 255, 255, 255, 0);
        pixelPos += sizeof(uint32_t);
    }

    SDL_Rect dstrct;
    dstrct.x = 0;
    dstrct.y = 0;
    dstrct.h = 8;
    dstrct.w = 8;
    SDL_UnlockTexture(texture);
    SDL_SetRenderTarget(d_sdlRenderer, texture);
    SDL_RenderCopy(d_sdlRenderer, texture, NULL, NULL);
    SDL_RenderPresent(d_sdlRenderer);
}
