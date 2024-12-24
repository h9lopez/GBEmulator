#include <SDL2/SDL_render.h>
#include <gb_sdl_screen.h>
#include <gb_screen_api.h>

#include <SDL2/SDL.h>
#include <boost/log/trivial.hpp>
#include <gb_typeutils.h>
#include <addressrange.h>
#include <boost/range/iterator_range.hpp>

namespace {
    // Helper functions
    ActivePalette decodePaletteData(ByteType value)
    {
        ActivePalette res;
        auto rawBitset = std::bitset<BYTETYPE_SIZE>(value);
        ByteType tmp = rawBitset[1];
        res.color0 = static_cast<GBScreenAPI::GBScreenPixelValue>((tmp << 1) | rawBitset[0]);
        tmp = rawBitset[3];
        res.color1 = static_cast<GBScreenAPI::GBScreenPixelValue>((tmp << 1) | rawBitset[2]);
        tmp = rawBitset[5];
        res.color2 = static_cast<GBScreenAPI::GBScreenPixelValue>((tmp << 1) | rawBitset[4]);
        tmp = rawBitset[7];
        res.color3 = static_cast<GBScreenAPI::GBScreenPixelValue>((tmp << 1) | rawBitset[6]);
        return res;
    }
}


GBScreenAPI::GBScreenPixelValue convertBitPairToPixelIntensityValue(uint8_t bit1, uint8_t bit2) {
    static const std::vector< std::vector<GBScreenAPI::GBScreenPixelValue> > _bitPairMap = {
        { GBScreenAPI::GBScreenPixelValue::OFF , GBScreenAPI::GBScreenPixelValue::MEDIUM},
        { GBScreenAPI::GBScreenPixelValue::LOW, GBScreenAPI::GBScreenPixelValue::HIGH}
    };

    return _bitPairMap[bit1][bit2];
}

SDLScreen::SDLScreen(RAM* ram, SDL_Window* window, DisplayPalette palette)
    : d_ram(ram), d_sdlWindow(window), d_colorPalette(palette), d_layerRenderer()
{
    SDL_Color_Comp cmp;
    d_redrawMap = std::map<SDL_Color, std::vector<SDL_Point>, SDL_Color_Comp>(cmp);
    d_sdlRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(d_sdlWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE), SDL_DestroyRenderer);

    _initTileTable(d_upperTileMapRange, d_upperTileMapLookupGrid);
    _initTileTable(d_lowerTileMapRange, d_lowerTileMapLookupGrid);

    // Insert the tile tables as render targets
    // Initialize them both as unshown at construction time

    BOOST_LOG_TRIVIAL(info) << "Creating Layer objects";
    d_backgroundLayer = std::shared_ptr<Layer>(new Layer(GBScreenAPI::RenderLayer::BACKGROUND, false, d_ram));
    d_windowLayer = std::shared_ptr<Layer>(new Layer(GBScreenAPI::RenderLayer::WINDOW, false, d_ram));
    d_layerRenderer = LayerRenderer(d_backgroundLayer, d_sdlRenderer);

}

void SDLScreen::_initTileTable(const AddressRange& addrRange, 
                                std::map<Address, DisplayGridItem*>& lookupMap )
{
    int x = 0;
    int y = 0;
    for (auto i = addrRange.start; i <= addrRange.end; i += sizeof(ByteType)) {
        DisplayTile* tile = new DisplayTile();

        // Create the SDL texture and insert it into the map
        tile->texture = SDL_CreateTexture(d_sdlRenderer.get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GB_TILE_PIXEL_WIDTH, GB_TILE_PIXEL_HEIGHT);

        DisplayGridItem* newItem = new DisplayGridItem{
            .tile = tile, 
            .pos = new SDL_Rect{
                .x = x*GB_TILE_PIXEL_WIDTH, 
                .y = y*GB_TILE_PIXEL_HEIGHT, 
                .w = GB_TILE_PIXEL_WIDTH, 
                .h = GB_TILE_PIXEL_HEIGHT
            } 
        };
        lookupMap.insert(std::pair<Address, DisplayGridItem*>(i, newItem));

        if (newItem->tile->texture == NULL ) {
            BOOST_LOG_TRIVIAL(error) << "SDL_Texture not created on Tile Table setup! x/y: " << x << "/" << y;
        }

        x++;
        if (x >= GB_TILETABLE_WIDTH) {
            x = 0;
            y++;
        }
    }
}

