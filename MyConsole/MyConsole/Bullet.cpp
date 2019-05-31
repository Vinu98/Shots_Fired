#include <iostream> 
#include "Bullet.h"

Bullet::Bullet() { bulletShape.setSize(sf::Vector2f(20, 10)); }

void Bullet::shoot(Vector2f start, bool face, sf::Texture* textureB ) {
	std::cout << start.x << "  " << start.y;
	this->face = face;
	inFlight = true;
	position = start;
	float range = 1000.0f;
	//min.x = start.x - range;
	//max.x = start.x + range;
	//min.y = start.x - range;
	//max.y = start.y + range;
	bulletShape.setOrigin(bulletShape.getSize() / 2.0f);
	std::cout << position.x << " " << position.y;
	bulletShape.setPosition(position);
}

void Bullet::stop() { inFlight = false; }
bool Bullet::isInFlight() { return inFlight; }
FloatRect Bullet::getBulletPosition() { return bulletShape.getGlobalBounds(); }
RectangleShape Bullet::getShape() { return bulletShape; }

void Bullet::update(float shot) {
	if (face)
		position.x += bulletSpeed * shot;
	else 
		position.x -= bulletSpeed * shot;
	//Move
	bulletShape.setPosition(position);
	
	if (position.x < 0.0f || position.x > 1920.0f)
		inFlight = false;
}

void Bullet::onCollisionB(sf::Vector2f directionB) {
	if (directionB.x < 0.0f) {//left
		bulletSpeed = 0.0f;
		position.x = 2100.0f;
	}
	else if (directionB.x > 0.0f){//right
		bulletSpeed = 0.0f;
		position.x = 2100.0f;
	}
}
