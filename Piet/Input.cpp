#include "Input.h"

Input::Input()
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
	{
		std::cout << "no controller connected!";
	}
	gamePad = SDL_JoystickOpen(0);
	if (gamePad == NULL)
	{
		std::cout << "no controller connected";
	}

	deadZone = 1000;
}

Input::~Input()
{
}

bool Input::getGamePadButton(int gamePadButton)
{
	return gamePadButtonIsPressed[gamePadButton];
}

bool Input::getGamePadButtonDown(int gamePadButton)
{
	return gamePadButtonJustDown[gamePadButton];
}

bool Input::getGamePadButtonUp(int gamePadButton)
{
	return gamePadButtonJustUp[gamePadButton];
}

int Input::getGamePadAxis(int axis)
{
	if (abs(SDL_JoystickGetAxis(gamePad, axis)) > deadZone)
		return SDL_JoystickGetAxis(gamePad, axis);

	return 0;
}

bool Input::getKey(int keyCode)
{
	return keyIsPressed[keyCode];
}

bool Input::getKeyDown(int keyCode)
{
	return keyJustDown[keyCode];
}

bool Input::getKeyUp(int keyCode)
{
	return keyJustUp[keyCode];
}

int Input::getMouseX()
{
	return mouseX;
}

int Input::getMouseY()
{
	return mouseY;
}

bool Input::getMouseButton(int mouseButton)
{
	return mouseButtonIsPressed[mouseButton];
}

bool Input::getMouseButtonDown(int mouseButton)
{
	return mouseButtonJustDown[mouseButton];
}

bool Input::getMouseButtonUp(int mouseButton)
{
	return mouseButtonJustUp[mouseButton];
}

void Input::update()
{
	//mouse
	SDL_PumpEvents();
	button = SDL_GetMouseState(&mouseX, &mouseY);

	for (unsigned int i = 0; i < NUM_MOUSECODES; i++)
	{
		//just down keys
		if (!mouseButtonIsPressed[i] && (button&SDL_BUTTON(i+1)))
			mouseButtonJustDown[i] = true;
		else
			mouseButtonJustDown[i] = false;

		//just up keys
		if (mouseButtonIsPressed[i] && !(button&SDL_BUTTON(i + 1)))
			mouseButtonJustUp[i] = true;
		else
			mouseButtonJustUp[i] = false;

		//key is down
		if ((button&SDL_BUTTON(i + 1)))
			mouseButtonIsPressed[i] = true;
		else
			mouseButtonIsPressed[i] = false;
	}

	//keyboard
	state = SDL_GetKeyboardState(NULL);
	for (unsigned int i = 0; i < NUM_KEYCODES; i++)
	{
		//just down keys
		if (!keyIsPressed[i] && state[i])
			keyJustDown[i] = true;
		else
			keyJustDown[i] = false;

		//just up keys
		if (keyIsPressed[i] && !state[i])
			keyJustUp[i] = true;
		else
			keyJustUp[i] = false;

		//key is down
		if (state[i])
			keyIsPressed[i] = true;
		else
			keyIsPressed[i] = false;
	}

	//gamepad
	SDL_JoystickUpdate();
	for (unsigned int i = 0; i < NUM_GAMEPADCODES; i++)
	{
		//just down keys
		if (!gamePadButtonIsPressed[i] && SDL_JoystickGetButton(gamePad, i))
			gamePadButtonJustDown[i] = true;
		else
			gamePadButtonJustDown[i] = false;

		//just up keys
		if (gamePadButtonIsPressed[i] && !SDL_JoystickGetButton(gamePad, i))
			gamePadButtonJustUp[i] = true;
		else
			gamePadButtonJustUp[i] = false;

		//key is down
		if (SDL_JoystickGetButton(gamePad, i))
			gamePadButtonIsPressed[i] = true;
		else
			gamePadButtonIsPressed[i] = false;
	}
}