void SDLScreen::drawScreen() const
{

    for (auto &[layer, target] : d_renderTargets)
    {
        auto& tileTable = std::get<1>(target);
        auto& targetActive = std::get<0>(target);
        if (!targetActive) {
            continue;
        }

        auto tileStart = tileTable.ingress;
        int totalTiles = 0;
        for (auto i = d_upperTileMapRange.start; 
                i < d_upperTileMapRange.end; 
                i += sizeof(ByteType), tileStart += sizeof(ByteType) 
        )
        {
            // Wow this sucks time-wise
            auto gridItem = findDisplayTile(tileStart);
            auto tile = gridItem->tile;

            // Sanity check the tile. If it's bad or untouched, then don't render it.
            /*if (!tile->texture || tile->redrawMap.size() == 0) {
                continue;
            }*/

            SDL_SetRenderTarget(d_sdlRenderer.get(), tile->texture);
            // Get draw data
            /*for (auto &[color, points] : tile->redrawMap)
            {
                SDL_SetRenderDrawColor(d_sdlRenderer, color.r, color.g, color.b, color.a);

                SDL_RenderDrawPoints(d_sdlRenderer, &points[0], points.size());
            }*/
            
            SDL_SetRenderTarget(d_sdlRenderer.get(), nullptr);
            SDL_RenderCopy(d_sdlRenderer.get(), tile->texture, nullptr, gridItem->pos);
        }
        SDL_RenderPresent(d_sdlRenderer.get());

    }
}

// DisplayTile* SDLScreen::findDisplayTile(Address addr, GBScreenAPI::TileAddressingMode addrMode)
// {
//     switch (addrMode) {
//         case GBScreenAPI::TileAddressingMode::SIGNED_MODE:
//         case GBScreenAPI::TileAddressingMode::UNSIGNED_MODE:
//         default:
//             BOOST_LOG_TRIVIAL(error) << "Unrecognized addresesing mode for display tile lookup";
//     }
// }

DisplayGridItem* SDLScreen::findDisplayTile(Address addr) const {
    auto gridItemIt = d_upperTileMapLookupGrid.find(addr);
    if (gridItemIt == d_upperTileMapLookupGrid.end()) {
        return NULL;
    }

    return gridItemIt->second;
}

std::shared_ptr<std::vector<std::tuple<SDL_Point, SDL_Color> > > decodeGBTileRow(WordType word, int rowNum, const DisplayPalette& palette)
{
    // Let's allocate the vector memory here and pass it on as a shared_ptr
    auto pixelList = std::make_shared<std::vector<std::tuple<SDL_Point, SDL_Color> > >();

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
        SDL_Point p = {.x = x, .y = rowNum};
        pixelList->push_back(std::make_tuple(p, gbPixelColor));        

        // Next pair
        hi = hi >> 1;
        lo = lo >> 1;
        ++x;
    }
    return pixelList;
}


void SDLScreen::processWTTUpdate(Address addr, RAM::SegmentUpdateData data)
{
    BOOST_LOG_TRIVIAL(debug) << "Processing WTT update for address " << std::hex  << addr;
}

void SDLScreen::processBGPUpdate(Address addr, RAM::SegmentUpdateData data)
{
    BOOST_LOG_TRIVIAL(info) << "Update at BGP port - updating global display palette.";
        BOOST_LOG_TRIVIAL(debug) << "Old BGP color0=" << d_backgroundPalette.color0
                << ", color1=" << d_backgroundPalette.color1
                << ", color2=" << d_backgroundPalette.color2
                << ", color3=" << d_backgroundPalette.color3;

    // Decode the data and construct the display palette object to override our instance var
    d_backgroundPalette = decodePaletteData(data.byte);

    BOOST_LOG_TRIVIAL(debug) << "New BGP color0=" << d_backgroundPalette.color0
                << ", color1=" << d_backgroundPalette.color1
                << ", color2=" << d_backgroundPalette.color2
                << ", color3=" << d_backgroundPalette.color3;
}

void SDLScreen::processOBP0Update(Address addr, RAM::SegmentUpdateData data)
{
    BOOST_LOG_TRIVIAL(info) << "Update at OBP0 port - updating global display palette.";
        BOOST_LOG_TRIVIAL(debug) << "Old OBP0 color0=" << d_backgroundPalette.color0
                << ", color1=" << d_backgroundPalette.color1
                << ", color2=" << d_backgroundPalette.color2
                << ", color3=" << d_backgroundPalette.color3;

    // Decode the data and construct the display palette object to override our instance var
    d_spritePalette0 = decodePaletteData(data.byte);

    BOOST_LOG_TRIVIAL(debug) << "New OBP0 color0=" << d_backgroundPalette.color0
                << ", color1=" << d_backgroundPalette.color1
                << ", color2=" << d_backgroundPalette.color2
                << ", color3=" << d_backgroundPalette.color3;
}


