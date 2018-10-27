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

	int nugget_left[18] = {
		142, 123,
		164, 165,
		162, 173,
		156, 174,
		146, 166,
		138, 152,
		133, 137,
		132, 126,
		142, 123
	};
	nugget_left_ = App->physics->CreateChain(0, 0, nugget_left, 18);

	int LargeNugget_right[34] = {
		38, 192,
		52, 180,
		62, 167,
		70, 154,
		76, 135,
		78, 123,
		80, 119,
		86, 120,
		87, 130,
		86, 147,
		82, 165,
		73, 184,
		61, 199,
		51, 206,
		44, 206,
		38, 200,
		38, 193
	};
	large_nugget_right_ = App->physics->CreateChain(0, 0, LargeNugget_right, 34);

	int LargeNugget_left[34] = {
		100, 122,
		100, 142,
		103, 162,
		109, 178,
		119, 194,
		134, 207,
		141, 208,
		147, 201,
		148, 193,
		140, 185,
		125, 169,
		115, 152,
		110, 137,
		108, 126,
		108, 121,
		104, 118,
		100, 122
	};
	large_nugget_left_ = App->physics->CreateChain(0, 0, LargeNugget_left, 34);

	int mid_ball[18] = {
		214, 173,
		205, 173,
		200, 177,
		200, 189,
		204, 194,
		216, 194,
		221, 189,
		221, 178,
		214, 173
	};
	mid_ball_ = App->physics->CreateChain(0, 0, mid_ball, 18);

	int left_ball[18] = {
		163, 208,
		158, 214,
		158, 224,
		164, 229,
		173, 229,
		179, 224,
		179, 214,
		173, 208,
		163, 208
	};
	left_ball_ = App->physics->CreateChain(0, 0, left_ball, 18);

	int right_ball[18] = {
		14, 207,
		8, 213,
		8, 223,
		13, 229,
		23, 229,
		29, 224,
		29, 213,
		24, 207,
		14, 207
	};
	right_ball_ = App->physics->CreateChain(0, 0, right_ball, 18);
}