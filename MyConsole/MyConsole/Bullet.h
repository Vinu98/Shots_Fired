#pragma once
#include <sfml\Graphics.hpp>
#include "Collider.h"
using namespace sf;
class Bullet {
private:
	Vector2f position;
	RectangleShape bulletShape;
	bool inFlight = false;
	float bulletSpeed = 400.0f;
	Vector2f bulletDistance;
	bool face;
	//Boundaries
	Vector2f max;
	Vector2f min;
public:
	Bullet();
	void stop();
	bool isInFlight();
	void shoot(Vector2f start, bool face, Texture* textureB);
	RectangleShape getShape();
	void update(float shot);
	FloatRect getBulletPosition();
	void onCollisionB(sf::Vector2f directionB);
	Collider GetCollider() { return Collider(bulletShape); }
};
