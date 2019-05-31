#include <iostream> 
#include "Player.h"
//#include "SFML/Graphics.hpp"


Player::Player(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, float jumpHeight):

animation(texture, imageCount, switchTime){
	this->speed = speed;
	this->jumpHeight = jumpHeight;
	row = 0;
	faceRight = true;
	body.setSize(sf::Vector2f(100.0f, 150.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(206.0f, 206.0f);
	body.setTexture(texture);

}

Player::~Player(){}

bool Player::Update(float deltaTime){
	velocity.x *= 0.5f;//speed factor while stopping

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity.x -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity.x += speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && canJump) {
		canJump = false;
		velocity.y = -sqrt(2.0f * 980 * jumpHeight);//v = root(2gh) 100 sfml unit = 1 m
	}
	velocity.y += 980 * deltaTime;
	if (velocity.x == 0.0f)
		row = 0;
	else {
		row = 1;
		if (velocity.x > 0)
			faceRight = true;
		else
			faceRight = false;
	}
	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(velocity * deltaTime);
	return faceRight;
}

void Player::Death(float deltaTime){	
	body.setScale(2, 2);
	body.setPosition(960, 540);
	animation.Update(8, deltaTime, true);
	body.setTextureRect(animation.uvRect);
}



void Player::Draw(sf::RenderWindow& window){
	window.draw(body);
}

void Player::onCollision(sf::Vector2f direction) {
	if (direction.x < 0.0f)//left
		velocity.x = 0.0f;
	else if (direction.x > 0.0f)//right
		velocity.x = 0.0f;
	if (direction.y < 0.0f) {
		//bottom collision
		velocity.y = 0.0f;
		canJump = true;
	} 
	else if (direction.y > 0.0f) {
		//top collision
		velocity.y = 0.0f;	
	}
}

