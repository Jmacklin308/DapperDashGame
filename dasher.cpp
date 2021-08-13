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
	//game window
	const int screen_width{1280};
	const int screen_height{720};
	InitWindow(screen_width, screen_height, "Dapper Dasher");

	//rectangle
	int rec_jump_velocity{-900}; //pixles per second
	int rec_current_velocity{0};

	//acceleration due to graveity (pixels per second /per second)
	const int rec_gravity{1500};

	//Texture for player spritesheet
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfy_data;
	scarfy_data.rec.width = scarfy.width / 6;
	scarfy_data.rec.height = scarfy.height;
	scarfy_data.rec.x = 0;
	scarfy_data.rec.y = 0;
	scarfy_data.pos.x = screen_width / 2 - scarfy_data.rec.width / 2;
	scarfy_data.pos.y = screen_height / 2 - scarfy_data.rec.height;
	scarfy_data.frame = 0;
	scarfy_data.update_time = 1.0f / 12.0f;
	scarfy_data.running_time = 0.0;

	// Rectangle scarfy_rec;
	// scarfy_rec.width = scarfy.width / 6; // 6 images in spritesheet
	// scarfy_rec.height = scarfy.height;
	// scarfy_rec.x = 0;
	// scarfy_rec.y = 0;

	// //player animation
	// int frame{};
	// float anim_speed{1.3f};
	// const float update_time{anim_speed / 12.0f};
	// float running_time{};

	// //player position
	// Vector2 scarfy_pos;
	// scarfy_pos.x = screen_width / 8 - scarfy_rec.width / 2; //center
	// scarfy_pos.y = screen_height - scarfy_rec.height;

	//for double jump prevention
	bool isinAir{false};

	//Texture for nebulas
	Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");

	//anim data for nebulas
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

	// Rectangle nebula_rec{0.0f, 0.0f, nebula.width / 8.f, nebula.height / 8.f};
	// Vector2 nebula_pos{screen_width, screen_height - nebula_rec.height};

	// Rectangle neb_2_Rec{0.0f, 0.0f, nebula.width / 8.f, nebula.height / 8.f};
	// Vector2 neb_2_pos{screen_width + 300, screen_height - nebula_rec.height};

	//nebula x velocity (Pixels per second)
	int nebVel{-600};

	//nebula animation variables
	int neb_frame{};
	const float neb_update_time{1.0f / 12.0f};
	float neb_running_time{};

	int neb_2_frame{};
	const float neb_2_update_time{1.0f / 16.0f};
	float neb_2_running_time{};

	//-------------------------------------------------------------------------------------

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//get Delta Time
		const float dt{GetFrameTime()};
		scarfy_data.running_time += dt;

		//Drawing-------------------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		//Ground check & apply gravity----------------------------------------------------------------
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
		//-------------------------------------------------------------------------------------------

		//input check
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

		//draw nebula
		DrawTextureRec(nebula, neb_data.rec, neb_data.pos, WHITE);

		//draw second nebula
		DrawTextureRec(nebula, neb_2_data.rec, neb_2_data.pos, RED);

		//draw player
		DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);

		EndDrawing();
		//----------------------------------
	}
	CloseWindow();
	//unload our player texture
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	return 0;
}