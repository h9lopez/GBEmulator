#if !defined(DISPLAYCORE_TILE_INCLUDED_H)
#define DISPLAYCORE_TILE_INCLUDED_H

#include <utils/gb_typeutils.h>
#include <addressrange.h>
#include <boost/signals2.hpp>

namespace DisplayCore
{
    class Tile
    {
    private:
        AddressRange d_tileDataRange;
        int d_positionX;
        int d_positionY;
        
    public:
     
        Tile(int x, int y); 

        void refreshTile(Address addr, SegmentUpdateData data);
        ~Tile();
    };
    
    Tile::Tile(/* args */)
    {
    }
    
    Tile::~Tile()
    {
    }
    
} // namespace DisplayCore


#endif // DISPLAYCORE_TILE_INCLUDED_H
