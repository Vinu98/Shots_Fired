#include <sfml\Graphics.hpp>
#include <iostream>
#include <vector>
#include "Bullet.h"
#include "Player.h"
#include "Platform.h"

using namespace sf;
static const float view_height = 512.0f;

int main()
{	
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ph", sf::Style::Default);
	Texture background;
	background.loadFromFile("Custom/background.png");
	Sprite backgroundSprite;
	backgroundSprite.setTexture(background);
	window.draw(backgroundSprite);
	sf::View view(sf::Vector2f(0.0f, 512.0f), sf::Vector2f(800.0f, 1024.0f));//Center and Size parmaters
	sf::View view1(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1920.0f, 1080.0f));
	sf::View death(sf::Vector2f(960.0f, 540.0f), sf::Vector2f(1920.0f, 1080.0f));

    sf::Texture playerTexture;
	playerTexture.loadFromFile("Custom/tux_from_linux.png");
	//sf::Sprite sprite(playerTexture1);
	Player player(&playerTexture, sf::Vector2u(3, 9), 0.3f, 50.0f, 200.0f);//image crop,1s animation,fps,jump
	animation animation(&playerTexture, sf::Vector2u(3, 9), 0.3f);

	Font font;
	font.loadFromFile("Custom/fo.ttf");
	Text myText;
	myText.setString("GAME OVER!");
	myText.setCharacterSize(100);
	myText.setFillColor(Color::White); 
	myText.setFont(font);

	FloatRect textRect = myText.getLocalBounds();

	myText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	myText.setPosition(1920 / 3.0f, 1080 / 3.0f);
	myText.setScale(sf::Vector2f(2.0f, 2.0f));

	float hvf = 960.0f;
	float vvf = 890.0f;
	float temp;
	std::vector<Platform> platforms;
	platforms.push_back(Platform(nullptr, sf::Vector2f(1920.0f, 50.0f), sf::Vector2f(960.0f, 940.0f)));//size,position Base Platform
	for (int i = 0; i < 3; i++){ //Steps
		platforms.push_back(Platform(nullptr, sf::Vector2f(50.0f, 50.0f), sf::Vector2f(hvf, vvf)));
			hvf += 50.0f;
			vvf -= 50.0f;
	}
	
	//platforms.push_back(Platform(nullptr, sf::Vector2f(1920.0f, 50.0f), sf::Vector2f(960.0f, 940.0f)));
	Bullet bullets[100];
	int loadedBullet = 0;
	int ammo = 30;
	int clipSize = 10;
	int bulletsInClip = 10;
	float fireRate = 1;
	
	sf::Clock clock;
	Time gameTimeTotal = sf::seconds(0);
	Time lastPressed = sf::seconds(0);
	//platform- theme,size,position
	//platforms.push_back(Platform(nullptr, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 600.0f)));
	//platforms.push_back(Platform(nullptr, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 0.0f)));
	RectangleShape timeBar;
	float timeBarStartWidth = 800;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	float timeRemaining = 60.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool alive = true;
	bool face;

	while (window.isOpen()) {
		Time dt = clock.restart();
		float dtAsSeconds = dt.asSeconds();
		if (dtAsSeconds > 1.0f / 20.0f)//least frame rate can be 20 only
			dtAsSeconds = 1.0f / 20.0f;

		timeRemaining -= dtAsSeconds;
		if (timeRemaining >= 0.0f)
			alive = true;
		else
			alive = false;
		timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
		
		face = player.Update(dtAsSeconds);
		gameTimeTotal += dt;


		//std::cout << timeRemaining << "\n";
		if (alive) {
			Event event;
			while (window.pollEvent(event)) {
				if (Keyboard::isKeyPressed(Keyboard::Escape))
					window.close();
				if (Keyboard::isKeyPressed(Keyboard::R)) {
					if (ammo >= clipSize) {
						ammo += bulletsInClip;
						bulletsInClip = clipSize;
						ammo -= clipSize;
					}
					else if (ammo > 0) {
						bulletsInClip = ammo;
						ammo = 0;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::K))
					if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) {
						bullets[loadedBullet].shoot(player.GetPosition(), face, &playerTexture);
						loadedBullet++;
						if (loadedBullet > 99)
							loadedBullet = 0;
						lastPressed = gameTimeTotal;
						bulletsInClip--;
					}
			}
			for (int i = 0; i < 100; i++)
				if (bullets[i].isInFlight())
					bullets[i].update(dtAsSeconds);

			sf::Vector2f direction;
			sf::Vector2f directionB;

			for (Platform& platform : platforms)//for (int i = 0; i < platforms.size(); i++) {Platform platform = platforms[i];}
				if (platform.GetCollider().CheckCollision(player.GetCollider(), direction, 1.0f))//Outer entity checks once for every moment
					player.onCollision(direction);

			sf::Vector2f centerConv = player.GetPosition();
			timeBar.setPosition(view.getCenter().x - 400.0f, 912.0f);
			if (centerConv.x < 400.0f)
				centerConv.x = 400.0f;
			else if (centerConv.x > 1500.0f)
				centerConv.x = 1500.0f;
			centerConv.y = 512.0f;

			view.setCenter(centerConv);
			window.setView(view);
			//window.clear();

			window.draw(backgroundSprite);
			player.Draw(window);
			for (Platform& platform : platforms) //for (int i = 0; i < platforms.size(); i++) {Platform platform = platforms[i];}
				platform.Draw(window);
			for (int i = 0; i < 100; i++) {
				for (Platform& platform : platforms)
					if (platform.GetCollider().CheckCollision(bullets[i].GetCollider(), directionB, 1.0f))
						bullets[i].onCollisionB(directionB);
				if (bullets[i].isInFlight())
					window.draw(bullets[i].getShape());
			}
			window.draw(timeBar);

			view1.setViewport(sf::FloatRect(0.75f, 0.0f, 0.2f, 0.2f));
			view1.setCenter(960.0f, 540.0f);
			window.setView(view1);
			window.draw(backgroundSprite);
			player.Draw(window);
			for (Platform& platform : platforms)
				platform.Draw(window);
		}
		else if (timeRemaining < 0.0f && timeRemaining > -4.0f) {
			death.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f ,1.0f));
			//death.setCenter(0, 0);
			window.setView(death);
			player.Death(dtAsSeconds);
			window.draw(backgroundSprite);
			player.Draw(window);
			window.draw(myText);
		}
		else
			window.close();
		
		window.display();
	}	
	return 0;
}