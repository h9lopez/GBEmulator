#if !defined(GB_SCREEN_LAYEREVENTMANAGER_INCLUDED)
#define GB_SCREEN_LAYEREVENTMANAGER_INCLUDED

#include <gb_screen_layer.h>

class LayerEventManager
{
private:
    std::shared_ptr<Layer> d_managedLayer;
public:
    typedef std::shared_ptr<boost::signals2::signal<void (Address, SegmentUpdateData)> > LayerDataEventSignal;
    typedef std::shared_ptr<boost::signals2::slot<void (Address, SegmentUpdateData)> > LayerDataEventSlot;
    LayerEventManager(std::shared_ptr<Layer> layer);

    ~LayerEventManager();
};

LayerEventManager::LayerEventManager(std::shared_ptr<Layer> layer)
{
}

LayerEventManager::~LayerEventManager()
{
}



#endif // GB_SCREEN_LAYEREVENTMANAGER_INCLUDED
