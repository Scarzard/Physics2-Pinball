#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	map = App->textures->Load("Assets/Map.png");
	arrow = App->textures->Load("Assets/arrow.png");
	piece_tornado = App->textures->Load("Assets/piece_tornado.png");
	star = App->textures->Load("Assets/star.png");

	App->audio->PlayMusic("Audio/music.ogg");

	//sensors--------------------
	deathzone = App->physics->CreateRectangleSensor(210, 532, 59, 4);
	tornado = App->physics->CreateRectangleSensor(58, 158, 14, 14);
	balleater_sense = App->physics->CreateRectangleSensor(210, 311, 36, 2); 
	leftkicker = App->physics->CreateRectangleSensor(90, 410, 10, 10);
	rightkicker = App->physics->CreateRectangleSensor(327, 410, 10, 10);

	launcherSensor = App->physics->CreateRectangleSensor(398, 325, 20, 20);
	leftStar = App->physics->CreateRectangleSensor(175, 131, 8, 22);
	middleStar = App->physics->CreateRectangleSensor(211, 122, 8, 22);
	rightStar = App->physics->CreateRectangleSensor(248, 131, 8, 22);

	right_nugget_sens = App->physics->CreateRectangleSensor(266, 423, 50, 1, 118);
	left_nugget_sens = App->physics->CreateRectangleSensor(157, 423, 50, 1, 65);

	leftBumper = App->physics->CreateRectangleSensor(170, 220, 20, 20);
	midBumper = App->physics->CreateRectangleSensor(211, 184, 20, 20);
	rightBumper = App->physics->CreateRectangleSensor(253, 220, 20, 20);
	//------------------------

	/*left_ball_ = App->physics->CreateCircle(170, 220, 10, b2_staticBody, 1.2f);
	mid_ball_ = App->physics->CreateCircle(211, 184, 10, b2_staticBody, 1.2f);
	right_ball_ = App->physics->CreateCircle(253, 220, 10, b2_staticBody, 1.2f);*/


	central_counter = 0;
	tornado_counter = 0;
	leftcurve_counter = 0;
	rightcurve_counter = 0;

	Colliders_Draw();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Map -------------------------------------------------------------------
	App->renderer->Blit(map, 0, 0, NULL, 1.0f);

	//Central Arrows ----------------------------------------------------------
	App->renderer->Blit(arrow, 204, central_arrowPosY[central_counter / 10], NULL, 1.0f);
	central_counter++;
	if (central_counter > 79)
		central_counter = 0;


	//Tornado Arrows-----------------------------------------------------------
	App->renderer->Blit(arrow, tornado_arrowPosX[tornado_counter / 15], tornado_arrowPosY[tornado_counter / 15], NULL, 1.0f, -35);
	tornado_counter++;
	if (tornado_counter > 40)
		tornado_counter = 0;

	//Left curve Arrows --------------------------------------------------------
	App->renderer->Blit(arrow, leftcurve_arrowPosX[leftcurve_counter / 10], leftcurve_arrowPosY[leftcurve_counter / 10], NULL, 1.0f, leftangle_arrow[leftcurve_counter / 10]);
	leftcurve_counter++;
	if (leftcurve_counter > 79)
		leftcurve_counter = 0;

	//Right curve Arrows --------------------------------------------------------
	App->renderer->Blit(arrow, rightcurve_arrowPosX[rightcurve_counter / 10], rightcurve_arrowPosY[rightcurve_counter / 10], NULL, 1.0f, rightangle_arrow[rightcurve_counter / 10]);
	rightcurve_counter++;
	if (rightcurve_counter > 79)
		rightcurve_counter = 0;

	if (touching_leftStar)
	{
		App->renderer->Blit(star, 165, 100);
	}
	if (touching_middleStar)
	{
		App->renderer->Blit(star, 201,94);
	}
	if (touching_rightStar)
	{
		App->renderer->Blit(star, 240, 100);
	}

	if (touching_leftStar && touching_middleStar && touching_rightStar)
		combo = true;

	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}

