#define _CRT_SECURE_NO_DEPRECATE
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
//#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <math.h>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	//bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Set image dimensions
	void set_dimensions(int width_input, int height_input);

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

int search_in_array(int array[500], int delta, int number, int target);

//Create dot based on time 
bool create_dots(int time);

//Create map
bool create_map(bool column[4]);

//Point counter
void point_counter(bool column[4], int time);

void count_down();

bool start_game(bool is_click);
bool check_in_rect(int x, int y, int x_rect, int y_rect, int width, int height);

void menu_handle(bool is_click);

//Frees media and shuts down SDL
void close();

//SDL_Surface* loadSurface(std::string path);

//Main loop flag
bool quit = false;

//Event handler
SDL_Event event;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
//SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
//SDL_Surface* gHelloWorld = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
//TTF_Font* gFont = NULL;

//Scene textures
LTexture gDots_check;
LTexture gDots_check_reaction;
LTexture gDots;
LTexture gBackgroundTexture;
LTexture list_numbers[10];
LTexture gDancin_text;
LTexture gDancin_text_reaction;
LTexture easy_text;
LTexture easy_text_reaction;
LTexture medium_text;
LTexture medium_text_reaction;
LTexture hard_text;
LTexture hard_text_reaction;

LTexture menu_image;
LTexture menu_image_reaction;
LTexture quit_image;
LTexture quit_image_reaction;
LTexture resume_image;
LTexture resume_image_reaction;
LTexture play_again_image;
LTexture play_again_image_reaction;

Mix_Music* Music_list = NULL;
Mix_Music* clapping = NULL;

//The sound effects that will be used
Mix_Chunk* gScratch = NULL;
Mix_Chunk* gHigh = NULL;
Mix_Chunk* gMedium = NULL;
Mix_Chunk* gLow = NULL;

int time_appearce[500];
int column_appearce[500];
int number_of_times = 0;
int time_end = 0;
bool check_point[500];
int point_true = 0, point_false = 0, point_streak = 0;
int point = 0, point_count = 0;
int timer = 0;
int time_start_music = 0;
int game_speed = -1;
int music_chosen = -1;
int mouse_x, mouse_y;
bool is_menu_mode = false;
bool menu_temp = false;
int time_stop = -1;
int time_during_stop = 0;
bool is_start = false;
bool is_end = false;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

//bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
//{
//	//Get rid of preexisting texture
//	free();
//
//	//Render text surface
//	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
//	if (textSurface == NULL)
//	{
//		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
//	}
//	else
//	{
//		//Create texture from surface pixels
//		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
//		if (mTexture == NULL)
//		{
//			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
//		}
//		else
//		{
//			//Get image dimensions
//			mWidth = textSurface->w;
//			mHeight = textSurface->h;
//		}
//
//		//Get rid of old surface
//		SDL_FreeSurface(textSurface);
//	}
//
//	//Return success
//	return mTexture != NULL;
//}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::set_dimensions(int width_input, int height_input)
{
	mWidth = width_input;
	mHeight = height_input;
}

