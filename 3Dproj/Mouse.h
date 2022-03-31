#pragma once
#include <SFML/Window.hpp>
#include "Keyboard.h"
#include "Vec.h"
#include "WindowHelper.h"
#include <queue>

struct MousePoint
{
	int x, y;
};

//made from Jpres https://www.youtube.com/channel/UC5Lxe7GAsk_f8qMBsNmlOJg
class mouseEvent {
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		INVALID
	};
private:
	EventType type;
	int x;
	int y;
public:
	mouseEvent();
	mouseEvent(const EventType type, const int x, const int y);
	bool IsValid() const;
	EventType getType() const;
	MousePoint getPos() const;
	int getPosX() const;
	int getPosY() const;
};

class Mouse {
public:
	Mouse();
	virtual ~Mouse();

	void updateMouse(MSG msg);
	bool IsLeftDown();
	bool isMiddleDown();
	bool isRightDown();
	MousePoint getPos();
	int getPosX();
	int getPosY();
	bool EventBufferEmpty();
	mouseEvent ReadEvent();

	void changeSense(float newSence);
	float getSense();
	bool getMouseActive();
private:
	void onLeftPressed(int x, int y);
	void onLeftReleased(int x, int y);
	void onRightPressed(int x, int y);
	void onRightReleased(int x, int y);
	void onMiddlePressed(int x, int y);
	void onMiddleReleased(int x, int y);
	void onWheelUp(int x, int y);
	void onWheelDown(int x, int y);
	void onMouseMove(int x, int y);
	void onMouseMoveRaw(int x, int y);
private:
	void activateMouse();
	bool mouse_active;
	bool once;
	int x, y;
	bool leaftIsDown, rightIsDown, midIsDown;
	float mouseSense;
	std::queue<mouseEvent>mouseBuffer;
};

