#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Application.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Colliders_Draw();

public:
	p2List<PhysBody*> pinball;

	//Colliders ------------------------
	PhysBody* nugget_right_;
	PhysBody* bouncer_right_;
	PhysBody* nugget_left_;
	PhysBody* bouncer_left_;
	PhysBody* large_nugget_right_;
	PhysBody* large_nugget_left_;
	PhysBody* mid_ball_;
	PhysBody* left_ball_;
	PhysBody* right_ball_;
	PhysBody* map_border_;
	PhysBody* Collider_in_Spawn_;
	PhysBody* left_topbar_;
	PhysBody* right_topbar_;
	PhysBody* top_left_nugget_;
	PhysBody* top_right_nugget_;
	PhysBody* Ball_Eater_;
	//-----------------------------------

	//sensors --------------------------------
	PhysBody* deathzone;
	bool touching_deathzone = false;

	PhysBody* tornado;
	bool touching_tornado = false;

	PhysBody* balleater_sense;
	bool touching_balleater = false;

	PhysBody* right_nugget_sens;
	bool touching_right_nugget = false;

	PhysBody* left_nugget_sens;
	bool touching_left_nugget = false;
	//--------------------------------------------

	PhysBody* leftkicker;
	PhysBody* rightkicker;

	PhysBody* leftStar;
	bool touching_leftStar = false;
	PhysBody* middleStar;
	bool touching_middleStar = false;
	PhysBody* rightStar;
	bool touching_rightStar = false;

	PhysBody* launcherSensor;
	bool touching_launcherSensor = true;

	//-----------------------------------------------

	PhysBody* leftBumper;
	PhysBody* midBumper;
	PhysBody* rightBumper;
	bool combo = false;


	//-----------------------------------------------

	SDL_Texture* map;
	SDL_Texture* arrow;
	SDL_Texture* piece_tornado;
	SDL_Texture* star;
	
	//White Arrows animations ---------------------------------------------------
	int central_arrowPosY[8] = { 446 ,428 ,410 ,392 ,374 ,356 ,338 ,320 };
	int central_counter;

	int tornado_arrowPosY[3] = { 244, 228, 214 };
	int tornado_arrowPosX[3] = { 59, 50, 41 };
	int tornado_counter;

	int leftcurve_arrowPosX[8] = { 87, 600, 600, 600, 600, 113, 129, 148 };
	int leftcurve_arrowPosY[8] = { 217, 600, 600, 600, 600, 106, 98, 93 };
	int leftangle_arrow[8] = { -33, 0, 0, 0, 0, 60, 75, 90 };
	int leftcurve_counter;

	int rightcurve_arrowPosX[8] = { 321, 600, 334, 328, 600, 295, 278, 258 };
	int rightcurve_arrowPosY[8] = { 216, 600, 163, 146, 600, 108, 97, 92 };
	int rightangle_arrow[8] = { 35, 0, 0,-15, 0, -60, -75, -90 };
	int rightcurve_counter;
	//------------------------------------------------------------------------------

	uint bonus_fx;
};
