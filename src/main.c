#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL3/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define SCALE 200

typedef struct {
    double x, y, z;
} Point;

typedef struct {
    int rows;
    int cols;
    double** data;
} Matrix;

Point make_point(double x, double y, double z) {
    Point p = { x, y, z };
    return p;
}

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        m.data[i] = calloc(cols, sizeof(double));
    }
    return m;
}

void free_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->data[i]);
    }
    free(m->data);
}

Matrix dot(Matrix a, Matrix b) {
    Matrix result = create_matrix(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            for (int k = 0; k < b.rows; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

Point transform(Matrix m, Point p) {
    Matrix vec = create_matrix(3, 1);
    vec.data[0][0] = p.x;
    vec.data[1][0] = p.y;
    vec.data[2][0] = p.z;

    Matrix result = dot(m, vec);
    Point out = make_point(result.data[0][0], result.data[1][0], result.data[2][0]);

    free_matrix(&vec);
    free_matrix(&result);
    return out;
}

Point translate(Point shift, Point p) {
    return make_point(p.x + shift.x, p.y + shift.y, p.z + shift.z);
}

void connect(SDL_Renderer* renderer, Point* points, int i, int j) {
    SDL_RenderLine(renderer, points[i].x, points[i].y, points[j].x, points[j].y);
}

Matrix get_rotation_matrix() {
    double alpha = 0.001;
    Matrix rx = create_matrix(3, 3);
    rx.data[0][0] = 1;
    rx.data[1][1] = cos(alpha);
    rx.data[1][2] = -sin(alpha);
    rx.data[2][1] = sin(alpha);
    rx.data[2][2] = cos(alpha);

    double beta = 0.002;
    Matrix ry = create_matrix(3, 3);
    ry.data[0][0] = cos(beta);
    ry.data[0][2] = sin(beta);
    ry.data[1][1] = 1;
    ry.data[2][0] = -sin(beta);
    ry.data[2][2] = cos(beta);

    double gamma = 0.003;
    Matrix rz = create_matrix(3, 3);
    rz.data[0][0] = cos(gamma);
    rz.data[0][1] = -sin(gamma);
    rz.data[1][0] = sin(gamma);
    rz.data[1][1] = cos(gamma);
    rz.data[2][2] = 1;

    Matrix tmp = dot(ry, rx);
    Matrix result = dot(rz, tmp);

    free_matrix(&rx);
    free_matrix(&ry);
    free_matrix(&rz);
    free_matrix(&tmp);

    return result;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer("GAME", WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
        printf("SDL Window/Renderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Point points[4] = {
        make_point(1, 1, 1),     // Vertex A
        make_point(-1, -1, 1),   // Vertex B
        make_point(-1, 1, -1),   // Vertex C
        make_point(1, -1, -1)    // Vertex D
    };    
    
    Point screenShift = make_point(WIDTH / 2, HEIGHT / 2, 0);
    Point screenShiftOpposite = make_point(-WIDTH / 2, -HEIGHT / 2, 0);

    for (int i = 0; i < 8; i++) {
        points[i].x = SCALE * points[i].x + screenShift.x;
        points[i].y = SCALE * points[i].y + screenShift.y;
        points[i].z = SCALE * points[i].z + screenShift.z;
    }

    Matrix rotationXYZ = get_rotation_matrix();

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        for (int i = 0; i < 8; i++) {
            points[i] = translate(screenShiftOpposite, points[i]);
            points[i] = transform(rotationXYZ, points[i]);
            points[i] = translate(screenShift, points[i]);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
 
        int edges[6][2] = {
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 2},
            {1, 3},
            {2, 3}
        };

        for (int i = 0; i < 6; i++) {
            connect(renderer, points, edges[i][0], edges[i][1]);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(3);
    }

    free_matrix(&rotationXYZ);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

