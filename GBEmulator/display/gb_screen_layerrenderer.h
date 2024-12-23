#ifndef GB_SCREEN_LAYERRENDERER_INCLUDED
#define GB_SCREEN_LAYERRENDERER_INCLUDED

#include <display/gb_screen_layer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class LayerRenderer
{
private:
    /* data */
    std::shared_ptr<Layer> d_layer;
    std::shared_ptr<SDL_Renderer> d_sdlRenderer;

public:
    LayerRenderer(std::shared_ptr<Layer> layer, std::shared_ptr<SDL_Renderer> renderObj);
    LayerRenderer();
    ~LayerRenderer();
};

#endif // GB_SCREEN_LAYERRENDERER_INCLUDED
