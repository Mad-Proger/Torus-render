#define _USE_MATH_DEFINES
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Main.hpp"
#include "Vector3.h"
#include "Torus.h"
#include "Windows.h"

#include <iostream>
#include <cmath>
#include <random>
#include <fstream>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {
	std::ifstream parametersFile("StartParameters.txt");

	long double radius, distance, wx, wy, wz;
	Vector3 center;
	parametersFile >> radius >> distance >> center >> wx >> wy >> wz;
	Torus torus(center, radius, distance);

	Vector3 lightSource, viewPosition;
	long double fovAngle;
	parametersFile >> lightSource >> viewPosition >> fovAngle;
	fovAngle = M_PI * (fovAngle / 180.L);

	unsigned int width, height;
	parametersFile >> width >> height;
	
	parametersFile.close();

	sf::RenderWindow wnd(sf::VideoMode(width, height), "Torus Render");
	sf::Texture torusTexture;
	torusTexture.create(width, height);
	sf::Sprite torusSprite;
	torusSprite.setTexture(torusTexture);
	sf::Uint8 *pixelArray = new sf::Uint8[(long long)width * height * 4U];

	while (wnd.isOpen()) {
		sf::Event evnt;
		while (wnd.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) {
				wnd.close();
				break;
			}
		}

		wnd.clear();
		torus.render(lightSource, viewPosition, fovAngle, (unsigned char *)pixelArray, width, height);
		torusTexture.update(pixelArray);
		wnd.draw(torusSprite);
		wnd.display();
		torus.rotateX(wx);
		torus.rotateY(wy);
		torus.rotateZ(wz);
	}
	
	delete[] pixelArray;
	return 0;
}
