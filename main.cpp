#include <SDL.h>

#include <ctime>
#include <stdlib.h>
#include <stdio.h>

struct Vector
{
	float x;
	float y;
	float z;

	Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

int main(int argc, char* argv[])
{
	srand(time(NULL));

	static const int WINDOW_WIDTH = 640;
	static const int WINDOW_HEIGHT = 480;
	static const int CAMERA_HEIGHT = 50;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	bool running = true;

	float lookX = 0;
	float lookY = CAMERA_HEIGHT;
	float lookZ = -100;
	float lookDepth = .3;
	float lookAngle = 0;

	int input[6] = { 0, 0, 0, 0, 0 };

	float gravity = .06f;
	float yvel;

	bool jumping = false;

	SDL_Rect sky = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	SDL_Rect grass = { 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT };

	int px[10000];
	int height[10000];
	int pz[10000];

	for (int i = 0; i < 10000; i++)
	{
		px[i] = -200 + rand() % 400 + 1;
		pz[i] = rand() % 100 + 1;
		height[i] = rand() % 5 + 1;
	}

	while (running)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					input[0] = 1;
					break;
				case SDLK_DOWN:
					input[1] = 1;
					break;
				case SDLK_LEFT:
					input[2] = 1;
					break;
				case SDLK_RIGHT:
					input[3] = 1;
					break;
				case SDLK_q:
					input[4] = 1;
					//lookAngle -= 1;
					break;
				case SDLK_e:
					input[5] = 1;
					//lookAngle += 1;
					break;
				case SDLK_SPACE:
					yvel = 1.8;
					jumping = true;
					break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					input[0] = 0;
					break;
				case SDLK_DOWN:
					input[1] = 0;
					break;
				case SDLK_LEFT:
					input[2] = 0;
					break;
				case SDLK_RIGHT:
					input[3] = 0;
					break;
				case SDLK_q:
					input[4] = 0;
					//lookAngle -= 1;
					break;
				case SDLK_e:
					input[5] = 0;
					//lookAngle += 1;
					break;
				}
				break;
			}
		}

		if (input[0] == 1) lookZ += .5;
		if (input[1] == 1) lookZ -= .5;
		if (input[2] == 1) lookX -= 2.0;
		if (input[3] == 1) lookX += 2.0;
		if (input[4] == 1) lookAngle -= 1;
		if (input[5] == 1) lookAngle += 1;

		if (jumping == true)
		{
			lookY += yvel;
			yvel -= gravity;
		}

		if (lookY < CAMERA_HEIGHT)
		{
			lookY = CAMERA_HEIGHT;
			jumping = false;
		}

		float scale = lookDepth / (0 - lookZ);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		sky.h = (1 - scale * (0 - lookY)) * WINDOW_HEIGHT / 2;

		SDL_SetRenderDrawColor(renderer, 80, 80, 200, 255);
		SDL_RenderFillRect(renderer, &sky);

		grass.y = (1 - scale * (0 - lookY)) * WINDOW_HEIGHT / 2;

		SDL_SetRenderDrawColor(renderer, 40, 150, 40, 255);
		SDL_RenderFillRect(renderer, &grass);

		for (int i = 0; i < 10000; i++)
		{
			float scale = lookDepth / (-pz[i] - lookZ);
			float x = (1 + scale * (-px[i] - lookX)) * WINDOW_WIDTH / 2;
			float y = (1 - scale * (0 - lookY)) * WINDOW_HEIGHT / 2;

			SDL_SetRenderDrawColor(renderer, 100, 255, 100,  100);

			if (y >= WINDOW_HEIGHT / 2 && y <= WINDOW_HEIGHT && x >= 0 && x <= WINDOW_WIDTH)
			{
				SDL_RenderDrawLine(renderer, x, y, x, y - (height[i]));
			}
		}
		SDL_RenderPresent(renderer);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}