void ModuleSceneIntro::Colliders_Draw()
{
	int nugget_right[16] = {
		279, 400,
		256, 442,
		258, 450,
		267, 449,
		276, 439,
		285, 422,
		289, 403,
		284, 399
	};
	nugget_right_ = App->physics->CreateChain(0, 0, nugget_right, 16);

	//bouncer_right
	int bouncer_right[8] = {
		277, 401,
		258, 440,
		260, 441,
		279, 402
	};
	bouncer_right_ = App->physics->CreateChain(0, 0, bouncer_right, 8, 2.0f);

	int nugget_left[20] = {
		133, 403,
		134, 419,
		142, 435,
		152, 448,
		159, 452,
		164, 449,
		164, 441,
		154, 421,
		143, 400,
		136, 400
	};
	nugget_left_ = App->physics->CreateChain(0, 0, nugget_left, 20);

	int bouncer_left[8] = {
		144, 402,
		163, 440,
		161, 441,
		142, 403
	};
	bouncer_left_ = App->physics->CreateChain(0, 0, bouncer_left, 8, 2.0f);

	int LargeNugget_right[28] = {
		287, 481,
		302, 466,
		313, 448,
		319, 428,
		320, 397,
		315, 394,
		311, 399,
		308, 420,
		299, 440,
		286, 455,
		274, 466,
		271, 474,
		275, 481,
		284, 483
	};
	large_nugget_right_ = App->physics->CreateChain(0, 0, LargeNugget_right, 28);

	int LargeNugget_left[28] = {
		141, 483,
		147, 476,
		147, 468,
		129, 450,
		116, 429,
		109, 408,
		108, 399,
		104, 393,
		99, 398,
		99, 420,
		104, 442,
		114, 463,
		125, 476,
		136, 483
	};
	large_nugget_left_ = App->physics->CreateChain(0, 0, LargeNugget_left, 28);

	left_ball_ = App->physics->CreateCircle(170, 220, 10, b2_staticBody, 1.2f);
	mid_ball_ = App->physics->CreateCircle(211, 184, 10, b2_staticBody, 1.2f);
	right_ball_ = App->physics->CreateCircle(253, 220, 10, b2_staticBody, 1.2f);

	int Map_Border[208] = {
		254, 80,
		276, 81,
		294, 84,
		321, 88,
		342, 92,
		360, 94,
		376, 96,
		393, 104,
		405, 115,
		413, 130,
		420, 150,
		421, 167,
		420, 181,
		414, 202,
		410, 221,
		409, 323,
		414, 324,
		426, 325,
		432, 326,
		438, 335,
		438, 438,
		433, 447,
		426, 450,
		385, 450,
		385, 257,
		391, 211,
		399, 179,
		400, 162,
		398, 150,
		390, 135,
		383, 127,
		368, 120,
		354, 116,
		337, 116,
		347, 140,
		352, 162,
		350, 183,
		341, 215,
		335, 231,
		326, 248,
		334, 253,
		340, 243,
		348, 235,
		358, 232,
		369, 243,
		368, 255,
		354, 271,
		341, 297,
		335, 322,
		335, 353,
		341, 378,
		343, 401,
		342, 427,
		332, 456,
		316, 484,
		293, 505,
		267, 522,
		239, 529,
		241, 536,
		179, 536,
		179, 531,
		168, 528,
		146, 519,
		126, 506,
		106, 487,
		92, 467,
		82, 446,
		76, 420,
		75, 391,
		80, 366,
		81, 341,
		79, 314,
		73, 288,
		57, 251,
		25, 204,
		19, 183,
		23, 159,
		33, 146,
		44, 141,
		55, 140,
		66, 145,
		71, 154,
		73, 163,
		68, 170,
		57, 173,
		50, 169,
		43, 171,
		40, 181,
		43, 193,
		58, 213,
		73, 241,
		89, 255,
		96, 252,
		92, 240,
		81, 212,
		76, 184,
		80, 156,
		87, 132,
		97, 115,
		122, 98,
		144, 88,
		166, 84,
		175, 69,
		246, 69
	};

	map_border_ = App->physics->CreateChain(0,0,Map_Border, 208);

	int Collider_in_Spawn[16] = {
		410, 327,
		406, 332,
		406, 422,
		409, 428,
		414, 428,
		416, 424,
		416, 332,
		413, 327
	};
	Collider_in_Spawn_ = App->physics->CreateChain(0, 0, Collider_in_Spawn, 16);


	int left_topbar[16] = {
		190, 107,
		186, 111,
		186, 136,
		190, 141,
		195, 141,
		198, 136,
		198, 111,
		195, 107
	};
	left_topbar_ = App->physics->CreateChain(0, 0, left_topbar, 16);

	int right_topbar[16] = {
		227, 107,
		224, 111,
		224, 136,
		227, 140,
		232, 140,
		235, 136,
		235, 111,
		232, 107
	};

	right_topbar_ = App->physics->CreateChain(0, 0, right_topbar, 16);

	int top_left_nugget[32] = {
		153, 121,
		158, 131,
		155, 144,
		142, 150,
		118, 159,
		109, 168,
		107, 181,
		111, 197,
		119, 219,
		116, 225,
		109, 225,
		101, 214,
		99, 201,
		111, 130,
		136, 118,
		149, 118
	};
	top_left_nugget_ = App->physics->CreateChain(0, 0, top_left_nugget, 32);

	int top_right_nugget[38] = {
		282, 116,
		271, 118,
		263, 128,
		265, 143,
		284, 152,
		299, 158,
		309, 165,
		314, 176,
		312, 189,
		303, 218,
		305, 226,
		313, 225,
		322, 208,
		329, 180,
		329, 166,
		325, 154,
		315, 138,
		302, 125,
		292, 119
	};
	top_right_nugget_ = App->physics->CreateChain(0, 0, top_right_nugget, 38);

	int Ball_Eater[32] = {
		172, 273,
		178, 270,
		187, 269,
		203, 268,
		218, 268,
		237, 269,
		245, 270,
		250, 274,
		251, 281,
		249, 290,
		242, 301,
		228, 309,
		193, 309,
		180, 300,
		172, 289,
		171, 280
	};
	Ball_Eater_ = App->physics->CreateChain(0, 0, Ball_Eater, 32);
}