#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	sf::RectangleShape playerRectangle;
	sf::RectangleShape mouseRectangle;
	sf::CircleShape circle;
	playerRectangle.setPosition(0.0f, 0.0f);
	playerRectangle.setSize(sf::Vector2f(88.5f, 88.5f));
	playerRectangle.setOutlineThickness(4.0f);
	playerRectangle.setFillColor(sf::Color::Transparent);
	mouseRectangle.setSize(sf::Vector2f(88.5f, 88.5f));
	mouseRectangle.setOutlineThickness(4.0f);
	mouseRectangle.setFillColor(sf::Color::Transparent);
	mouseRectangle.setOutlineColor(sf::Color::Green);
	playerRectangle.setOutlineColor(sf::Color::Green);
	circle.setPosition(700.0f, 0.0f);
	circle.setRadius(50);
	circle.setFillColor(sf::Color::Yellow);
	// Load a sprite to display
	sf::Texture sprite_sheet;
	if (!sprite_sheet.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture mouse_texture;
	if (!mouse_texture.loadFromFile("assets\\mouse.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Setup a mouse Sprite
	sf::Sprite mouse;
	mouse.setTexture(mouse_texture);

	//Setup mouse AABB
	c2AABB aabb_mouse;
	aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(), 
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());
	
	
	//setup circle collider
	c2Circle c2circle_collider;
	c2circle_collider.p = c2V(750, 50);
	c2circle_collider.r = 50;

	// Setup the Player
	Player player(animated_sprite);
	Input input;

	// Collision result
	int AABBresult = 0;
	int Circleresult = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		mouseRectangle.setPosition(mouse.getPosition().x, mouse.getPosition().y);
		// Update mouse AABB
		aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Check for collisions
		AABBresult = c2AABBtoAABB(aabb_mouse, aabb_player);
		Circleresult = c2CircletoAABB(c2circle_collider, aabb_mouse);
		cout << ((AABBresult != 0) ? ("Collision") : "") << endl;
		cout << ((Circleresult != 0) ? ("Collision") : "") << endl;
		if (AABBresult)
		{
			mouseRectangle.setOutlineColor(sf::Color::Red);
			playerRectangle.setOutlineColor(sf::Color::Red);
		}
		else 
		{
			mouseRectangle.setOutlineColor(sf::Color::Green);
			playerRectangle.setOutlineColor(sf::Color::Green);
		}

		if (Circleresult)
		{
			mouseRectangle.setOutlineColor(sf::Color::Red);
		}
		else
		{
			mouseRectangle.setOutlineColor(sf::Color::Green);
		}

		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		window.draw(mouse);
		window.draw(playerRectangle);
		window.draw(mouseRectangle);
		window.draw(circle);
		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};