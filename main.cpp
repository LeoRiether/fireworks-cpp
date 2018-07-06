#ifndef SDL_h_
#include <SDL.h>
#endif

#ifndef CMATH
#define CMATH
#include <cmath>
#endif

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "particles.cpp"

#define WIDTH 960
#define HEIGHT 720
#define PARTICLE_COLORS_SIZE 10
double dt = 1.0/40.0;

bool quit;


int randint(int min, int max) {
  return rand() % (max-min) + min;
}

float randfloat(float min, float max) {
  return ((float)rand()/(float)RAND_MAX) * (max-min) + min;
}

int randsign() {
  return rand() & 1 ? 1 : -1;
}

uint32_t colors[] = { 0xFF0000, 0x00FF00, 0x4891ea, 0xFF00FF, 0x00FFFF, 0xFFFF00, 0xFFFFFF, 0xd8700f, 0x4c10ef, 0x15d89e };

void handle_event(SDL_Event* event) {
  switch (event->type)
  {
    case SDL_QUIT:
      quit = true;
      break;
    default:
      break;
  }
}

void update(std::vector<Particle>& p) {
  bool elementErased = false;
  std::vector<Particle>::size_type size = p.size();
  for(std::vector<Particle>::size_type i = 0; i < size; ) { 
    if (p[i].fuse == -0xFF) { ++i; continue; } // why even bother
    p[i].update(dt);
    if (p[i].fuse <= 0) {
      if (p[i].explodes) {
        // Explode the firework
        p[i].fuse = -0xFF; // Set the fuse to -0xFF if it already exploded
        int n = randint(6, 17); // Number of exploded particles
        float v = randfloat(250, 400);// Explosion velocity
        float fuse = randfloat(1.25, 1.5);
        float t0 = randfloat(0, 2.0*M_PI); // Angle of the first firework
        float step = 2.0f*M_PI/(float)n; // How much the angle should increase at every iteration
        for (float t = t0; t < 2*M_PI+t0; t += step) {
          p.push_back(Particle(
            p[i].x, p[i].y,
            v*cos(t), v*sin(t),
            fuse,
            p[i].color, false
          ));
          size++; // safer I guess
        }
      } else {
        // elementErased = true;
        // p.erase(p.begin()+i);
        // size--;
        p[i].fuse = -0xFF;
      }
    }

    if(!elementErased) ++i;
  }

  // meh
  if (p.size() > 950) p.clear();

}

void draw(SDL_Renderer* r, std::vector<Particle>& p) {
  SDL_SetRenderDrawColor(r, 0,0,0,0xFF);
  SDL_RenderClear(r);
  for(Particle& p0 : p) p0.draw(r);
  SDL_RenderPresent(r);
}

int main(int argc, char* args[])
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
  SDL_Renderer* renderer = NULL;
  quit = false;
  srand(time(NULL));

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    quit = true;
  } else {
		window = SDL_CreateWindow( "SDL Fireworks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
		if (window == NULL || renderer == NULL) {
			std::cout << "Window/Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      quit = true;
    } else {
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

  auto particles = std::vector<Particle>();

  SDL_Event* event;
  uint32_t currentTime = 0, lastTime;
  uint32_t fireworkTimer = 0, fireworkDelay = 0;
  while (!quit) {
    update(particles);
    draw(renderer, particles);

    while (SDL_PollEvent(event)) 
      handle_event(event);

    currentTime = SDL_GetTicks();
    dt = (currentTime-lastTime)/1000.0;
    lastTime = currentTime;

    if (currentTime > fireworkTimer + fireworkDelay) {
      particles.push_back(Particle(
        randint(50, WIDTH-50), HEIGHT,
        randfloat(-75, 75), randfloat(-600, -375),
        randfloat(1.5, 3),
        0xFF0000, true
      ));
      particles.back().setColor(colors + randint(0, PARTICLE_COLORS_SIZE));
      fireworkTimer = currentTime;
      fireworkDelay = randint(600, 1000);
    }

    SDL_Delay(10);
  }

  SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
