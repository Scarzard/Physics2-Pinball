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
	App->renderer->Blit(map, 0, 0, NULL, 1.0f);

	//if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//{
	//	circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	//	circles.getLast()->data->listener = this;
	//}

	//if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	//{
	//	boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	//}

	//if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	//{
	//	// Pivot 0, 0
	//	int rick_head[64] = {
	//		14, 36,
	//		42, 40,
	//		40, 0,
	//		75, 30,
	//		88, 4,
	//		94, 39,
	//		111, 36,
	//		104, 58,
	//		107, 62,
	//		117, 67,
	//		109, 73,
	//		110, 85,
	//		106, 91,
	//		109, 99,
	//		103, 104,
	//		100, 115,
	//		106, 121,
	//		103, 125,
	//		98, 126,
	//		95, 137,
	//		83, 147,
	//		67, 147,
	//		53, 140,
	//		46, 132,
	//		34, 136,
	//		38, 126,
	//		23, 123,
	//		30, 114,
	//		10, 102,
	//		29, 90,
	//		0, 75,
	//		30, 62
	//	};

	//	ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	//}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	//// All draw functions ------------------------------------------------------
	//p2List_item<PhysBody*>* c = circles.getFirst();

	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

	//c = boxes.getFirst();

	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

	//c = ricks.getFirst();

	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

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

	int mid_ball[18] = {
		206, 172,
		200, 179,
		200, 188,
		206, 194,
		215, 194,
		220, 189,
		220, 178,
		215, 172,
		208, 172
	};
	mid_ball_ = App->physics->CreateChain(0, 0, mid_ball, 18);

	int left_ball[18] = {
		164, 209,
		158, 215,
		158, 223,
		164, 228,
		173, 228,
		179, 222,
		179, 213,
		173, 209,
		166, 209
	};
	left_ball_ = App->physics->CreateChain(0, 0, left_ball, 18);

	int right_ball[18] = {
		248, 208,
		242, 214,
		242, 223,
		248, 228,
		257, 228,
		262, 224,
		262, 214,
		257, 208,
		250, 208
	};
	right_ball_ = App->physics->CreateChain(0, 0, right_ball, 18);

	int Map_Border[208] = {
	252, 81,
	275, 85,
	295, 92,
	321, 109,
	331, 101,
	344, 97,
	365, 95,
	384, 101,
	401, 112,
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
	352, 119,
	339, 124,
	347, 147,
	349, 164,
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



	
}