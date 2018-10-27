#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

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

	//Colliders
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

	//sensors
	PhysBody* nugget_bouncers_sensors[2];
	bool touch_nuggets[2] = {false, false};


	SDL_Texture* map;

	uint bonus_fx;
};
