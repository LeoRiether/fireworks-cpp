#ifndef SDL_h_
#include <SDL.h>
#endif

#ifndef CMATH
#define CMATH
#include <cmath>
#endif

#include <algorithm>

#define G 220
#define PARTICLE_SIZE 2 

class Particle {
private:
  float vx, vy;
  SDL_Rect rect;

public:  
  float fuse;
  bool explodes;
  uint8_t* color;
  float x, y;
  Particle(int x, int y, float vx, float vy, float fuse, uint32_t color, bool explodes) : 
    Particle(x,y,vx,vy,fuse,reinterpret_cast<uint8_t*>(&color),explodes) { }

  Particle(int x, int y, float vx, float vy, float fuse, uint8_t* color, bool explodes) {
    this->x = x; this->y = y;
    this->vx = vx; this->vy = vy;
    this->fuse = fuse;
    this->explodes = explodes;
    this->color = color; 

    this->rect = SDL_Rect();
    this->rect.x = x; this->rect.y = y;
    this->rect.w = this->rect.h = PARTICLE_SIZE;
  }

  void update(const float dt) {
    this->fuse -= dt;

    if (this->explodes)
      this->vy += G*dt;
    else {
      this->vy += G*dt;
      // this->vy /= 92*dt;
      // this->vx /= 92*dt;
      this->vy *= .98;
      this->vx *= .98;
      // if (sqrt(this->vy*this->vy+this->vx*this->vx) <= 100) this->fuse = -0xFF;
    }

    this->x += this->vx*dt;
    this->y += this->vy*dt;


    this->rect.x = this->x; this->rect.y = this->y;
  }

  void draw(SDL_Renderer* r) {
    if (this->fuse == -0xFF) return;
    SDL_SetRenderDrawColor(r, this->color[2], this->color[1], this->color[0], 0xFF);
    SDL_RenderFillRect(r, &this->rect);
  }

  void setColor(uint32_t* color) {
    // uint8_t* c = reinterpret_cast<uint8_t*>(color);
    // std::copy(c, c+2, this->color);
    this->color = reinterpret_cast<uint8_t*>(color);
  }
  
};
