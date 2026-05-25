#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 900
#define HEIGHT 600
#define START_RADIUS 20
#define START_COLOR 0xff0000

#define TARGET_FPS 60
#define COLOR_RECT_SIZE 40

Uint32 color = START_COLOR;
Uint32 color_palette[] = {0x000000, 0xffffff, 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff};
const int color_palette_size = sizeof(color_palette)/4;

bool inside_color_palette(int x, int y)
{
    return x <= color_palette_size*COLOR_RECT_SIZE && y <= COLOR_RECT_SIZE;
}

// check if user clicked color palette and update color if so,
Uint32 check_color_palette_chosen(int x, int y)
{
    int i;
    if (inside_color_palette(x, y))
    {
        // mouse is inside x,y range of color palette
        i = x / COLOR_RECT_SIZE;
        color = color_palette[i];
    }
}

// draws the color palette consisting of size elements of colors
void draw_palette(SDL_Surface *surface, Uint32 *colors, int size)
{
    SDL_Rect color_rect;
    for (int i = 0; i < size; i++)
    {
        color_rect = (struct SDL_Rect) {i*COLOR_RECT_SIZE, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE};
        SDL_FillRect(surface, &color_rect, colors[i]);
    }
}


// draws a circle at center coordinates with given radius and color
void draw_circle(SDL_Surface *surface, int x_center, int y_center, int radius, Uint32 color)
{
    SDL_Rect pixel = {0, 0, 1, 1};
    for (int x = x_center - radius; x < x_center + radius; x++)
    {
        for (int y = y_center - radius; y < y_center + radius; y++)
        {
            // is this pixel part of the circle?
            int distance_from_center = sqrt(pow(x - x_center, 2) + pow(y - y_center, 2));
            if (distance_from_center < radius)
            {
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

int main()
{
    bool running = true;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Paint App",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    float delay_ms = (1.0 / TARGET_FPS) * 1000;

    bool draw = false;
    int x, y;
    int brush_size = START_RADIUS;

    draw_palette(surface, color_palette, color_palette_size);
    SDL_UpdateWindowSurface(window);

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.motion.x;
                    y = event.motion.y;
                    check_color_palette_chosen(x, y);
                    if (!inside_color_palette(x, y))
                        draw = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    draw = false;
                    break;
                case SDL_MOUSEWHEEL:                    
                    brush_size += event.wheel.preciseY;
                    if (brush_size < 2)
                        brush_size = 2;
                    if (brush_size > 100)
                        brush_size = 100;
                    break;
            }

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }

        }

        if (draw)
        {
            draw_circle(surface, x, y, brush_size, color);
            SDL_UpdateWindowSurface(window);
        }
        draw_palette(surface, color_palette, color_palette_size);
        SDL_Delay(delay_ms);
    }

    return 0;
}

/*
for future updates:
    git add .
    git commit -m "describe changes"
    git push
*/
