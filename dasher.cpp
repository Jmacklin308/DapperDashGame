#include "raylib.h"
#include <stdio.h>

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
	Rectangle scarfy_rec;
	scarfy_rec.width = scarfy.width / 6; // 6 images in spritesheet
	scarfy_rec.height = scarfy.height;
	scarfy_rec.x = 0;
	scarfy_rec.y = 0;

	//player animation
	int frame{};
	float anim_speed{1.3f};
	const float update_time{anim_speed / 12.0f};
	float running_time{};

	//player position
	Vector2 scarfy_pos;
	scarfy_pos.x = screen_width / 8 - scarfy_rec.width / 2; //center
	scarfy_pos.y = screen_height - scarfy_rec.height;

	//for double jump prevention
	bool isinAir{false};

	//Texture for NEBULA
	Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");
	Rectangle nebula_rec{0.0f, 0.0f, nebula.width / 8.f, nebula.height / 8.f};
	Vector2 nebula_pos{screen_width, screen_height - nebula_rec.height};

	Rectangle neb_2_Rec{0.0f, 0.0f, nebula.width / 8.f, nebula.height / 8.f};
	Vector2 neb_2_pos{screen_width + 300, screen_height - nebula_rec.height};

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
		running_time += dt;

		//Drawing-------------------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		//Ground check & apply gravity----------------------------------------------------------------
		if (scarfy_pos.y >= screen_height - scarfy_rec.height)
		{
			if (scarfy_pos.y > screen_height - scarfy_rec.height)
			{
				//push the player up the amount they passed the bottom of the screen
				scarfy_pos.y -= scarfy_pos.y - (screen_height - scarfy_rec.height);
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
			if (scarfy_pos.y <= 0)
			{
				float amountOver{scarfy_pos.y};
				scarfy_pos.y -= amountOver;
			}
		}
		//-------------------------------------------------------------------------------------------

		//input check
		if (IsKeyPressed(KEY_SPACE) && isinAir == false)
		{
			rec_current_velocity += rec_jump_velocity;
		}

		//update nebula position
		nebula_pos.x += nebVel * dt;
		if (nebula_pos.x <= 0 - nebula_rec.width)
		{
			nebula_pos.x = screen_width + nebula_rec.width;
		}

		//update second nebula position
		neb_2_pos.x += nebVel * dt;
		if (neb_2_pos.x <= 0 - neb_2_Rec.width)
		{
			neb_2_pos.x = screen_width + neb_2_Rec.width;
		}

		//update player position
		scarfy_pos.y += rec_current_velocity * dt;

		//Update player animation
		scarfy_rec.x = frame * scarfy_rec.width;

		//animate the player
		if (running_time >= update_time && !isinAir)
		{
			running_time = 0.0f;
			frame++;
			if (frame > 5)
			{
				frame = 0;
			}
		}
		else if (isinAir)
		{
			//freeze player animation to first one when jumpint
			frame = 0;
		}

		//animate nebula
		neb_running_time += dt;
		if (neb_running_time >= neb_update_time)
		{
			neb_running_time = 0.0f;
			nebula_rec.x = neb_frame * nebula_rec.width;
			neb_frame++;
			if (neb_frame > 8)
			{
				neb_frame = 0;
			}
		}
		//animate second nebula
		neb_2_running_time += dt;
		if (neb_2_running_time >= neb_2_update_time)
		{
			neb_2_running_time = 0.0f;
			neb_2_Rec.x = neb_2_frame * neb_2_Rec.width;
			neb_2_frame++;
			if (neb_2_frame > 8)
			{
				neb_2_frame = 0;
			}
		}

		//draw nebula
		DrawTextureRec(nebula, nebula_rec, nebula_pos, WHITE);

		//draw second nebula
		DrawTextureRec(nebula, neb_2_Rec, neb_2_pos, RED);

		//draw player
		DrawTextureRec(scarfy, scarfy_rec, scarfy_pos, WHITE);

		EndDrawing();
		//----------------------------------
	}
	CloseWindow();
	//unload our player texture
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	return 0;
}