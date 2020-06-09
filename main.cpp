#include <SFML/Graphics.hpp>
#include <chrono>
#include <Windows.h> // ����� ������ ��� Sleep � ����� �����

const double pi = 3.14159;
const int Screen_W = 1280;		// ������ ������
const int Screen_H = 720;		// ������ ������
const int Map_W = 16;			// ������ �������� ����
const int Map_H = 16;			// ������ �������� ����
const float Speed = 5.0;		//�������� ������������ ������\�����
double CameraX = 7.5;			//����������� ��������� ������ �� �����������
double CameraY = 7.5;			//����������� ��������� ������ �� ��������� (������ ����)
double direction = 0.0;			// ����������� ����������� �������
double DOV = 16;				// ������� ���������� (����� �� ������, ��� �� �������)
double FOV = pi / 6;			// ���� ������
double thr_dist;				//���������� ����� ���� � Ray Cast
double luma;					// ������������ �������
double size;					// �������� ������ ������� (��� �����������)

float RayCast(double thr_dist,double RayX,double RayY,bool Collision,std::string map) {			// �������, ����������� Ray Casting
	while (thr_dist < DOV && !Collision) {		// �����������, ���� �� ����� �� ������� ������� ���������� ��� �� ����������� �� ������
		thr_dist += 0.1;																		// ��� ���������� ����� ����
		int AimX = int(CameraX + RayX * thr_dist);												// ����� ������� thr_dist �� �
		int AimY = int(CameraY + RayY * thr_dist);												// ����� ������� thr_dist �� �
		if (AimX < 0 || AimY < 0 || AimX > Map_W || AimY > Map_H)								// ���� ��� ����� �� ������� �����, �� �������, ��� �������� � ������� ���������� ( ���� ����� �� �������� �� ���������)
		{
			Collision = true;
			thr_dist = DOV;
		}
		else if (map.c_str()[AimX + AimY * Map_W] == '1')										// ��������� ������������ � ��������
			Collision = true;
	}
	return thr_dist;		//����� �������
}

int main()
{
	std::string map;					//������ ������� ���� � ���� ������� ������
	map += "1111111111111111";
	map += "1..............1";
	map += "1......111.....1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1.........1....1";
	map += "1..............1";
	map += "1.....11.......1";
	map += "1...111111.....1";
	map += "1..............1";
	map += "1111111111111111";

	auto time1 = std::chrono::system_clock::now();									// ����� ������ ���������� ��������
	auto time2 = std::chrono::system_clock::now();									// ����� ������ ������ ��������
	sf::RenderWindow window(sf::VideoMode(Screen_W, Screen_H), "3d!");			// ������ ����
	while (window.isOpen()) {													// ���� ���� ������� ���� ���� ����� �����������
		sf::Event event;							//
		while (window.pollEvent(event))				//
		{											//  ��������� ����������� �������� ����
			if (event.type == sf::Event::Closed)	//
				window.close();						//
		}
		time2 = std::chrono::system_clock::now();						// ��������� ����� � ������ ������
		std::chrono::duration<float> dTime = time2 - time1;				// ������� ������� ������� ������ � ����������� ����
		time1 = time2;													//���������� ��� ����� ����� ��������� � ���� ��� ��������� ����������
		float interval = dTime.count();									 // ���������� ��������� ����� � ���������� � ������� �������
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))				// ������� ������� W
		{
			CameraX += sin(direction) * Speed * interval;				// �������� ���������� �� � � �� � �� ��������������� �������� � ������ ����
			CameraY += cos(direction) * Speed * interval;
			if (map[int(CameraX + (CameraY)*Map_W)] == '1') {			// ���� ��� ���� �� ������ � �����, �� ���������� ���
				CameraX -= sin(direction) * Speed * interval;
				CameraY -= cos(direction) * Speed * interval;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))				// ������� ������� A
		{
			direction -= 0.1;											// ������� �����
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))				// ������� ������� S
		{
			CameraX -= sin(direction) * Speed * interval;				// �������� ���������� �� � � �� � �� ��������������� �������� � ������ ����
			CameraY -= cos(direction) * Speed * interval;
			if (map[int(CameraX + (CameraY)*Map_W)] == '1') {			// ���� ��� ���� �� ������ � �����, �� ���������� ���
				CameraX += sin(direction) * Speed * interval;
				CameraY += cos(direction) * Speed * interval;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))				// ������� ������� D
		{
			direction += 0.1;											// ������� ������
		}
		window.clear(sf::Color::Black);																// ����� ������ ����� ���������� ����������� ���� ����� ������
		for (int x = 0; x < Screen_W; ++x)															// ���������� �� ������ ������� �������� � ����� ����
		{
			double CurrentAngle = direction - FOV / 2 + double(x) / Screen_W * FOV;					// ���� ��� ������ ���� � Ray Cast
			double thr_dist = 0.f;																	// ����� ����
			bool Collision = false;																	// ���������� ������������ �� ������ (��\���)
			double RayX = sinf(CurrentAngle);														// ��������� ������ ���� �� �
			double RayY = cosf(CurrentAngle);														// ��������� ������ ���� �� �
			thr_dist = RayCast(thr_dist, RayX, RayY, Collision, map);								// �������� �������� Ray Casting � ������ ��������� �� ����� � ���������� �����������
			luma = ((DOV - thr_dist) / DOV);														// ������� ������� ������������ �������
			size = Screen_H/2*luma;																	// ������� �������� ������ ����������� ������� (��� �����������)
			sf::RectangleShape rect;																// ��������� �������������
			rect.setSize(sf::Vector2f(1, 2.4 * size));												// ������ ��� ������ 1 ������� (�������) � ������� ������������� ����� � �����������
			rect.setPosition(sf::Vector2f(x, int(Screen_H / 2 - 1.2*size)));						// ������ ������� � ������ ����� �� ������
			rect.setFillColor(sf::Color(255*luma, 255 * luma, 255 * luma));							// ������������ ������� � ����������� �� �����������
			window.draw(rect);																		// ������������ �������
		}
		window.display();																			// ������� �� ����� ��� ������� �� �����
		Sleep(40);																					// �������� � 1/25 ������� ����� �� ����������� ��������� � �������� ������� ������������
	}
	return 0;
}