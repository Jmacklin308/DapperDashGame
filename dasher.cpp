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
	float windowDemensions[2]{};
	windowDemensions[0] = 1280; //screen width
	windowDemensions[1] = 720;	//screen height

	//game window
	InitWindow(windowDemensions[0], windowDemensions[1], "Dapper Dasher");

	//(SCARFY) for double jump prevention
	bool isinAir{false};

	//gravity (pixels per second /per second)
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
	scarfy_data.pos.x = windowDemensions[0] / 8 - scarfy_data.rec.width / 2;
	scarfy_data.pos.y = windowDemensions[1] / 2 - scarfy_data.rec.height;
	scarfy_data.frame = 0;
	scarfy_data.update_time = 1.0f / 12.0f;
	scarfy_data.running_time = 0.0;

	//(NEBULAS) Texture for nebulas
	Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");
	//(NEBULAS)nebula x velocity (Pixels per second)
	int nebVel{-600};

	//initialize nebula array
	AnimData nebulae[3]{};

	// Assign data to each nebula uppon creation
	for (int i = 0; i < 3; i++)
	{

		//create the rectangle for each nebula
		nebulae[i].rec.x = 0.0f;
		nebulae[i].rec.y = 0.0f;
		nebulae[i].rec.width = nebula.width / 8.0f;
		nebulae[i].rec.height = nebula.width / 8.0f;

		//create the vector for each nebula
		nebulae[i].pos.y = windowDemensions[1] - nebula.height / 8.0f;

		//create the frame for each nebula
		nebulae[i].frame = 0;

		//create the update time for each nebula
		nebulae[i].update_time = 1.0f / 16.f;

		//create the running time for each nebula
		nebulae[i].running_time = 0.0f;
	}

	//manually set nebula data
	nebulae[0].pos.x = windowDemensions[0];
	nebulae[1].pos.x = windowDemensions[0] + 300;
	nebulae[2].pos.x = windowDemensions[0] + 500;

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
		if (scarfy_data.pos.y >= windowDemensions[1] - scarfy_data.rec.height)
		{
			if (scarfy_data.pos.y > windowDemensions[1] - scarfy_data.rec.height)
			{
				//push the player up the amount they passed the bottom of the screen
				scarfy_data.pos.y -= scarfy_data.pos.y - (windowDemensions[1] - scarfy_data.rec.height);
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
		//(Scarfy) input checking
		//---------------------------------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_SPACE) && isinAir == false)
		{
			rec_current_velocity += rec_jump_velocity;
		}
		//---------------------------------------------------------------------------------------------------------

		//-----------------------------------------------------------------------------------------------------------
		//Nebula position and animtaion handleing
		//-----------------------------------------------------------------------------------------------------------
		for (int i = 0; i < 3; i++)
		{
			//update nebula positon
			nebulae[i].pos.x += nebVel * dt;
			//reset if nebula goes out of bounds
			if (nebulae[i].pos.x <= 0 - nebulae[i].rec.width)
			{
				nebulae[i].pos.x = windowDemensions[0] + nebulae[i].rec.width;
			}

			//Animate each nebula nebula
			nebulae[i].running_time += dt;
			if (nebulae[i].running_time >= nebulae[i].update_time)
			{
				nebulae[i].running_time = 0.0f;
				nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
				nebulae[i].frame++;
				if (nebulae[i].frame > 8)
				{
					nebulae[i].frame = 0;
				}
			}

			//draw nebula
			DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
		}
		//-----------------------------------------------------------------------------------------------------------
		//(scarfy) Update, Animate, and draw
		//-----------------------------------------------------------------------------------------------------------
		scarfy_data.pos.y += rec_current_velocity * dt;

		//Update player animation
		scarfy_data.rec.x = scarfy_data.frame * scarfy_data.rec.width;

		//animate if not in air, freeze on first frame otherwise
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

		//draw scarfy
		DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);
		//-----------------------------------------------------------------------------------------------------------

		EndDrawing();
	}
	CloseWindow();
	//unload our player texture
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	return 0;
}