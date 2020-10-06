#include <gb_sdl_screen.h>
#include <gb_screen_api.h>

#include <SDL2/SDL.h>
#include <boost/log/trivial.hpp>
#include <gb_typeutils.h>

GBScreenAPI::GBScreenPixelValue convertBitPairToPixelIntensityValue(uint8_t bit1, uint8_t bit2) {
    static const std::vector< std::vector<GBScreenAPI::GBScreenPixelValue> > _bitPairMap = {
        { GBScreenAPI::GBScreenPixelValue::OFF , GBScreenAPI::GBScreenPixelValue::MEDIUM},
        { GBScreenAPI::GBScreenPixelValue::LOW, GBScreenAPI::GBScreenPixelValue::HIGH}
    };

    return _bitPairMap[bit1][bit2];
}

SDLScreen::SDLScreen(RAM* ram, SDL_Window* window, DisplayPalette palette)
    : d_ram(ram), d_sdlWindow(window), d_colorPalette(palette)
{
    d_sdlRenderer = SDL_CreateRenderer(d_sdlWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    // Resize the 2d vector for us to fit all of our active tiles in it
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

    int x = 0;
    int y = 0;
    SDL_Rect pos;
    AddressRange range;

    for (auto i = d_bttRange.start; i < d_bttRange.end; i += sizeof(ByteType)) {
        DisplayTile* tile = new DisplayTile();

        // Set the 1-byte long range as to where each tile lives.
        range.start = i;
        range.end = i + sizeof(ByteType);

        // Every tile occupies 8x8 boxes, and we position them in a grid determined by x, y
        pos.h = 8;
        pos.w = 8;
        pos.x = y * 8;
        pos.y = x * 8;

        // Create the SDL texture and insert it into the map
        tile->texture = SDL_CreateTexture(d_sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 8, 8);
        tile->pos = pos;
        d_bttLayout[x][y] = tile;

        if (d_bttLayout[x][y]->texture == NULL ) {
            BOOST_LOG_TRIVIAL(error) << "SDL_Texture not created on Tile Table setup! x/y: " << x << "/" << y;
        }

        x++;
        if (x >= 32) {
            x = 0;
            y++;
        }
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

std::shared_ptr<std::vector<std::tuple<int, int, SDL_Color> > > decodeGBTileRow(WordType word, int rowNum, const DisplayPalette& palette)
{
    // Let's allocate the vector memory here and pass it on as a shared_ptr
    auto pixelList = std::make_shared<std::vector<std::tuple<int, int, SDL_Color> > >();

    // Let's convert it to a word register so we can have access to high and low params
    WordRegister rowData;
    rowData.word = word;

    ByteType& hi = rowData.hi;
    ByteType& lo = rowData.lo;

    int x = 0;
    for (auto i = 0; i < sizeof(ByteType)*8; ++i) {
        auto nextHiVal = hi & 0x1;
        auto nextLoVal = lo & 0x1;
        // Decode the hi,lo pair of bits
        auto gbPixel = convertBitPairToPixelIntensityValue(nextHiVal, nextLoVal);
        
        // Convert it to a color from our attached palette
        auto gbPixelColor = palette.mapIntensityToPalette(gbPixel);

        // Now, where does this pixel actually go on our screen?
        pixelList->push_back(std::make_tuple(x, rowNum, gbPixelColor));        

        // Next pair
        hi = hi >> 1;
        lo = lo >> 1;
        ++x;
    }
    return pixelList;
}




void SDLScreen::processBTTUpdate(Address addr, RAM::SegmentUpdateData data)
{
    // A value in the BTT has changed, so we need to free an existing resource
    // and create a new SDL_Texture resource if necessary

    // Find the associate tile data range in memory
    auto renderer = d_sdlRenderer;

    AddressRange newTileData;
    int tileNumber = (int)d_ram->readByte(addr);
    newTileData.start = d_tptRangeSignedIngress + (tileNumber * 16);
    newTileData.end = newTileData.start + 16;
    BOOST_LOG_TRIVIAL(debug) << "Determined new tile data to be at: " << std::hex << newTileData.start << ", " << std::hex << newTileData.end << " for tile number " << tileNumber;

    DisplayTile* tile = findDisplayTile(addr, d_bttRange);
    if (tile == NULL) {
        BOOST_LOG_TRIVIAL(error) << "NO display tile exists for address " << std::hex << addr;
        return;
    }

    tile->sourceRange = newTileData;


    SDL_Texture *texture = tile->texture;
    SDL_SetRenderTarget(renderer, texture);

    int row = 0;
    for (Address addr = tile->sourceRange.start; addr < tile->sourceRange.end; addr += sizeof(WordType))
    {
        WordType memWord = d_ram->readWord(addr);
        auto rowPixelList = decodeGBTileRow(memWord, row, d_colorPalette);

        // Go through the pixel list an dupdate
        for (auto updatePixel : *rowPixelList)
        {
            auto [drawX, drawY, drawColor] = updatePixel;
            SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
            SDL_RenderDrawPoint(renderer, drawX, drawY);
        }
        

        // Increment..
        ++row;
    }

    SDL_SetRenderTarget(renderer, NULL);

    SDL_RenderCopy(renderer, texture, NULL, &tile->pos);

    SDL_RenderPresent(renderer);
}

