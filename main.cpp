#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

SDL_Texture* LoadBMP(SDL_Renderer* renderer, const char* file)
{
	SDL_Texture* tex = nullptr;
	SDL_Surface* surf = IMG_Load(file);
	if (surf != nullptr)
	{
		tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
	}

	return tex;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;
	static const int CAMERA_HEIGHT = 25;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window* window = SDL_CreateWindow("3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	bool running = true;

	float lookX = 0;
	float lookY = CAMERA_HEIGHT;
	float lookZ = -1000;
	float lookDepth = .84;
	//float lookAngleX = 0;
	float lookAngleY = 0;

	int input[6] = { 0, 0, 0, 0, 0 };

	float gravity = .06f;
	float yvel = 0;

	bool jumping = false;

	SDL_Rect sky = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	SDL_Rect grass = { 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT };

	int px[1000];
	int height[1000];
	int pz[1000];

	for (int i = 0; i < 1000; i++)
	{
		px[i] = (-50 + rand() % 100 + 10)*20;
		pz[i] = (i/10)*20;
		height[i] = rand() % 3 + 2;
	}

	std::vector<SDL_Texture*>trees;
	SDL_Rect treepos[20];

	for (int i = 0; i < 20; i++)
	{
		trees.push_back(LoadBMP(renderer, "tree.png"));
	}

	SDL_Texture* tree = LoadBMP(renderer, "tree.png");

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
					break;
				case SDLK_e:
					input[5] = 1;
					break;
				case SDLK_SPACE:
					if (!jumping)
					{
						yvel = 1.8;
						jumping = true;
					}
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
					//lookAngleX -= 1;
					break;
				case SDLK_e:
					input[5] = 0;
					//lookAngleX += 1;
					break;
				}
				break;
			}
		}

		if (input[0] == 1) lookZ += 1;
		if (input[1] == 1) lookZ -= 1;
		if (input[2] == 1) lookX -= 1;
		if (input[3] == 1) lookX += 1;
		if (input[4] == 1) lookAngleY -= 1;
		if (input[5] == 1) lookAngleY += 1;

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
		grass.h = WINDOW_HEIGHT*4;

		SDL_SetRenderDrawColor(renderer, 40, 150, 40, 255);
		SDL_RenderFillRect(renderer, &grass);

		for (int i = 0; i < 1000; i++)
		{
			float scale = lookDepth / (-pz[i] - lookZ);
			float x = (1 + scale * (-px[i] - lookX)) *	 WINDOW_WIDTH / 2;
			float y = (1 - scale * (0 - lookY)) * WINDOW_HEIGHT / 2;

			SDL_SetRenderDrawColor(renderer, 100, 255, 100,  100);

			if (y >= WINDOW_HEIGHT / 2 && y <= WINDOW_HEIGHT && x >= 0 && x <= WINDOW_WIDTH)
			{
				SDL_RenderDrawLine(renderer, x, y, x, y - (height[i]));
			}
		}

		for (int i = 0; i < 20; i++)
		{
			scale = lookDepth / (-pz[i*50] - lookZ);
			float x = (1 + scale * (-px[i*50] - lookX)) * WINDOW_WIDTH / 2;
			float y = (1 - scale * (0 - lookY)) * WINDOW_HEIGHT / 2;

			treepos[i].x = x;
			treepos[i].y = y;
			treepos[i].w = 50000 * scale; treepos[i].h = 50000 * scale;
			treepos[i].y -= treepos[i].h;
			SDL_RenderCopy(renderer, trees[i], NULL, &treepos[i]);
		}

		SDL_RenderPresent(renderer);
	}

	for (auto it : trees)
		SDL_DestroyTexture(it);

	SDL_DestroyTexture(tree);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
