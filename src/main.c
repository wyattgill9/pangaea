
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_PARTICLES 5000
#define PARTICLE_RADIUS 5
#define GRAVITY 0.5
#define BOUNCE 0.8
#define FRICTION 0.99

typedef struct {
    double x, y;        // Position
    double vx, vy;      // Velocity
    double mass;        // Mass
    int r, g, b;        // Color
} Particle;

Particle particles[MAX_PARTICLES];
int num_particles = 0;

void init_particle(Particle* p, double x, double y) {
    p->x = x;
    p->y = y;
    p->vx = (rand() % 200 - 100) / 100.0;  // Random initial velocity
    p->vy = (rand() % 200 - 100) / 100.0;
    p->mass = 1.0;
    p->r = rand() % 255;
    p->g = rand() % 255;
    p->b = rand() % 255;
}

void update_particle(Particle* p) {
    // Apply gravity
    p->vy += GRAVITY;
    
    // Update position
    p->x += p->vx;
    p->y += p->vy;
    
    // Apply friction
    p->vx *= FRICTION;
    p->vy *= FRICTION;
    
    // Handle collisions with walls
    if (p->x < PARTICLE_RADIUS) {
        p->x = PARTICLE_RADIUS;
        p->vx = -p->vx * BOUNCE;
    }
    if (p->x > WINDOW_WIDTH - PARTICLE_RADIUS) {
        p->x = WINDOW_WIDTH - PARTICLE_RADIUS;
        p->vx = -p->vx * BOUNCE;
    }
    if (p->y < PARTICLE_RADIUS) {
        p->y = PARTICLE_RADIUS;
        p->vy = -p->vy * BOUNCE;
    }
    if (p->y > WINDOW_HEIGHT - PARTICLE_RADIUS) {
        p->y = WINDOW_HEIGHT - PARTICLE_RADIUS;
        p->vy = -p->vy * BOUNCE;
    }
}

void handle_particle_collisions() {
    for (int i = 0; i < num_particles; i++) {
        for (int j = i + 1; j < num_particles; j++) {
            Particle* p1 = &particles[i];
            Particle* p2 = &particles[j];
            
            double dx = p2->x - p1->x;
            double dy = p2->y - p1->y;
            double distance = sqrt(dx*dx + dy*dy);
            
            if (distance < 2 * PARTICLE_RADIUS) {
                // Collision detected
                double angle = atan2(dy, dx);
                double sin_a = sin(angle);
                double cos_a = cos(angle);
                
                // Rotate velocities
                double vx1 = p1->vx * cos_a + p1->vy * sin_a;
                double vy1 = -p1->vx * sin_a + p1->vy * cos_a;
                double vx2 = p2->vx * cos_a + p2->vy * sin_a;
                double vy2 = -p2->vx * sin_a + p2->vy * cos_a;
                
                // Elastic collision
                double m1 = p1->mass;
                double m2 = p2->mass;
                double v1 = vx1;
                double v2 = vx2;
                
                vx1 = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
                vx2 = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
                
                // Rotate back
                p1->vx = vx1 * cos_a - vy1 * sin_a;
                p1->vy = vx1 * sin_a + vy1 * cos_a;
                p2->vx = vx2 * cos_a - vy2 * sin_a;
                p2->vy = vx2 * sin_a + vy2 * cos_a;
                
                // Move particles apart
                double overlap = 2 * PARTICLE_RADIUS - distance;
                double move_x = (overlap * cos_a) / 2;
                double move_y = (overlap * sin_a) / 2;
                
                p1->x -= move_x;
                p1->y -= move_y;
                p2->x += move_x;
                p2->y += move_y;
            }
        }
    }
}

void render_particles(SDL_Renderer* renderer) {
    for (int i = 0; i < num_particles; i++) {
        Particle* p = &particles[i];
        SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255);

        for (int dx = -PARTICLE_RADIUS; dx <= PARTICLE_RADIUS; dx++) {
            for (int dy = -PARTICLE_RADIUS; dy <= PARTICLE_RADIUS; dy++) {
                if (dx*dx + dy*dy <= PARTICLE_RADIUS*PARTICLE_RADIUS) {
                    SDL_RenderPoint(renderer, p->x + dx, p->y + dy);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Particle Simulator",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize random seed
    srand(time(NULL));
    
    // Create initial particles
    for (int i = 0; i < 50; i++) {
        if (num_particles < MAX_PARTICLES) {
            init_particle(&particles[num_particles],
                         rand() % (WINDOW_WIDTH - 2*PARTICLE_RADIUS) + PARTICLE_RADIUS,
                         rand() % (WINDOW_HEIGHT - 2*PARTICLE_RADIUS) + PARTICLE_RADIUS);
            num_particles++;
        }
    }

    int quit = 0;
    SDL_Event e;
    int mouse_x = 0, mouse_y = 0;
    int is_dragging = 0;
    Uint32 last_particle_time = 0;
    const Uint32 particle_delay = 16;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = 1;
            } else if (e.type == SDL_EVENT_MOUSE_MOTION) {
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;

                if (is_dragging && num_particles < MAX_PARTICLES) {
                    Uint32 current_time = SDL_GetTicks();
                    if (current_time - last_particle_time >= particle_delay) {
                        
                        for (int i = 0; i < 3; i++) { 
                            if (num_particles < MAX_PARTICLES) {
                                int offset_x = (rand() % 11) - 5; 
                                int offset_y = (rand() % 11) - 5;
                                init_particle(&particles[num_particles], 
                                            mouse_x + offset_x, 
                                            mouse_y + offset_y);
                                num_particles++;
                            }
                        }
                        last_particle_time = current_time;
                    }
                }
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    is_dragging = 1;
                    if (num_particles < MAX_PARTICLES) {
                        init_particle(&particles[num_particles], mouse_x, mouse_y);
                        num_particles++;
                    }
                }
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    is_dragging = 0;
                }
            }
        }

        for (int i = 0; i < num_particles; i++) {
            update_particle(&particles[i]);
        }

        handle_particle_collisions();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_particles(renderer);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16); // Cap at ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
