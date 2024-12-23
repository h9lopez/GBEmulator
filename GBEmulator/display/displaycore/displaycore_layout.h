#if !defined(DISPLAYCORE_LAYOUT_INCLUDED)
#define DISPLAYCORE_LAYOUT_INCLUDED

#include <displaycore_tile.h>

namespace DisplayCore
{
    class Layout
    {
    public:
        typedef std::vector< std::vector< std::shared_ptr<Tile> > > LayoutContainer;

    private:
        LayoutContainer d_container;

    public:
        Layout(int width, int height);
        LayoutContainer::iterator begin();
        LayoutContainer::iterator end();
        std::shared_ptr<Tile> at(int x, int y) const;
        std::pair<int, int> widthHeight() const;
    };
    
} // namespace DisplayCore



#endif // DISPLAYCORE_LAYOUT_INCLUDED
