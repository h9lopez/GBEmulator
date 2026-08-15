#include <SDL2/SDL_render.h>
#include <gb_sdl_screen.h>
#include <gb_screen_api.h>

#include <array>
#include <SDL2/SDL.h>
#include <boost/log/trivial.hpp>
#include <gb_typeutils.h>
#include <addressrange.h>
#include <boost/range/iterator_range.hpp>

namespace {
    static const AddressRange UPPER_TILEMAP_ADDR_RANGE(0x9800, 0x9BFF);
    static const AddressRange LOWER_TILEMAP_ADDR_RANGE(0x9C00, 0x9FFF);

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
    static constexpr GBScreenAPI::GBScreenPixelValue _bitPairMap[2][2] = {
        { GBScreenAPI::GBScreenPixelValue::OFF , GBScreenAPI::GBScreenPixelValue::MEDIUM},
        { GBScreenAPI::GBScreenPixelValue::LOW, GBScreenAPI::GBScreenPixelValue::HIGH}
    };

    return _bitPairMap[bit1][bit2];
}

SDLScreen::SDLScreen(RAM* ram, SDL_Window* window, DisplayPalette palette)
    : d_ram(ram), d_sdlWindow(window), d_colorPalette(palette)
{
    SDL_Color_Comp cmp;
    d_redrawMap = std::map<SDL_Color, std::vector<SDL_Point>, SDL_Color_Comp>(cmp);
    d_sdlRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(d_sdlWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE), SDL_DestroyRenderer);
}

void SDLScreen::drawScreen() const
{
    if (d_sdlRenderer) {
        SDL_RenderPresent(d_sdlRenderer.get());
    }
}

namespace {
std::array<std::tuple<SDL_Point, SDL_Color>, 8> decodeGBTileRow(WordType word, int rowNum, const DisplayPalette& palette)
{
    std::array<std::tuple<SDL_Point, SDL_Color>, 8> pixelList;

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
        pixelList[i] = std::make_tuple(p, gbPixelColor);

        // Next pair
        hi = hi >> 1;
        lo = lo >> 1;
        ++x;
    }
    return pixelList;
}
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
        .addressingMode = (d_lcdcState.tileDataSelect.start == 0x8000) ? GBScreenAPI::TileDataAddressingMode::UNSIGNED_MODE : GBScreenAPI::TileDataAddressingMode::SIGNED_MODE
    }; 
    std::get<1>(background) = newTileDataRegion;
    std::get<1>(window) = newTileDataRegion;

    d_renderTargets[GBScreenAPI::RenderLayerType::BACKGROUND] = background;
    d_renderTargets[GBScreenAPI::RenderLayerType::WINDOW] = window;
}



void SDLScreen::processBTTUpdate(Address addr, RAM::SegmentUpdateData data)
{
    BOOST_LOG_TRIVIAL(debug) << "BTT update at address " << std::hex << addr;
}

void SDLScreen::watchScreenPower(ScreenPowerFlippedSlot watcher)
{
    d_powerFlippedSignal.connect(watcher);
}
