#include <gb_screen_layerrenderer.h>

LayerRenderer::LayerRenderer(std::shared_ptr<Layer> layer, std::shared_ptr<SDL_Renderer> renderObj)
    : d_layer(layer), d_sdlRenderer(renderObj)
{
    // Iterate through the existing layout format and create SDL_Texture 
    for (auto it = d_layer->layoutGridBegin(); it != d_layer->layoutGridEnd(); it++)
    {
        //it->tile->texture = SDL_
    }
    
}

LayerRenderer::LayerRenderer()
    : d_layer(), d_sdlRenderer()
{
}

LayerRenderer::~LayerRenderer()
{
}