void SDLScreen::processOBP1Update(Address addr, RAM::SegmentUpdateData data)
{
    BOOST_LOG_TRIVIAL(info) << "Update at OBP1 port - updating global display palette.";
        BOOST_LOG_TRIVIAL(debug) << "Old OBP1 color0=" << d_backgroundPalette.color0
                << ", color1=" << d_backgroundPalette.color1
                << ", color2=" << d_backgroundPalette.color2
                << ", color3=" << d_backgroundPalette.color3;

    // Decode the data and construct the display palette object to override our instance var
    d_spritePalette1 = decodePaletteData(data.byte);

    BOOST_LOG_TRIVIAL(debug) << "New OBP1 color0=" << d_backgroundPalette.color0
                << ", color1=" << d_backgroundPalette.color1
                << ", color2=" << d_backgroundPalette.color2
                << ", color3=" << d_backgroundPalette.color3;
}

void SDLScreen::processLCDCUpdate(Address addr, RAM::SegmentUpdateData data)
{
    GBScreenAPI::LCDCState newState(data.byte);
    BOOST_LOG_TRIVIAL(info) << "Updated LCDC. lcdOn=" << newState.lcdOn 
        << ", windowOn=" << newState.windowOn
        << ", spritesOn=" << newState.spritesOn
        << ", backgroundOn=" << newState.backgroundOn
        << ", spriteDimension=(w=" << newState.spriteDimension.first << ", h=" << newState.spriteDimension.second << ")"
        << ", backgroundTileMapSelect=(start=" << std::hex << newState.backgroundTileMapSelect.start << ", end=" << std::hex << newState.backgroundTileMapSelect.end << ")"
        << ", windowTileMapSelect=(start=" << std::hex << newState.windowTileMapSelect.start << ", end=" << std::hex << newState.windowTileMapSelect.end << ")"
        << ", tileDataSelect=(start=" << std::hex << newState.tileDataSelect.start << ", end=" << std::hex << newState.tileDataSelect.end << ")";
    d_lcdcState = newState;

    // React to the LCDC update and set appropriate variables
    
    // We don't do anything here to turn off the display or not, so we send out
    // an external signal
    d_powerFlippedSignal(d_lcdcState.lcdOn);

    //d_backgroundLayer.

    std::tuple<bool, GBScreenAPI::TileDataRegionInfo> background;
    std::tuple<bool, GBScreenAPI::TileDataRegionInfo> window;

    std::get<0>(background) = d_lcdcState.backgroundOn;
    std::get<0>(window) = (d_lcdcState.backgroundOn) ? d_lcdcState.windowOn : false;

    GBScreenAPI::TileDataRegionInfo newTileDataRegion = {
        .range = d_lcdcState.tileDataSelect,
        .ingress = static_cast<Address>((d_lcdcState.tileDataSelect.start == 0x8000) ? 0x8000 : 0x9000),
        .addressingMode = (d_lcdcState.tileDataSelect.start == 0x8000) ? GBScreenAPI::TileDataAddressingMode::UNSIGNED_MODE : GBScreenAPI::TileDataAddressingMode::SIGNED_MODE
    }; 
    std::get<1>(background) = newTileDataRegion;
    std::get<1>(window) = newTileDataRegion;

    d_renderTargets[GBScreenAPI::RenderLayer::BACKGROUND] = background;
    d_renderTargets[GBScreenAPI::RenderLayer::WINDOW] = window;
}



void SDLScreen::processBTTUpdate(Address addr, RAM::SegmentUpdateData data)
{
    // A value in the BTT has changed, so we need to free an existing resource
    // and create a new SDL_Texture resource if necessary

    // Find the associate tile data range in memory

    //AddressRange newTileData;
    int tileNumber = (int)data.byte;
    //newTileData.start = d_tptRangeSignedIngress + (tileNumber * 16);
    //newTileData.end = newTileData.start + 16;
    //BOOST_LOG_TRIVIAL(debug) << "Determined new tile data to be at: " << std::hex << newTileData.start << ", " << std::hex << newTileData.end << " for tile number " << tileNumber;

    auto gridItem = findDisplayTile(addr);
    auto tile = gridItem->tile;
    if (tile == NULL) {
        BOOST_LOG_TRIVIAL(error) << "NO display tile exists for address " << std::hex << addr;
        return;
    }

    //tile->sourceRange = newTileData;


    SDL_Texture *texture = tile->texture;
    SDL_SetRenderTarget(d_sdlRenderer.get(), texture);

    int row = 0;
    for (Address addr = tile->sourceRange.start; addr < tile->sourceRange.end; addr += sizeof(WordType))
    {
        WordType memWord = d_ram->readWord(addr);
        auto rowPixelList = decodeGBTileRow(memWord, row, d_colorPalette);

        // Go through the pixel list an dupdate
        for (auto updatePixel : *rowPixelList)
        {
            auto [drawPoint, drawColor] = updatePixel;

            // Add to redraw list for tile
            //tile->redrawMap[drawColor].push_back(drawPoint);
            
        }

        ++row;
    }

}

void SDLScreen::watchScreenPower(ScreenPowerFlippedSlot watcher)
{
    d_powerFlippedSignal.connect(watcher);
}
