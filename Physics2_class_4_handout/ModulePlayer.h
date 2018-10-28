#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "Box2D/Box2D/Box2D.h"
#include <string>
#include "p2SString.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void CreateBall(int x, int y, float vx = 0, float vy = 0);
	void CreateFlippers();
	void CreateLauncher();

public:

	int i = 0;

	//Textures
	SDL_Texture* ball_texture;
	SDL_Texture* flipper;
	SDL_Texture* flipper2;

	//Player attributes
	uint score = 0;
	uint lives = 5;

	//fx
	uint flippers_FX;
	uint bouncers_FX;
	uint ball_eater_FX;
	uint kicker_fx;

	//kicker counter
	uint leftKickerActivations = 3;
	uint rightKickerActivations = 3;

	//kickers
	PhysBody* leftAutoKicker;
	PhysBody* rightAutoKicker;

	//Flippers
	PhysBody* leftFlipper;
	PhysBody* leftPivot;
	b2RevoluteJoint* leftJoint;

	PhysBody* rightFlipper;
	PhysBody* rightPivot;
	b2RevoluteJoint* rightJoint;

	PhysBody* launcherBody;
	PhysBody* launcherPivot;
	b2PrismaticJoint* launcherJoint;
	b2Vec2 axis;

	
private:

	PhysBody * ball;
	p2SString* title = new p2SString("Pepsi Pinball / Score: %d", score);
};