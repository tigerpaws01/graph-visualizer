#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define SCROLL_DAMPER 0.1f
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <graph/node.hpp>
#include <drawer.hpp>
#include <camera.hpp>
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
GV::Drawer drawer;
GV::Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

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
    // x = -300 ~ 300
    // y = -220 ~ 220
    for (int i = 0; i < NUM_NODES; i++) {
        int x = (rand() % 600) - 300;
        int y = (rand() % 440) - 220;
        nodes.push_back(GV::Node(i, x, y));
    }
    // randomly connect them
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            if (rand() % 2 == 0) {
                nodes[i].appendFanout(nodes[j]);
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
        // If mouse down: obtain relative position & move camera accordingly
        auto mouseEvent = (SDL_MouseMotionEvent*)event;
        if (mouseEvent->state == SDL_BUTTON_LEFT) {
            camera.center().x -= mouseEvent->xrel / camera.size();
            camera.center().y -= mouseEvent->yrel / camera.size();
        }
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        auto mouseEvent = (SDL_MouseWheelEvent*)event;
        camera.size() -= mouseEvent->y * SCROLL_DAMPER * camera.size();
        camera.size() = std::min(10.0f, std::max(0.05f, camera.size()));
    } /*else if (event->type == SDL_EVENT_KEY_DOWN) {
        if (((SDL_KeyboardEvent*)event)->key == SDLK_RIGHT) {
            camera.center().x += 2.0f / camera.size();
        } else if (((SDL_KeyboardEvent*)event)->key == SDLK_LEFT) {
            camera.center().x -= 2.0f / camera.size();
        } else if (((SDL_KeyboardEvent*)event)->key == SDLK_UP) {
            camera.center().y -= 2.0f / camera.size();
        } else if (((SDL_KeyboardEvent*)event)->key == SDLK_DOWN) {
            camera.center().y += 2.0f / camera.size();
        } 
        std::cout << "keydown " << ((SDL_KeyboardEvent*)event)->key << "\n";
    } else if (event->type == SDL_EVENT_KEY_UP) {
        std::cout << "keyup " << ((SDL_KeyboardEvent*)event)->key << "\n";
    }*/

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */
    SDL_RenderClear(renderer);

    // draw lines
    drawer.drawConnections(nodes, camera);

    for (const auto& node : nodes) {
        drawer.drawNode(node, camera);
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
            // if (!connections[i][j]) continue; // TODO: attraction works on connected pairs
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
