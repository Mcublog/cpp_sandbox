/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-08-23
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <chrono>
#include <cstdlib>
#include <thread>

#include <SDL2/SDL.h>
//>>---------------------- Log control
#define LOG_MODULE_NAME dispemu
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Renderer *renderer = NULL;

#ifdef __cplusplus
extern "C"
{
#endif

    int window_start(uint32_t w, uint32_t h)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            LOG_ERROR("SDL_Init != 0");
            return -1;

        }

        // SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

        if (window == NULL || renderer == NULL)
        {
            LOG_ERROR("SDL_CreateWindowAndRenderer != 0");
            return -2;
        }

        static constexpr uint8_t kFillColor = 0xFF;
        SDL_SetRenderDrawColor(renderer, kFillColor, kFillColor, kFillColor, kFillColor);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        return 0;
    }

    void window_DrawPx(uint32_t x, uint32_t y, uint8_t color)
    {
        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void window_render()
    {
        SDL_RenderPresent(renderer);
    }

    void window_destroy()
    {
        LOG_INFO("destoy");
        // SDL_DestroyRenderer(renderer);
        // SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void window_put_pixel(uint32_t x, uint32_t y, uint8_t color)
    {
        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }

#ifdef __cplusplus
}
#endif

int main(void)
{
    LOG_INFO("hello");

    int r = atexit(window_destroy);
    if(r != 0)
        LOG_ERROR("atexit != 0");

    constexpr int kXSizeMax = 640;
    constexpr int kYSizeMax = 200;

    window_start(kXSizeMax, kYSizeMax);

    for(auto x = 0; x < kXSizeMax; x++)
    {
        window_put_pixel(x, (kYSizeMax >> 1) + 0, 0x00);
        window_put_pixel(x, (kYSizeMax >> 1) + 1, 0x00);
        window_put_pixel(x, (kYSizeMax >> 1) + 2, 0x00);
    }
    window_render();

    bool running = true;
    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;

            default:
                break;
            }
        }
    }

    return 0;
}
