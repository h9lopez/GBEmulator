#include <gb_screen_layerrenderer.h>
#include <SDL2/SDL.h>

LayerRenderer::LayerRenderer()
{

}

LayerRenderer::~LayerRenderer()
{

}

LayerRenderer::LayerRenderer(std::shared_ptr<Layer> layer, std::shared_ptr<SDL_Renderer> renderObj)
    : d_layer(layer), d_sdlRenderer(renderObj)
{
    // Iterate through the existing layout format and create SDL_Texture 
    for (auto colIt = d_layer->layoutGridBegin(); colIt != d_layer->layoutGridEnd(); colIt++)
    {
        for (auto it = colIt->begin(); it != colIt->end(); it++)
        {
            (*it)->tile->texture = SDL_CreateTexture(d_sdlRenderer.get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GB_TILE_PIXEL_WIDTH, GB_TILE_PIXEL_HEIGHT);
        }
    }
}

