#include "raylib.h"
#include <stdio.h>

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float update_time;
	float running_time;
};

int main()
{

	//window demensions
	int windowDemensions[2];
	windowDemensions[0] = 1280; //screen width
	windowDemensions[1] = 720;	//screen height

	//game window
	const int screen_width{1280};
	const int screen_height{720};
	InitWindow(windowDemensions[0], screen_height, "Dapper Dasher");

	//for double jump prevention
	bool isinAir{false};

	//acceleration due to graveity (pixels per second /per second)
	const int rec_gravity{1500};

	//(SCARFY) Texture for player spritesheet
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	//(SCARFY) velocity for movment
	int rec_jump_velocity{-900}; //pixles per second
	int rec_current_velocity{0};

	//(SCARFY) scarfy animation data
	AnimData scarfy_data;
	scarfy_data.rec.width = scarfy.width / 6;
	scarfy_data.rec.height = scarfy.height;
	scarfy_data.rec.x = 0;
	scarfy_data.rec.y = 0;
	scarfy_data.pos.x = screen_width / 8 - scarfy_data.rec.width / 2;
	scarfy_data.pos.y = screen_height / 2 - scarfy_data.rec.height;
	scarfy_data.frame = 0;
	scarfy_data.update_time = 1.0f / 12.0f;
	scarfy_data.running_time = 0.0;

	//(NEBULAS) Texture for nebulas
	Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");
	//(NEBULAS)nebula x velocity (Pixels per second)
	int nebVel{-600};
	//(NEBULAS) data for our nebulas
	AnimData neb_data{
		{0.0f, 0.0f, nebula.width / 8.0f, nebula.height / 8.0f}, // Rectangle rec
		{screen_width, screen_height - nebula.height / 8.0f},	 //  Vector2 pos
		0,														 // int frame
		1.0f / 12.0f,											 // float Update Time
		0.0f													 // float Running time
	};
	AnimData neb_2_data{
		{0.0f, 0.0f, nebula.width / 8.0f, nebula.height / 8.0f},	// Rectangle rec
		{screen_width + 300, screen_height - nebula.height / 8.0f}, //  Vector2 pos
		0,															// int frame
		1.0f / 16.0f,												// float Update Time
		0.0f														// float Running time
	};
	//(CORE) Game loop----------------------------------------------------------------------------------------------
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//get Delta Time
		const float dt{GetFrameTime()};
		scarfy_data.running_time += dt;

		//Drawing-------------------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		//(SCARFY) Ground check & apply gravity----------------------------------------------------------------
		if (scarfy_data.pos.y >= screen_height - scarfy_data.rec.height)
		{
			if (scarfy_data.pos.y > screen_height - scarfy_data.rec.height)
			{
				//push the player up the amount they passed the bottom of the screen
				scarfy_data.pos.y -= scarfy_data.pos.y - (screen_height - scarfy_data.rec.height);
			}

			//reset velocity while on the ground
			rec_current_velocity = 0;
			isinAir = false;
		}
		else
		{
			//apply gravity whilst in air
			rec_current_velocity += rec_gravity * dt;
			isinAir = true;

			//player will hit ceiling
			if (scarfy_data.pos.y <= 0)
			{
				float amountOver{scarfy_data.pos.y};
				scarfy_data.pos.y -= amountOver;
			}
		}
		//---------------------------------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_SPACE) && isinAir == false)
		{
			rec_current_velocity += rec_jump_velocity;
		}

		//update nebula position
		neb_data.pos.x += nebVel * dt;
		if (neb_data.pos.x <= 0 - neb_data.rec.width)
		{
			neb_data.pos.x = screen_width + neb_data.rec.width;
		}

		//update second nebula position
		neb_2_data.pos.x += nebVel * dt;
		if (neb_2_data.pos.x <= 0 - neb_2_data.rec.width)
		{
			neb_2_data.pos.x = screen_width + neb_2_data.rec.width;
		}

		//animate nebula
		neb_data.running_time += dt;
		if (neb_data.running_time >= neb_data.update_time)
		{
			neb_data.running_time = 0.0f;
			neb_data.rec.x = neb_data.frame * neb_data.rec.width;
			neb_data.frame++;
			if (neb_data.frame > 8)
			{
				neb_data.frame = 0;
			}
		}
		//animate second nebula
		neb_2_data.running_time += dt;
		if (neb_2_data.running_time >= neb_2_data.update_time)
		{
			neb_2_data.running_time = 0.0f;
			neb_2_data.rec.x = neb_2_data.frame * neb_2_data.rec.width;
			neb_2_data.frame++;
			if (neb_2_data.frame > 8)
			{
				neb_2_data.frame = 0;
			}
		}

		//update player position
		scarfy_data.pos.y += rec_current_velocity * dt;

		//Update player animation
		scarfy_data.rec.x = scarfy_data.frame * scarfy_data.rec.width;

		//animate the player
		if (scarfy_data.running_time >= scarfy_data.update_time && !isinAir)
		{
			scarfy_data.running_time = 0.0f;
			scarfy_data.frame++;
			if (scarfy_data.frame > 5)
			{
				scarfy_data.frame = 0;
			}
		}
		else if (isinAir)
		{
			//freeze player animation to first one when jumpint
			scarfy_data.frame = 0;
		}

		//draw nebula
		DrawTextureRec(nebula, neb_data.rec, neb_data.pos, WHITE);

		//draw second nebula
		DrawTextureRec(nebula, neb_2_data.rec, neb_2_data.pos, RED);

		//draw player
		DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);

		EndDrawing();
	}
	CloseWindow();
	//unload our player texture
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	return 0;
}