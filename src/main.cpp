/* clear.c ... */

/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <graph/node.hpp>
#include <drawer.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_NODES 10

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

std::vector<GV::Node> nodes;
std::vector<std::vector<bool>> connections;
GV::Drawer drawer;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    srand(time(NULL));
    // generate 5 nodes
    for (int i = 0; i < NUM_NODES; i++) {
        int x = (rand() % 560) + 40;
        int y = (rand() % 400) + 40;
        nodes.push_back(GV::Node(x, y));
    }
    // randomly connect them
    connections = std::vector<std::vector<bool>>(NUM_NODES, std::vector<bool>(NUM_NODES, false));
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            if (rand() % 2 == 0) {
                connections[i][j] = true;
            }
        }
    }
    drawer = GV::Drawer(renderer);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        std::cout << "motion\n";
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        std::cout << "down\n";
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        std::cout << "up\n";
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    /*const float red = (float) (0.5 + 0.5 * SDL_sin(now));
    const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));*/
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */
    SDL_RenderClear(renderer);

    // draw lines
    SDL_SetRenderDrawColor(renderer, 255.0, 255.0, 255.0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            if (!connections[i][j]) continue;
            SDL_FPoint pos_i = {nodes[i].x(), nodes[i].y()};
            SDL_FPoint pos_j = {nodes[j].x(), nodes[j].y()};
            SDL_RenderLine(renderer, pos_i.x, pos_i.y, pos_j.x, pos_j.y);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    for (const auto& node : nodes) {
        drawer.drawNode(node);
    }

    // Apply Forces
    // 1. Repelling forces between all pairs
    float repel_rate = 0.00005f * 4;
    float social_distance = 200.0f;
    float attraction_distance = 250.0f;
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            SDL_FPoint pos_i = {nodes[i].x(), nodes[i].y()};
            SDL_FPoint pos_j = {nodes[j].x(), nodes[j].y()};
            float distance = std::sqrt(
                             (pos_i.x - pos_j.x) * (pos_i.x - pos_j.x)
                           + (pos_i.y - pos_j.y) * (pos_i.y - pos_j.y));
            if (distance > social_distance) continue;
            // Compute directions for i and j
            std::pair<float, float> dir_i;
            {
                dir_i.first = (pos_i.x - pos_j.x) / distance;
                dir_i.second = (pos_i.y - pos_j.y) / distance;
                dir_i.first *= (social_distance - distance) * repel_rate;
                dir_i.second *= (social_distance - distance) * repel_rate;
            }
            std::pair<float, float> dir_j;
            {
                dir_j.first = (pos_j.x - pos_i.x) / distance;
                dir_j.second = (pos_j.y - pos_i.y) / distance;
                dir_j.first *= (social_distance - distance) * repel_rate;
                dir_j.second *= (social_distance - distance) * repel_rate;
            }
            // std::cout << "(i, j) = (" << i << ", " << j << "), (" 
            //           << nodes[i].x() << ", " << nodes[i].y() << ") -- (" << nodes[j].x() << ", " << nodes[j].y() << ")\n";
            nodes[i].x() += dir_i.first;
            nodes[i].y() += dir_i.second;
            nodes[j].x() += dir_j.first;
            nodes[j].y() += dir_j.second;
            // std::cout << "\t(" 
            //           << nodes[i].x() << ", " << nodes[i].y() << ") -- (" << nodes[j].x() << ", " << nodes[j].y() << ")\n";
        }
    }

    // 2. Attracting forces between connected pairs.
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            if (!connections[i][j]) continue;
            SDL_FPoint pos_i = {nodes[i].x(), nodes[i].y()};
            SDL_FPoint pos_j = {nodes[j].x(), nodes[j].y()};
            float distance = std::sqrt(
                             (pos_i.x - pos_j.x) * (pos_i.x - pos_j.x)
                           + (pos_i.y - pos_j.y) * (pos_i.y - pos_j.y));
            if (distance < attraction_distance) continue;
            // Compute directions for i and j
            std::pair<float, float> dir_i;
            {
                dir_i.first = (pos_i.x - pos_j.x) / distance;
                dir_i.second = (pos_i.y - pos_j.y) / distance;
                dir_i.first *= (attraction_distance - distance) * repel_rate;
                dir_i.second *= (attraction_distance - distance) * repel_rate;
            }
            std::pair<float, float> dir_j;
            {
                dir_j.first = (pos_j.x - pos_i.x) / distance;
                dir_j.second = (pos_j.y - pos_i.y) / distance;
                dir_j.first *= (attraction_distance - distance) * repel_rate;
                dir_j.second *= (attraction_distance - distance) * repel_rate;
            }
            // std::cout << "(i, j) = (" << i << ", " << j << "), (" 
            //           << nodes[i].x() << ", " << nodes[i].y() << ") -- (" << nodes[j].x() << ", " << nodes[j].y() << ")\n";
            nodes[i].x() += dir_i.first;
            nodes[i].y() += dir_i.second;
            nodes[j].x() += dir_j.first;
            nodes[j].y() += dir_j.second;
            // std::cout << "\t(" 
            //           << nodes[i].x() << ", " << nodes[i].y() << ") -- (" << nodes[j].x() << ", " << nodes[j].y() << ")\n";
        }
    }

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
