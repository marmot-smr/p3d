#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <map>
#include <vector>
#include "settings.h"
#include <string>
#include <Windows.h>

#define PI 3.14159

float CameraX = 7.5;
float CameraY = 7.5;
float direction = 0.0;
float DOV = 16;
float FOV = PI / 3;
float thr_dist ;
int n;
int luma;
int size;

int main()
{
	std::vector <wchar_t> Screen;
	Screen.reserve(SCREEN_W * SCREEN_H);

	std::wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#....#.........#";
	map += L"#....##........#";
	map += L"#....#.........#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#.........#....#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#.......##.....#";
	map += L"#........##....#";
	map += L"#..............#";
	map += L"################";

	auto time1 = std::chrono::system_clock::now();
	auto time2 = std::chrono::system_clock::now();
	sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "3d?");
	while (window.isOpen()) {
		time2 = std::chrono::system_clock::now();
		std::chrono::duration<float> dTime = time2 - time1;
		time1 = time2;
		float interval = dTime.count();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			CameraX += cos(direction) * SPEED * interval;
			CameraY += sin(direction) * SPEED * interval;
			if (map.c_str()[int(CameraX + (CameraY) * MAP_W)] == '#') {
				CameraX -= cos(direction) * SPEED * interval;
				CameraY -= sin(direction) * SPEED * interval;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			direction -= 0.01;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			CameraX -= cos(direction) * SPEED * interval;
			CameraY -= sin(direction) * SPEED * interval;
			if (map.c_str()[int(CameraX + (CameraY) * MAP_W)] == '#') {
				CameraX += cos(direction) * SPEED * interval;
				CameraY += sin(direction) * SPEED * interval;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			direction += 0.01;
		}
		n = 1;		
		for (float i = -FOV / 2; i < FOV / 2; i += (FOV / SCREEN_W))
		{
			thr_dist = 0.1;
			while ((int(CameraX + thr_dist * cos(direction + i) + SCREEN_W * int(CameraY + thr_dist * sin(direction + i))) != '#') && (thr_dist<DOV)){
				thr_dist += 0.1;
			}
			luma = (DOV - thr_dist) / DOV;
			size = 20 + luma * 10;
			sf::Vertex line[] = {
				sf::Vertex(sf::Vector2f(n, SCREEN_H / 2 - size)),
				sf::Vertex(sf::Vector2f(n, SCREEN_H / 2 + size))
			};
			window.draw(line, 2, sf::Lines);
			n++;
		}
	}
	return 0;
}