bool init()
{
	//Initialization flag
	bool success = true;
	
	//Set check point false
	for (int i = 0; i < 500; i++)
		check_point[i] = false;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Piano Tapping", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				
				//Initialize SDL_ttf
				/*if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}*/

			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load file data
	FILE* f_data;
	f_data = fopen("data\\dancin.data","r");

	if (!fscanf(f_data, "%d", &time_end))
	{
		printf("Failed to load file data");
		success = false;
	}

	while (!feof(f_data))
	{
		if (!fscanf(f_data, "%d", &time_appearce[number_of_times]))
		{
			printf("Failed to load file data");
			success = false;
		}
		number_of_times++;
	}
	if (feof(f_data))
		//printf("%d", number_of_time);
		fclose(f_data);

	//Load dot_check texture
	if (!gDots_check.loadFromFile("data\\game_dot_check.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load dot_check texture
	if (!gDots_check_reaction.loadFromFile("data\\game_dot_check_reaction.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load dot texture
	if (!gDots.loadFromFile("data\\game_dot.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load dancin text
	if (!gDancin_text.loadFromFile("data\\dancin_text.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load dancin text reaction
	if (!gDancin_text_reaction.loadFromFile("data\\dancin_text_reaction.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load easy text
	if (!easy_text.loadFromFile("data\\easy_text.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load easy text reaction
	if (!easy_text_reaction.loadFromFile("data\\easy_text_reaction.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load medium text
	if (!medium_text.loadFromFile("data\\medium_text.png"))
	{
		printf("Failed to load medium texture image!\n");
		success = false;
	}

	//Load medium text reaction
	if (!medium_text_reaction.loadFromFile("data\\medium_text_reaction.png"))
	{
		printf("Failed to load medium text reaction texture image!\n");
		success = false;
	}

	//Load hard text
	if (!hard_text.loadFromFile("data\\hard_text.png"))
	{
		printf("Failed to load hard texture image!\n");
		success = false;
	}

	//Load dancin text reaction
	if (!hard_text_reaction.loadFromFile("data\\hard_text_reaction.png"))
	{
		printf("Failed to load hard texture image!\n");
		success = false;
	}


	Music_list = Mix_LoadMUS("data\\dancin.mp3");
	if (Music_list == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	clapping = Mix_LoadMUS("data\\clapping.mp3");
	if (clapping == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load background texture
	/*if (!gBackgroundTexture.loadFromFile("data\\game_background.png"))
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}*/

	//Load menu
	if (!menu_image.loadFromFile("data\\menu.png"))
	{
		printf("Failed to load menu image!\n");
		success = false;
	}

	if (!menu_image_reaction.loadFromFile("data\\menu_reaction.png"))
	{
		printf("Failed to load menu image!\n");
		success = false;
	}

	menu_image.set_dimensions(menu_image.getWidth() / 2, menu_image.getHeight() / 2);
	menu_image_reaction.set_dimensions(menu_image_reaction.getWidth()/2, menu_image_reaction.getHeight()/2);

	//Load quit
	if (!quit_image.loadFromFile("data\\quit.png"))
	{
		printf("Failed to load menu image!\n");
		success = false;
	}

	if (!quit_image_reaction.loadFromFile("data\\quit_reaction.png"))
	{
		printf("Failed to load quit image!\n");
		success = false;
	}

	quit_image.set_dimensions(quit_image.getWidth() / 2, quit_image.getHeight() / 2);
	quit_image_reaction.set_dimensions(quit_image_reaction.getWidth() / 2, quit_image_reaction.getHeight() / 2);

	//Load play again
	if (!play_again_image.loadFromFile("data\\play_again.png"))
	{
		printf("Failed to load menu image!\n");
		success = false;
	}

	if (!play_again_image_reaction.loadFromFile("data\\play_again_reaction.png"))
	{
		printf("Failed to load quit image!\n");
		success = false;
	}

	play_again_image.set_dimensions(play_again_image.getWidth() / 2, play_again_image.getHeight() / 2);
	play_again_image_reaction.set_dimensions(play_again_image_reaction.getWidth() / 2, play_again_image_reaction.getHeight() / 2);

	//Load resume
	if (!resume_image.loadFromFile("data\\resume.png"))
	{
		printf("Failed to load resume image!\n");
		success = false;
	}

	if (!resume_image_reaction.loadFromFile("data\\resume_reaction.png"))
	{
		printf("Failed to load resume image!\n");
		success = false;
	}

	resume_image.set_dimensions(resume_image.getWidth() / 2, resume_image.getHeight() / 2);
	resume_image_reaction.set_dimensions(resume_image_reaction.getWidth() / 2, resume_image_reaction.getHeight() / 2);

	//Load number
	char file_image_number_head[20] = "data\\number_";
	char file_image_number_back[20] = ".png";
	char file_image_number[40] = "";

	for (int i = 0; i < 10; i++)
	{
		std::string tmp = std::to_string(i);
		char const* num_char = tmp.c_str();

		strcpy(file_image_number, file_image_number_head);
		strcat(file_image_number, num_char);
		strcat(file_image_number, file_image_number_back);

		//printf("%s \n", file_image_number);

		if (!list_numbers[i].loadFromFile(file_image_number))
		{
			printf("Failed to load number %d image!\n",i);
			success = false;
		}
	}
	
	//Creat dot in ramdom column
	srand(SDL_GetTicks());
	int min = 1, max = 39;
	for (int i = 0; i <= number_of_times; i++)
	{
		int random_column = (rand() % (max + 1 - min))/10 + 1;
		column_appearce[i] = random_column;
		//printf("%d\n", random_column);
	}
	
	//Open the font
	//gFont = TTF_OpenFont("16_true_type_fonts/lazy.ttf", 28);
	//if (gFont == NULL)
	//{
	//	printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	//	success = false;
	//}
	//else
	//{
	//	//Render text
	//	SDL_Color textColor = { 0, 0, 0 };
	//	if (!gTextTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
	//	{
	//		printf("Failed to render text texture!\n");
	//		success = false;
	//	}
	//}

	return success;
}

bool create_dots(int time)
{
	int size = 75;
	int distance = 14;
	long space = (SCREEN_WIDTH - (size * 4 + distance * 3)) / 2;
	gDots.set_dimensions(size, size);
	for (int i = 0; i < number_of_times; i++)
	{
		int location = (time - time_appearce[i]) * game_speed + (700);
		if (location <= 700)
			gDots.render((size + distance) * ((column_appearce[i]) - 1) + space, location);
	}
	//printf("%d\n", time);
	return true;
}

bool create_map(bool column[4])
{
	int size = 75;
	int distance = 10;
	long space = (SCREEN_WIDTH - (size * 4 + distance * 3)) / 2;
	gDots_check.set_dimensions(size, size);
	gDots_check_reaction.set_dimensions(size, size);
	for (int i = 0; i < 4; i++)
	{
		if (column[i])
			gDots_check_reaction.render((size + distance) * i + space, 700);
		else
			gDots_check.render((size + distance) * i + space, 700);
	}
	SDL_Rect fillRect = { (size + distance) * 0 + space - distance, 0, 6, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xAD, 0xAD, 0xAD, 0xFF);
	SDL_RenderFillRect(gRenderer, &fillRect);

	fillRect = { (size + distance) * 3 + space + size - 6 + distance, 0, 6, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xAD, 0xAD, 0xAD, 0xFF);
	SDL_RenderFillRect(gRenderer, &fillRect);

	int scale_down = 2;
	int Width_text = gDancin_text.getWidth() / scale_down;
	int Height_text = gDancin_text.getHeight() / scale_down;
	gDancin_text.set_dimensions(Width_text, Height_text);
	gDancin_text.render(SCREEN_WIDTH / 2 - Width_text / 2, 10);
	gDancin_text.set_dimensions(Width_text * scale_down, Height_text * scale_down);

	return true;
}

int search_in_array(int array[500], int delta, int number, int target)
{
	for (int i = 0; i <= number; i++)
	{
		if ((target >= (array[i] - delta)) and (target <= (array[i] + delta)))
		{
			return i;
		}
	}
	return -1;
}

void point_counter(bool column[4], int time)
{
	int delta = 500;
	int take_column = 0;
	int time_in_array = search_in_array(time_appearce, 200, number_of_times, time);
	if (time > 1000)
	{
		if ((time >= (time_appearce[time_in_array])) and (time <= (time_appearce[time_in_array] + delta)))
		{
			if (!check_point[time_in_array])
			{
				for (int i = 0; i < 4; i++)
				{
					if (column[i])
						take_column = i + 1;
				}
				if ((column_appearce[time_in_array] == take_column) and (column != 0))
				{
					check_point[time_in_array] = true;
					point_true++;
					point_streak++;
					if (point_streak > 50)
						point += 10 + 50 / 10;
					else
						point += 10 + point_streak / 10;
					//printf("+%d\n", point_true);
				}
				else if ((column_appearce[time_in_array] != take_column) and (column != 0))
				{
					check_point[time_in_array] = true;
					point_false++;
					point_streak = 0;
					//printf("-%d\n", point_false);
				}
			}
		}
		else if ((time >= (time_appearce[time_in_array] + delta)) and (time <= (time_appearce[time_in_array] + delta * 1.1)))
		{
			if (!check_point[time_in_array])
			{
				check_point[time_in_array] = true;
				point_false++;
				point_streak = 0;
				//printf("--%d\n", point_false);
			}
		}
	}

	if ((time_end - (timer - time_start_music) >= -100) and (time_end - (timer - time_start_music) <= 100))
	{
		is_end = true;
		Mix_PauseMusic();
		Mix_PlayMusic(clapping, 1);
	}
}

void show_point_streak()
{
	int number_of_point = 0;
	int test_point = 10;
	int point_array[3];
	int temp_point = point_streak;

	for (int i = 0; i < 3; i++)
	{
		point_array[i] = temp_point / pow(10, 2 - i);
		if (point_array[i] != 0 and number_of_point == 0)
			number_of_point = 3 - i;
		temp_point -= point_array[i] * pow(10, 2 - i);
	}
	//printf("number_of_point: %d\n", number_of_point);
	int half = 40 / 2 * (2 - number_of_point);
	//printf("half: %d\n", half);
	for (int i = 2; i > 2 - number_of_point; i--)
	{
		int number_show = i;
		//printf("number_show: %d\n", number_show);
		list_numbers[point_array[number_show]].render((40 * (number_show - 2)) + SCREEN_WIDTH / 2 - half, 100);
	}
}

void show_point()
{
	int number_of_point = 0;;
	int point_array[5];
	int temp_point = point_count;

	if (point_count < point)
		point_count += 1;
	for (int i = 0; i < 5; i++)
	{
		point_array[i] = temp_point / pow(10, 4 - i);
		if (point_array[i] != 0 and number_of_point == 0)
			number_of_point = 5 - i;
		temp_point -= point_array[i] * pow(10, 4 - i);
	}
	//printf("number_of_point: %d\n", number_of_point);
	int half = 40 * (1 - number_of_point);
	//printf("half: %d\n", half);
	for (int i = 4; i > 4 - number_of_point; i--)
	{
		int number_show = i;
		//printf("number_show: %d\n", number_show);
		list_numbers[point_array[number_show]].render(40 * (number_show-4) - half, 0);
	}
}

bool check_in_rect(int x, int y, int x_rect, int y_rect, int width, int height)
{
	if (x >= x_rect and x <= x_rect + width and y >= y_rect and y <= y_rect + height)
		return true;
	else
		return false;
}

void count_down()
{
	int delay = 1000;

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gRenderer);
	list_numbers[2].render(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 30);
	SDL_RenderPresent(gRenderer);
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(delay);

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gRenderer);
	list_numbers[1].render(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 30);
	SDL_RenderPresent(gRenderer);
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(delay);
}

bool start_game(bool is_click)
{
	bool is_finish = false;
	int scale_down = 2;

	point_true = 0;
	point_false = 0;
	point = 0;
	point_count = 0;
	point_streak = 0;
	for (int i = 0; i < 500; i++)
		check_point[i] = false;

	int Width_text_dancin = gDancin_text.getWidth() / scale_down;
	int Height_text_dancin = gDancin_text.getHeight() / scale_down;
	gDancin_text.set_dimensions(Width_text_dancin, Height_text_dancin);
	gDancin_text_reaction.set_dimensions(Width_text_dancin, Height_text_dancin);

	int Width_text_easy = easy_text.getWidth() / scale_down;
	int Height_text_easy = easy_text.getHeight() / scale_down;
	easy_text.set_dimensions(Width_text_easy, Height_text_easy);
	easy_text_reaction.set_dimensions(Width_text_easy, Height_text_easy);

	int Width_text_medium = medium_text.getWidth() / scale_down;
	int Height_text_medium = medium_text.getHeight() / scale_down;
	medium_text.set_dimensions(Width_text_medium, Height_text_medium);
	medium_text_reaction.set_dimensions(Width_text_medium, Height_text_medium);

	int Width_text_hard = hard_text.getWidth() / scale_down;
	int Height_text_hard = hard_text.getHeight() / scale_down;
	hard_text.set_dimensions(Width_text_hard, Height_text_hard);
	hard_text_reaction.set_dimensions(Width_text_hard, Height_text_hard);

	if (music_chosen == -1) 
	{
		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - Width_text_dancin / 2, SCREEN_HEIGHT / 2 - Height_text_dancin / 2, Width_text_dancin, Height_text_dancin))
		{
			gDancin_text_reaction.render(SCREEN_WIDTH / 2 - Width_text_dancin / 2, SCREEN_HEIGHT / 2 - Height_text_dancin / 2);
			if (is_click)
			{
				music_chosen = 0;
				SDL_Delay(200);
			}
			//printf("ok");
		}
		else
			gDancin_text.render(SCREEN_WIDTH / 2 - Width_text_dancin / 2, SCREEN_HEIGHT / 2 - Height_text_dancin / 2);
	}
	else if (game_speed == -1)
	{
		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - Width_text_easy / 2, (SCREEN_HEIGHT-150) / 2 - Height_text_easy / 2, Width_text_easy, Height_text_easy))
		{
			easy_text_reaction.render(SCREEN_WIDTH / 2 - Width_text_easy / 2, (SCREEN_HEIGHT - 150) / 2 - Height_text_easy / 2);
			if (is_click)
				game_speed = 1;
		}
		else
			easy_text.render(SCREEN_WIDTH / 2 - Width_text_easy / 2, (SCREEN_HEIGHT - 150) / 2 - Height_text_easy / 2);

		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - Width_text_medium / 2, SCREEN_HEIGHT / 2 - Height_text_medium / 2, Width_text_medium, Height_text_medium))
		{
			medium_text_reaction.render(SCREEN_WIDTH / 2 - Width_text_medium / 2, SCREEN_HEIGHT / 2 - Height_text_medium / 2);
			if (is_click)
				game_speed = 2;
		}
		else
			medium_text.render(SCREEN_WIDTH / 2 - Width_text_medium / 2, SCREEN_HEIGHT / 2 - Height_text_medium / 2);

		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - Width_text_hard / 2, (SCREEN_HEIGHT + 150) / 2 - Height_text_hard / 2, Width_text_hard, Height_text_hard))
		{
			hard_text_reaction.render(SCREEN_WIDTH / 2 - Width_text_hard / 2, (SCREEN_HEIGHT + 150) / 2 - Height_text_hard / 2);
			if (is_click)
				game_speed = 3;
		}
		else
			hard_text.render(SCREEN_WIDTH / 2 - Width_text_hard / 2, (SCREEN_HEIGHT + 150) / 2 - Height_text_hard / 2);
	}

	gDancin_text.set_dimensions(Width_text_dancin * scale_down, Height_text_dancin * scale_down);
	gDancin_text_reaction.set_dimensions(Width_text_dancin * scale_down, Height_text_dancin * scale_down);

	easy_text.set_dimensions(Width_text_easy * scale_down, Height_text_easy * scale_down);
	easy_text_reaction.set_dimensions(Width_text_easy * scale_down, Height_text_easy * scale_down);

	medium_text.set_dimensions(Width_text_medium * scale_down, Height_text_medium * scale_down);
	medium_text_reaction.set_dimensions(Width_text_medium * scale_down, Height_text_medium * scale_down);

	hard_text.set_dimensions(Width_text_hard * scale_down, Height_text_hard * scale_down);
	hard_text_reaction.set_dimensions(Width_text_hard * scale_down, Height_text_hard * scale_down);

	if (music_chosen != -1 and game_speed != -1)
	{
		count_down();
		Mix_PlayMusic(Music_list, 1);
		time_start_music = SDL_GetTicks();
		return true;
	}
	return false;
}

void play_music_again()
{
	Mix_PauseMusic();
	for (int i = 0; i < 500; i++)
		check_point[i] = false;
	point_true = 0;
	point_false = 0;
	point = 0;
	point_count = 0;
	point_streak = 0;
	count_down();
	Mix_PlayMusic(Music_list, 1);
	time_start_music = SDL_GetTicks();
}

void menu_handle(bool is_click)
{
	if (!is_menu_mode)
	{
		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH - menu_image.getWidth() - 10, 10, menu_image.getWidth(), menu_image.getHeight()))
		{
			menu_image_reaction.render(SCREEN_WIDTH - menu_image_reaction.getWidth() - 10, 10);
			if (is_click)
			{
				SDL_Delay(100);
				is_menu_mode = !is_menu_mode;
				time_start_music -= 100;
			}
		}
		else
			menu_image.render(SCREEN_WIDTH - menu_image.getWidth() - 10, 10);
	}

	if (is_menu_mode)
	{
		if (is_menu_mode != menu_temp)
		{
			menu_temp = is_menu_mode;
			//printf("menu mode\n");
			time_stop = timer;
			Mix_PauseMusic();
		}

		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - resume_image.getWidth() / 2, SCREEN_HEIGHT / 2 - resume_image.getHeight() / 2 - 100 - 50, resume_image.getWidth(), resume_image.getHeight()))
		{
			resume_image_reaction.render(SCREEN_WIDTH / 2 - resume_image.getWidth() / 2, SCREEN_HEIGHT / 2 - resume_image.getHeight() / 2 - 100 - 50);
			if (is_click)
			{
				is_menu_mode = !is_menu_mode;
			}
		}
		else
			resume_image.render(SCREEN_WIDTH / 2 - resume_image.getWidth() / 2, SCREEN_HEIGHT / 2 - resume_image.getHeight() / 2 - 100 - 50);

		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH/2 - play_again_image.getWidth()/2, SCREEN_HEIGHT/2 - play_again_image.getHeight()/2 - 50, play_again_image.getWidth(), play_again_image.getHeight()))
		{
			play_again_image_reaction.render(SCREEN_WIDTH / 2 - play_again_image.getWidth() / 2, SCREEN_HEIGHT / 2 - play_again_image.getHeight() / 2 - 50);
			if (is_click)
			{
				is_menu_mode = !is_menu_mode;
				menu_temp = is_menu_mode;
				//printf("play again\n");
				play_music_again();
			}
		}
		else
			play_again_image.render(SCREEN_WIDTH / 2 - play_again_image.getWidth() / 2, SCREEN_HEIGHT / 2 - play_again_image.getHeight() / 2 - 50);

		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - menu_image.getWidth() / 2, SCREEN_HEIGHT / 2 - menu_image.getHeight() / 2 + 50, menu_image.getWidth(), menu_image.getHeight()))
		{
			menu_image_reaction.render(SCREEN_WIDTH / 2 - menu_image.getWidth() / 2, SCREEN_HEIGHT / 2 - menu_image.getHeight() / 2 + 50);
			if (is_click)
			{
				is_menu_mode = !is_menu_mode;
				menu_temp = is_menu_mode;
				music_chosen = -1;
				game_speed = -1;
				is_start = false;
			}
		}
		else
			menu_image.render(SCREEN_WIDTH / 2 - menu_image.getWidth() / 2, SCREEN_HEIGHT / 2 - menu_image.getHeight() / 2 + 50);

		if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - quit_image.getWidth() / 2, SCREEN_HEIGHT / 2 - quit_image.getHeight() / 2 + 150, quit_image.getWidth(), quit_image.getHeight()))
		{
			quit_image_reaction.render(SCREEN_WIDTH / 2 - quit_image.getWidth() / 2, SCREEN_HEIGHT / 2 - quit_image.getHeight() / 2 + 150);
			if (is_click)
			{
				is_menu_mode = !is_menu_mode;
				menu_temp = is_menu_mode;
				quit = true;
			}
		}
		else
			quit_image.render(SCREEN_WIDTH / 2 - quit_image.getWidth() / 2, SCREEN_HEIGHT / 2 - quit_image.getHeight() / 2 + 150);

	}
	else
	{
		if (is_menu_mode != menu_temp)
		{
			menu_temp = is_menu_mode;
			time_during_stop = timer - time_stop;
			//printf("%d\n", time_during_stop);
			count_down();
			time_start_music += time_during_stop + 2000;
			Mix_ResumeMusic();
		}
	}
	
}

