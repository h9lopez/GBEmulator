#include <gb_screen_layer.h>
#include <gb_typeutils.h>

Layer::Layer(GBScreenAPI::RenderLayer layer, bool active, RAM* ram)
    : d_displayLayer(layer), d_isActive(active), d_ram(ram)
{
    // Create our tile table
    d_layoutTable.resize(GB_TILETABLE_HEIGHT);
    for (auto i = 0; i < GB_TILETABLE_HEIGHT; ++i) {
        d_layoutTable[i].resize(GB_TILETABLE_WIDTH);
    }
}

Layer::Layer()
    : d_displayLayer(GBScreenAPI::RenderLayer::UNDEFINED), d_isActive(false), d_ram()
{}

Layer::~Layer()
{
}

Layer::GridLayoutTable::const_iterator Layer::layoutGridEnd() const
{
    return d_layoutTable.begin();
}

Layer::GridLayoutTable::const_iterator Layer::layoutGridBegin() const
{
    return d_layoutTable.begin();
}

std::pair<size_t, size_t> Layer::layoutGridWidthHeight() const
{
    return std::make_pair(GB_TILETABLE_WIDTH, GB_TILETABLE_HEIGHT);
}

void Layer::updateSourceRegionInfo(const GBScreenAPI::TileDataRegionInfo& info)
{
    d_dataRegionInfo = info;

    // Update all of the underlying display tiles if they need to be.
    // Either:
    //  - If data region range changes, we need to reassign all of the sourceRange for the tiles
    //  - If ingress is changed, that signifies switching to a new data map. This is unlikely but may come up
    //  - If addressingMode is changed, that also signifies switching to a new data map. Unlikely.
    
    // Now that we have a know source data range, we can create our layout table
    int x,y = 0;
    for (auto i = d_dataRegionInfo.range.start; i <= d_dataRegionInfo.range.end; i+=sizeof(ByteType))
    {
        auto tileNum = d_ram->readByte(i);
        auto tile = (d_layoutTable[x][y]->tile) ? d_layoutTable[x][y]->tile : new DisplayTile();
        
        tile->tileReferenceNum = tileNum;
        tile->sourceRange.start = info.ingress + (tileNum * 16);
        tile->sourceRange.end = tile->sourceRange.start + 16;
    }
}

