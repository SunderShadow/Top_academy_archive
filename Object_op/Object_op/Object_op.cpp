#include <iostream>
#include <math.h>

using namespace std;
using std::cin;
using std::cout;

class Point
{
private:
    double x, y;
public: 
    // ------
    // Setters
    void setX(double x)
    {
        this->x = x;
    }

    void setY(double y)
    {
        this->y = y;
    }

    // ------
    // Getters
    double getX()const
    {
        return this->x;
    }

    double getY()const
    {
        return this->y;
    }
    
    // ------
    // Other methods
    double distance(Point point)const
    {
        double x_diff = this->getX() - point.getX();
        double y_diff = this->getY() - point.getY();

        return sqrt(pow(x_diff, 2) + pow(y_diff, 2));
    }

    void dump()
    {
        cout
            << "X: " << this->getX() << endl
            << "Y: " << this->getY() << endl;
    }

    // ------
    // Operators
    Point operator+(Point& point)
    {
        Point pointSum;

        pointSum.setX(this->getX() + point.getX());
        pointSum.setY(this->getX() + point.getX());

        return pointSum;
    }
};

std::istream& operator>>(std::istream& stream, Point& p)
{
    cout << "Enter X, Y: ";

    int x, y;
    stream >> x >> y;

    p.setX(x);
    p.setY(y);

    return stream;
}

int main()
{
    Point p;

    std::cin >> p;

    p.dump();
}