void end_handle(bool is_click)
{
	int number_of_point = 0;
	int test_point = 10;
	int point_array[5];
	int temp_point = point;

	for (int i = 0; i < 5; i++)
	{
		point_array[i] = temp_point / pow(10, 4 - i);
		if (point_array[i] != 0 and number_of_point == 0)
			number_of_point = 5 - i;
		temp_point -= point_array[i] * pow(10, 4 - i);
	}
	//printf("number_of_point: %d\n", number_of_point);
	for (int i = 0; i < 10; i++)
	{
		list_numbers[i].set_dimensions(list_numbers[i].getWidth() * 2, list_numbers[i].getHeight() * 2);
	}
	int half = list_numbers[0].getWidth() / 2 * (2 - number_of_point);
	//printf("half: %d\n", half);
	for (int i = 4; i > 4 - number_of_point; i--)
	{
		int number_show = i;
		//printf("number_show: %d\n", number_show);
		list_numbers[point_array[number_show]].render((list_numbers[i].getWidth() * (number_show - 4)) + SCREEN_WIDTH / 2 - half, 100);
	}

	for (int i = 0; i < 10; i++)
	{
		list_numbers[i].set_dimensions(list_numbers[i].getWidth() / 2, list_numbers[i].getHeight() / 2);
	}

	if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - play_again_image.getWidth() / 2, SCREEN_HEIGHT / 2 - play_again_image.getHeight() / 2 - 50, play_again_image.getWidth(), play_again_image.getHeight()))
	{
		play_again_image_reaction.render(SCREEN_WIDTH / 2 - play_again_image.getWidth() / 2, SCREEN_HEIGHT / 2 - play_again_image.getHeight() / 2 - 50);
		if (is_click)
		{
			is_end = false;
			//printf("play again\n");
			play_music_again();
		}
	}
	else
		play_again_image.render(SCREEN_WIDTH / 2 - play_again_image.getWidth() / 2, SCREEN_HEIGHT / 2 - play_again_image.getHeight() / 2 - 50);

	if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - menu_image.getWidth() / 2, SCREEN_HEIGHT / 2 - menu_image.getHeight() / 2 + 50, menu_image.getWidth(), menu_image.getHeight()))
	{
		menu_image_reaction.render(SCREEN_WIDTH / 2 - menu_image.getWidth() / 2, SCREEN_HEIGHT / 2 - menu_image.getHeight() / 2 + 50);
		if (is_click)
		{
			is_end = false;
			music_chosen = -1;
			game_speed = -1;
			is_start = false;
		}
	}
	else
		menu_image.render(SCREEN_WIDTH / 2 - menu_image.getWidth() / 2, SCREEN_HEIGHT / 2 - menu_image.getHeight() / 2 + 50);

	if (check_in_rect(mouse_x, mouse_y, SCREEN_WIDTH / 2 - quit_image.getWidth() / 2, SCREEN_HEIGHT / 2 - quit_image.getHeight() / 2 + 150, quit_image.getWidth(), quit_image.getHeight()))
	{
		quit_image_reaction.render(SCREEN_WIDTH / 2 - quit_image.getWidth() / 2, SCREEN_HEIGHT / 2 - quit_image.getHeight() / 2 + 150);
		if (is_click)
		{
			quit = true;
		}
	}
	else
		quit_image.render(SCREEN_WIDTH / 2 - quit_image.getWidth() / 2, SCREEN_HEIGHT / 2 - quit_image.getHeight() / 2 + 150);

	//menu_handle(true);
}

