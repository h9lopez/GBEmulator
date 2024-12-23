#if !defined(GB_SCREEN_LAYER_INCLUDED)
#define GB_SCREEN_LAYER_INCLUDED

#include <gb_screen_api.h>
#include <gb_screen_displaygriditem.h>
#include <utils/addressrange.h>

class Layer
{
public:
    typedef std::vector< std::vector<DisplayGridItem*> > GridLayoutTable;

private:
    GBScreenAPI::RenderLayer d_displayLayer;
    bool d_isActive;
    GBScreenAPI::TileDataRegionInfo d_dataRegionInfo;
    GridLayoutTable d_layoutTable;
    bool d_isUpdated; // dirty flag

public:
    Layer();
    Layer(GBScreenAPI::RenderLayer layer, bool active);
    
    void updateSourceRegionInfo(const GBScreenAPI::TileDataRegionInfo& info);
    void loadNewTileAt(int x, int y, unsigned int tileNum);
    GridLayoutTable::const_iterator layoutGridBegin() const;
    GridLayoutTable::const_iterator layoutGridEnd() const;
    std::pair<size_t, size_t> layoutGridWidthHeight() const;
    DisplayGridItem* layoutGridAt(int x, int y) const;

    ~Layer();
};

Layer::Layer()
    : d_displayLayer(GBScreenAPI::RenderLayer::UNDEFINED), d_isActive(false)
{}

Layer::~Layer()
{
}



#endif // GB_SCREEN_LAYER_INCLUDED
