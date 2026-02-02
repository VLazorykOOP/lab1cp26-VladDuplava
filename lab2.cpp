#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

// Структура для координат та векторів
struct Point {
    double x;
    double y;
};

class Car {
public:
    Point startPos;
    Point currentPos;
    Point targetPos;
    Point direction;
    double velocity;
    double timeElapsed;
    double totalTimeNeeded;
    bool isMoving;
    std::string type;

    Car(std::string t, double startX, double startY, double targetX, double targetY, double V) {
        type = t;
        startPos = {startX, startY};
        currentPos = startPos;
        targetPos = {targetX, targetY};
        velocity = V;
        timeElapsed = 0;
        isMoving = true;

        // Перевірка: якщо машина вже в цільовій області
        if (startX == targetX && startY == targetY) {
            isMoving = false;
        }

        // 1. Обчислення вектора напрямку (Нормалізація) згідно з формулою:
        // (dx; dy) = (x2-x1; y2-y1) / sqrt((x2-x1)^2 + (y2-y1)^2)
        double dx_raw = targetPos.x - startPos.x;
        double dy_raw = targetPos.y - startPos.y;
        double distance = std::sqrt(dx_raw * dx_raw + dy_raw * dy_raw);

        if (distance > 0) {
            direction.x = dx_raw / distance;
            direction.y = dy_raw / distance;
            // 2. Розрахунок часу, необхідного для подолання відрізка
            totalTimeNeeded = distance / velocity;
        } else {
            isMoving = false;
        }
    }

    void update(double deltaTime) {
        if (!isMoving) return;

        timeElapsed += deltaTime;

        // Умова продовження руху: T < часу, який потрібен, щоб пройти відрізок
        if (timeElapsed >= totalTimeNeeded) {
            currentPos = targetPos;
            isMoving = false;
            std::cout << type << " прибула в кінцеву точку.\n";
        } else {
            // Формула з тексту: (xc; yc) = (x1; y1) + (dx; dy) * V * T
            currentPos.x = startPos.x + direction.x * velocity * timeElapsed;
            currentPos.y = startPos.y + direction.y * velocity * timeElapsed;
        }
    }
};

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Ukrainian");

    // Параметри області симуляції
    double w = 800; // Ширина
    double h = 600; // Довжина
    double V = 50;  // Швидкість (піксель/сек)

    // --- ВАРІАНТ 5 ---

    // 1. Вантажна машина: рух у ліву верхню чверть [0;0] - [w/2; h/2]
    double truckTargetX = (rand() % (int)(w / 2));
    double truckTargetY = (rand() % (int)(h / 2));
    Car truck("Вантажівка", 500, 400, truckTargetX, truckTargetY, V);

    // 2. Легкова машина: рух у праву нижню чверть [w/2; h/2] - [w; h]
    double carTargetX = (w / 2) + (rand() % (int)(w / 2));
    double carTargetY = (h / 2) + (rand() % (int)(h / 2));
    Car sedan("Легковик", 100, 100, carTargetX, carTargetY, V);

    std::cout << "Симуляція руху за Варіантом 5 розпочата...\n";

    // Цикл симуляції (крок 0.5 секунди)
    double dt = 0.5;
    for (int step = 0; step < 20; ++step) {
        std::cout << "--- Секунда " << step * dt << " ---\n";

        truck.update(dt);
        sedan.update(dt);

        if (truck.isMoving)
            std::cout << truck.type << ": (" << truck.currentPos.x << ", " << truck.currentPos.y << ")\n";
        if (sedan.isMoving)
            std::cout << sedan.type << ": (" << sedan.currentPos.x << ", " << sedan.currentPos.y << ")\n";

        if (!truck.isMoving && !sedan.isMoving) break;
    }

    return 0;
}
