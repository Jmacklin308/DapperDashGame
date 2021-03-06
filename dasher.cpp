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

AnimData updateAnimData(AnimData data, float dt, int maxFrame)
{
	data.running_time += dt;
	if (data.running_time >= data.update_time)
	{
		data.running_time = 0.0f;
        
		data.rec.x = data.frame * data.rec.width;
		data.frame++;
		if (data.frame > maxFrame)
		{
			data.frame = 0;
		}
	}
    
	return data;
}
bool isOnGround(AnimData data, int windowHeight)
{
	return data.pos.y >= windowHeight - data.rec.height;
}

int main()
{
    
	//window demensions
	float windowDemensions[2]{};
	windowDemensions[0] = 512; //screen width
	windowDemensions[1] = 384; //screen height

	//game window
	InitWindow(windowDemensions[0], windowDemensions[1], "Dapper Dasher");
    
	//(SCARFY) for double jump prevention
	bool isinAir{};

	//(SCARFY)gravity (pixels per second /per second)
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
    
	//Numbe of nebulae
	const int num_of_nebulae{4};

	//initialize nebula array
	AnimData nebulae[num_of_nebulae]{};
    
	for (int i = 0; i < num_of_nebulae; i++)
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
        
		//initialize
		nebulae[i].pos.x = windowDemensions[0] + i * 300;
	}

	//Finish line
	float finishLine{nebulae[num_of_nebulae - 1].pos.x};

	//(Background) background textures
	Texture2D far_background = LoadTexture("textures/far-buildings.png");
	Texture2D midground = LoadTexture("textures/back-buildings.png");
	Texture2D foreground = LoadTexture("textures/foreground.png");
	float bg_x{};
	float m_bg_x{};
	float f_bg_x{};

	//collision detection
	bool collsion{false};

	//(CORE) Game loop----------------------------------------------------------------------------------------------
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//-----------------------------------------------------------------------------------------------------
		//-------------------(CORE) Setup Delta, Update and draw sprites, Begin/end drawing--------------------
		//-----------------------------------------------------------------------------------------------------
		//get Delta Time
		const float dt{GetFrameTime()};
        
		//Drawing-------------------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);

		//move background
		bg_x -= 230 * dt;
		if (bg_x <= -far_background.width * 2)
		{
			//reset background
			bg_x = 0.0f;
		}
		m_bg_x -= 358 * dt;
		if (m_bg_x <= -midground.width * 2)
		{
			//reset midground
			m_bg_x = 0.0f;
		}
		f_bg_x -= 440 * dt;
		if (f_bg_x <= -foreground.width * 2)
		{
			//reset foreground
			f_bg_x = 0.0f;
		}

		//draw far background
		Vector2 bg_1pos{bg_x, 0.0f};
		DrawTextureEx(far_background, bg_1pos, 0.0f, 2.0f, WHITE);
		Vector2 bg_2pos{bg_x + far_background.width * 2.0f, 0.0f};
		DrawTextureEx(far_background, bg_2pos, 0.0f, 2.0f, WHITE);

		//draw midground
		Vector2 m_bg_1pos{m_bg_x, 0.0f};
		DrawTextureEx(midground, m_bg_1pos, 0.0f, 2.0f, WHITE);
		Vector2 m_bg_2pos{m_bg_x + midground.width * 2.0f, 0.0f};
		DrawTextureEx(midground, m_bg_2pos, 0.0f, 2.0f, WHITE);

		//draw foreground
		Vector2 f_bg_1pos{f_bg_x, 0.0f};
		DrawTextureEx(foreground, f_bg_1pos, 0.0f, 2.0f, WHITE);
		Vector2 f_bg_2pos{f_bg_x + foreground.width * 2.0f, 0.0f};
		DrawTextureEx(foreground, f_bg_2pos, 0.0f, 2.0f, WHITE);

		//----------------------------------(scarfy) Gravity and check ground/ceiling------------------------------
		if (isOnGround(scarfy_data, windowDemensions[1]))
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

		//------------------------------------(Scarfy) input checking-----------------------------------------------
		if (IsKeyPressed(KEY_SPACE) && isinAir == false)
		{
			rec_current_velocity += rec_jump_velocity;
		}

		//------------------------------------Check collision-------------------------------------

		for (AnimData nebula : nebulae)
		{
			float pad{20};
			//setup nebula collision
			Rectangle nebRec{
				nebula.pos.x + pad,
				nebula.pos.y + 20,
				nebula.rec.width - 2 * pad,
				nebula.rec.height - 2 * pad};

			Rectangle scarfyRec{
				scarfy_data.pos.x,
				scarfy_data.pos.y,
				scarfy_data.rec.width,
				scarfy_data.rec.height};

			if (CheckCollisionRecs(nebRec, scarfyRec))
			{
				collsion = true;
			}
		}

		//Stop drawing nebula and scarfy if colliding
		if (collsion)
		{
			//loose the game
		}
		else
		{

			//------------------------------------(Scarfy) update position and draw-------------------------------------
			//Apply Gravity
			scarfy_data.pos.y += rec_current_velocity * dt;

			//Animate Scarfy with said data
			if (!isinAir)
			{
				//update the animation
				scarfy_data = updateAnimData(scarfy_data, dt, 5);
			}

			//draw scarfy
			DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);

			//-----------------------------------------------------------------------------------------------------------
			//-------------------------(Nebulae) Nebula position and animtaion handleing --------------------------------
			//-----------------------------------------------------------------------------------------------------------

			//Update "finish line" (last nebulae)
			finishLine += nebVel * dt;

			for (int i = 0; i < num_of_nebulae; i++)
			{
				//update nebula positon
				nebulae[i].pos.x += nebVel * dt;
				//reset if nebula goes out of bounds
				if (nebulae[i].pos.x <= 0 - nebulae[i].rec.width)
				{
					nebulae[i].pos.x = windowDemensions[0] + nebulae[i].rec.width;
				}

				//update the animation frame
				nebulae[i] = updateAnimData(nebulae[i], dt, 7);

				//draw nebula
				DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
			}
		}

		EndDrawing();
	}
	CloseWindow();
	//unload our textures
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	UnloadTexture(far_background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	return 0;
}