void time_line(int current_time)
{
	int point_show = (current_time*SCREEN_WIDTH)/time_end;
	//printf("%d-", point_show);
	SDL_Rect fillRect = {0, SCREEN_HEIGHT - 6, point_show, 6 };
	SDL_SetRenderDrawColor(gRenderer, 0xAD, 0xAD, 0xAD, 0xFF);
	SDL_RenderFillRect(gRenderer, &fillRect);
}

void close()
{
	//Deallocate surface
	//SDL_FreeSurface(gHelloWorld);
	//gHelloWorld = NULL;
	gDots_check.free();
	gBackgroundTexture.free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	const int FPS = 100;
	const int DELAY_TIME = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;
	bool colum_input[4];
	bool is_click = false;
	for (int i = 0; i < 4; i++)
		colum_input[i] = false;
	/*SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;*/
	
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			return 0;
		}
		while (!quit)
		{
			Mix_Volume(-1, 0);
			timer = SDL_GetTicks();
			frameStart = SDL_GetTicks();
			SDL_GetMouseState(&mouse_x, &mouse_y);
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							if (is_start)
								is_menu_mode = !is_menu_mode;
							break;

						case SDLK_d:
							colum_input[0] = true;
							break;

						case SDLK_f:
							colum_input[1] = true;
							break;

						case SDLK_j:
							colum_input[2] = true;
							break;

						case SDLK_k:
							colum_input[3] = true;
							break;

						case SDLK_p:
							is_end = true;
							Mix_PauseMusic();
							Mix_PlayMusic(clapping, 1);
							break;

						default:
							printf("");
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_d:
						colum_input[0] = false;
						break;

					case SDLK_f:
						colum_input[1] = false;
						break;

					case SDLK_j:
						colum_input[2] = false;
						break;

					case SDLK_k:
						colum_input[3] = false;
						break;
					}
				}
				else if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					is_click = true;
				}
				else if (event.type == SDL_MOUSEBUTTONUP)
				{
					is_click = false;
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(gRenderer);
			//gBackgroundTexture.render(0, 0);
			if (time_start_music != 0 and is_start)
			{
				if (!is_end)
				//if (false)
				{
					if (!is_menu_mode) 
					{
						create_map(colum_input);
						create_dots(timer - time_start_music);
						point_counter(colum_input, timer - time_start_music);
						time_line(timer - time_start_music);
						show_point_streak();
						show_point();
						//printf("%d\n", timer - time_start_music);
					
					}
					menu_handle(is_click);
				}
				else
				{
					end_handle(is_click);
				}
			}
			else
				is_start = start_game(is_click);

			//Render background texture to screen
			//Render Foo' to the screen
			//gFooTexture.render(a, b);
			//Update screen
			SDL_RenderPresent(gRenderer);
			SDL_UpdateWindowSurface(gWindow);
			// FPS
			frameTime = SDL_GetTicks() - frameStart;
			if (frameTime < DELAY_TIME)
			{
				SDL_Delay(DELAY_TIME - frameTime);
			}
		}
	}
	close();
	return 0;
}