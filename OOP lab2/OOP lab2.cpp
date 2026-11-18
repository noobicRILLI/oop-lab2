#include <iostream>
using namespace std;

class Point {
protected:
    int x;
    int y;

public:
    Point() : x(0), y(0) {
        cout << "Point() [x=" << x << ", y=" << y << "]" << endl;
    }

    Point(int x, int y) : x(x), y(y) {
        cout << "Point(int x, int y) [x=" << this->x << ", y=" << this->y << "]" << endl;
    }

    Point(const Point& p) : x(p.x), y(p.y) {
        cout << "Point(const Point& p) [x=" << x << ", y=" << y << "]" << endl;
    }

    ~Point() {
        cout << "~Point() [x=" << x << ", y=" << y << "]" << endl;
    }

    void print() {
        cout << "Point: (" << x << ", " << y << ")" << endl;
    }

    void setXY(int newX, int newY);
    void move(int dx, int dy);
};

void Point::setXY(int newX, int newY) {
    x = newX;
    y = newY;
}

void Point::move(int dx, int dy) {
    x += dx;
    y += dy;
}
class ColoredPoint : public Point {
private:
    int color;

public:
    ColoredPoint() : Point(), color(0) {
        cout << "ColoredPoint() [color=" << color << "]" << endl;
    }

    ColoredPoint(int x, int y, int color) : Point(x, y), color(color) {
        cout << "ColoredPoint(int, int, int) [color=" << this->color << "]" << endl;
    }

    ColoredPoint(const ColoredPoint& p) : Point(p), color(p.color) {
        cout << "ColoredPoint(const ColoredPoint& p) [color=" << color << "]" << endl;
    }

    ~ColoredPoint() {
        cout << "~ColoredPoint() [x=" << x << ", y=" << y << ", color=" << color << "]" << endl;
    }

    void changeColor(int newColor) {
        color = newColor;
    }

    void print() {
        Point::print();
        cout << "  -> color = " << color << endl;
    }
};


int main() {
    cout << "=== 1. СТАТИЧЕСКИЕ И ДИНАМИЧЕСКИЕ ОБЪЕКТЫ Point ===" << endl;
    {
        Point p1;             // Конструктор без параметров
        Point p2(10, 20);     // Конструктор с параметрами
        Point p3(p2);         // Конструктор копирования

        p1.setXY(5, 5);
        p1.print();
        p2.print();
        p3.print();
    } // Здесь автоматически вызываются деструкторы p3, p2, p1 (в обратном порядке)

    cout << "\n--- Динамический Point ---" << endl;
    Point* dynP = new Point(7, 8);
    dynP->move(2, 2);
    dynP->print();
    delete dynP; // Ручное уничтожение

    cout << "\n=== 2. НАСЛЕДОВАНИЕ (ColoredPoint) ===" << endl;
    {
        ColoredPoint cp1;
        ColoredPoint cp2(1, 2, 255);
        ColoredPoint cp3(cp2);
        cp3.changeColor(128);
        cp3.print();
    } // Сначала ~ColoredPoint(), затем ~Point()

    cout << "\n=== 3. КОМПОЗИЦИЯ: ПО ЗНАЧЕНИЮ VS ПО УКАЗАТЕЛЮ ===" << endl;
    {
        cout << "-- По значению (сначала создаются поля Point, потом Line) --" << endl;
        LineByValue lVal(0, 0, 10, 10);
    }
    {
        cout << "\n-- По указателю (сначала тело Line, потом new Point) --" << endl;
        LineByPointer lPtr1(1, 1, 5, 5);
        LineByPointer lPtr2(lPtr1); // Проверка глубокого копирования
    }

    cout << "\n=== 4. КОПИРОВАНИЕ ОБЪЕКТОВ VS КОПИРОВАНИЕ УКАЗАТЕЛЕЙ ===" << endl;
    {
        // Копирование объектов (независимые сущности)
        Point a(10, 10);
        Point b = a;
        a.setXY(99, 99);
        cout << "Объекты после изменения a:" << endl;
        a.print(); // (99, 99)
        b.print(); // (10, 10) — не изменился

        // Копирование указателей (один объект в памяти)
        Point* ptrA = new Point(10, 10);
        Point* ptrB = ptrA; // Копируется только адрес!
        ptrA->setXY(77, 77);
        cout << "Указатели после изменения ptrA:" << endl;
        ptrA->print(); // (77, 77)
        ptrB->print(); // (77, 77) — изменился тоже!
        delete ptrA;   // Удаляем только ОДИН раз (delete ptrB вызвал бы ошибку)
    }

    cout << "\n=== 5. УКАЗАТЕЛЬ БАЗОВОГО КЛАССА НА ПОТОМКА ===" << endl;
    {
        ColoredPoint* child = new ColoredPoint(3, 4, 50);
        Point* basePtr = child; // Помещаем потомка в указатель базового класса

        cout << "Вызов print() через Point* (вызывается Point::print):" << endl;
        basePtr->print();

        cout << "Вызов print() через ColoredPoint* (вызывается ColoredPoint::print):" << endl;
        child->print();

        // Удаляем через указатель своего типа, так как ~Point() не виртуальный
        delete child;
    }

    return 0;
}
