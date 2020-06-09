#include <SFML/Graphics.hpp>
#include <chrono>
#include <Windows.h> // Нужен только для Sleep в самом конце

const double pi = 3.14159;
const int Screen_W = 1280;		// Ширина экрана
const int Screen_H = 720;		// высота экрана
const int Map_W = 16;			// ширина игрового поля
const int Map_H = 16;			// высота игрового поля
const float Speed = 5.0;		//скорость передвижения вперед\назад
double CameraX = 7.5;			//изначальное положение игрока по горизонтали
double CameraY = 7.5;			//изначальное положение игрока по вертикали (сверху вниз)
double direction = 0.0;			// изначальное направление взгляда
double DOV = 16;				// глубина прорисовки (лучше не менять, код не идеален)
double FOV = pi / 6;			// угол обзора
double thr_dist;				//переменная длины луча в Ray Cast
double luma;					// освещённость полоски
double size;					// половина высоты полоски (без коэфициента)

float RayCast(double thr_dist,double RayX,double RayY,bool Collision,std::string map) {			// функция, реализующая Ray Casting
	while (thr_dist < DOV && !Collision) {		// выполняется, пока не вышли за пределы глубины прорисовки или не столкнулись со стеной
		thr_dist += 0.1;																		// шаг увеличения длины луча
		int AimX = int(CameraX + RayX * thr_dist);												// конец вектора thr_dist по Х
		int AimY = int(CameraY + RayY * thr_dist);												// конец вектора thr_dist по У
		if (AimX < 0 || AimY < 0 || AimX > Map_W || AimY > Map_H)								// если луч вышел за приделы карты, то считаем, что уперлись в глубину прорисовки ( если карта не замкнута по периметру)
		{
			Collision = true;
			thr_dist = DOV;
		}
		else if (map.c_str()[AimX + AimY * Map_W] == '1')										// обработка столкновения с объектом
			Collision = true;
	}
	return thr_dist;		//длина вектора
}

int main()
{
	std::string map;					//храним игровое поле в виде обычной строки
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

	auto time1 = std::chrono::system_clock::now();									// время начала предыдущей итерации
	auto time2 = std::chrono::system_clock::now();									// время начала данной итерации
	sf::RenderWindow window(sf::VideoMode(Screen_W, Screen_H), "3d!");			// создаём окно
	while (window.isOpen()) {													// пока окно открыто этот цикл будет повторяться
		sf::Event event;							//
		while (window.pollEvent(event))				//
		{											//  обработка возможности закрытия окна
			if (event.type == sf::Event::Closed)	//
				window.close();						//
		}
		time2 = std::chrono::system_clock::now();						// фиксируем время в данный момент
		std::chrono::duration<float> dTime = time2 - time1;				// смотрим сколько времени прошло с предыдущего раза
		time1 = time2;													//запоминаем это время чтобы вернуться к нему при следующем повторении
		float interval = dTime.count();									 // записываем прошедшее время в переменную в удобном формате
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))				// нажатие клавиши W
		{
			CameraX += sin(direction) * Speed * interval;				// изменяем координаты по Х и по У на соответствующие значения с учетом угла
			CameraY += cos(direction) * Speed * interval;
			if (map[int(CameraX + (CameraY)*Map_W)] == '1') {			// если при этом мы войдем в стену, то откатываем шаг
				CameraX -= sin(direction) * Speed * interval;
				CameraY -= cos(direction) * Speed * interval;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))				// нажатие клавиши A
		{
			direction -= 0.1;											// поворот влево
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))				// нажатие клавиши S
		{
			CameraX -= sin(direction) * Speed * interval;				// изменяем координаты по Х и по У на соответствующие значения с учетом угла
			CameraY -= cos(direction) * Speed * interval;
			if (map[int(CameraX + (CameraY)*Map_W)] == '1') {			// если при этом мы войдем в стену, то откатываем шаг
				CameraX += sin(direction) * Speed * interval;
				CameraY += cos(direction) * Speed * interval;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))				// нажатие клавиши D
		{
			direction += 0.1;											// поворот вправо
		}
		window.clear(sf::Color::Black);																// перед каждой новой отрисовкой закрашиваем весь экран черным
		for (int x = 0; x < Screen_W; ++x)															// прозодимся по каждой колонке пикселей в нашем окне
		{
			double CurrentAngle = direction - FOV / 2 + double(x) / Screen_W * FOV;					// угол для броска луча в Ray Cast
			double thr_dist = 0.f;																	// длина луча
			bool Collision = false;																	// переменная столкновения со стеной (да\нет)
			double RayX = sinf(CurrentAngle);														// единичный вектор луча по Х
			double RayY = cosf(CurrentAngle);														// единичный вектор луча по У
			thr_dist = RayCast(thr_dist, RayX, RayY, Collision, map);								// вызываем алгоритм Ray Casting и узнаем дистанцию до стены в конкретном направлении
			luma = ((DOV - thr_dist) / DOV);														// считаем яркость вертикальной полоски
			size = Screen_H/2*luma;																	// считаем половину высоты верткальной полоски (без коэфициента)
			sf::RectangleShape rect;																// объявляем прямоугольник
			rect.setSize(sf::Vector2f(1, 2.4 * size));												// задаем ему ширину 1 пиксель (полоску) и заранее заготовленную длину с коэфицентом
			rect.setPosition(sf::Vector2f(x, int(Screen_H / 2 - 1.2*size)));						// ставим полоску в нужное место на экране
			rect.setFillColor(sf::Color(255*luma, 255 * luma, 255 * luma));							// раскрашиваем полоску в зависимости от удаленности
			window.draw(rect);																		// отрисовываем полоску
		}
		window.display();																			// выводим на экран все полоски из цикла
		Sleep(40);																					// задержка в 1/25 секунды чтобы не перегружать компьютер и добиться большей стабильности
	}
	return 0;
}