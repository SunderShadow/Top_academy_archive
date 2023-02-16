#include <iostream>
using namespace std;

#define DEBUG 1

// Ставит числа в правильные позиции (меньшее слева, большее справа)
void setNumRightPos(int& lower, int& higher)
{
    if (lower > higher) {
        swap(lower, higher);
    }
}

// Алгоритм евклида
int getGreatesCommonDivisor(int lower, int higher)
{
    setNumRightPos(lower, higher);

    if (lower == higher || !(higher % lower)) {
        return lower;
    }

    int remainder = -1;

    do
    {
        remainder = higher % lower;

        higher = lower;
        lower = remainder;
    } while (remainder);

    return higher;
}

class Fraction;

Fraction operator+ (Fraction, Fraction);
Fraction operator- (Fraction, Fraction);
Fraction operator* (Fraction, Fraction);
Fraction operator/ (Fraction, Fraction);

class Fraction
{
protected:
    int integer, nominator, denominator;
    bool proper;
public:
    // -----
    // Constructor
    Fraction(int int_num = 0, int nominator = 0, int denominator = 1)
    {
        (*this)
            .setInteger(int_num)
            .setNominator(nominator)
            .setDenominator(denominator)
            .to_proper()
            .reduce();
    }

    Fraction(double num)
    {
        int integer = num;
        double remainder = num - integer;

    // -------
    // Setters
    
    Fraction setInteger(int integer)
    {
        this->integer = integer;
        return *this;
    }

    Fraction setNominator(int nominator)
    {
        this->nominator = nominator;
        return *this;
    }

    Fraction setDenominator(int denominator)
    {
        this->denominator = denominator;
        return *this;
    }

    // -------
    // Getters
    bool isProper()const
    {
        return this->proper;
    }

    int getInteger()const
    {
        return this->integer;
    }

    int getNominator()const
    {
        return this->nominator;
    }

    int getDenominator()const
    {
        return this->denominator;
    }

    // -------
    // Operators
    Fraction& operator++ ()
    {
        this->integer++;
        return *this;
    }

    Fraction& operator-- ()
    {
        this->integer++;
        return *this;
    }

    // Assignment operators section \\

    Fraction& operator= (Fraction f)
    {
        this->integer     = f.getInteger();
        this->nominator   = f.getNominator();
        this->denominator = f.getDenominator();
    }

    Fraction& operator*= (Fraction& f) {
        return *this = *this * f;
    }

    Fraction& operator-= (Fraction& f) {
        return *this = *this - f;
    }

    Fraction& operator+= (Fraction& f) {
        return *this = *this + f;
    }

    Fraction& operator/= (Fraction& f) {
        return *this = *this / f;
    }

    // -------
    // Main methods
    Fraction& to_improper()
    {
        if (this->denominator == 0)
        {
            this->denominator = 1;
        }

        this->proper = false;

        this->nominator += this->integer * this->denominator;
        this->integer = 0;

        return *this;
    }

    Fraction& to_proper()
    {
        this->proper = true;

        if (this->denominator != 0)
        {
            int divisionInteger = this->nominator / this->denominator;

            this->integer += divisionInteger;
            this->nominator -= divisionInteger * this->denominator;
        }

        return *this;
    }

    Fraction& reduce()
    {
        if (this->nominator == 0) {
            this->denominator = 0;
            return *this;
        }

        int GCD = getGreatesCommonDivisor(this->nominator, this->denominator);
        
        this->nominator /= GCD;
        this->denominator /= GCD;

        return *this;
    }

    Fraction& flip()
    {
        std::swap(this->nominator, this->denominator);

        return *this;
    }

    Fraction& cast(int multiplier)
    {
        this->nominator   *= multiplier;
        this->denominator *= multiplier;

        return *this;
    }

    operator double()
    {
        return this->integer + (double)this->nominator / (double)this->denominator;
    }

#if DEBUG
    // ------
    // Debug heleprs
    Fraction& dump()
    {
        cout << "Number(" << (this->proper ? "" : "im") << "proper): " << this->getInteger() << ' ';

        if (this->getNominator()) {
            cout << this->getNominator() << '/' << this->getDenominator() << endl;
        }

        return *this;
    }
#endif
};

istream& operator>>(istream& input, Fraction& f)
{
    int i, n, d;

    cout << "Enter integer: ";
    input >> i;

    cout << "Enter nominator: ";
    input >> n;

    cout << "Enter denominator: ";
    input >> d;

    return input;
}

void castToEqualDenominators(Fraction& f1, Fraction& f2)
{
    if (f1.getDenominator() % f2.getDenominator() == 0)
    {
        f2.cast(f1.getDenominator() / f2.getDenominator());
    }
    else
    {
        int tmpDenominator = f1.getDenominator();
        f1.cast(f2.getDenominator());
        f2.cast(tmpDenominator);
    }
}

Fraction operator+ (Fraction f1, Fraction f2)
{
    castToEqualDenominators(f1, f2);

    return Fraction (
        f1.getInteger() + f2.getInteger(),
        f1.getNominator() + f2.getNominator(),
        f1.getDenominator()
    ).reduce();
}

Fraction operator- (Fraction f1, Fraction f2)
{
    castToEqualDenominators(f1, f2);

    return Fraction(
        f1.getInteger() - f2.getInteger(),
        f1.getNominator() - f2.getNominator(),
        f1.getDenominator()
    ).reduce();
}

Fraction operator* (Fraction f1, Fraction f2)
{
    f1.to_improper();
    f2.to_improper();

    return Fraction(0,
        f1.getNominator()   * f2.getNominator(),
        f1.getDenominator() * f2.getDenominator()
    ).reduce();
}

Fraction operator/ (Fraction f1, Fraction f2)
{
    f2.to_improper();
    return f1 * f2.flip();
}

bool operator== (Fraction f1, Fraction f2)
{
    f1.to_improper();
    f2.to_improper();

    castToEqualDenominators(f1, f2);

    // Т.к. Делители одинаковы можно сравнивать только делимое
    return f1.getNominator() == f2.getNominator();
}

bool operator> (Fraction f1, Fraction f2)
{
    f1.to_improper();
    f2.to_improper();

    castToEqualDenominators(f1, f2);

    // Т.к. Делители одинаковы можно сравнивать только делимое
    return f1.getNominator() > f2.getNominator();
}

bool operator< (Fraction f1, Fraction f2)
{
    f1.to_improper();
    f2.to_improper();

    castToEqualDenominators(f1, f2);

    // Т.к. Делители одинаковы можно сравнивать только делимое
    return f1.getNominator() < f2.getNominator();
}

bool operator>= (Fraction f1, Fraction f2)
{
    f1.to_improper();
    f2.to_improper();

    castToEqualDenominators(f1, f2);

    // Т.к. Делители одинаковы можно сравнивать только делимое
    return f1 == f2 || f1 > f2;
}

bool operator<= (Fraction f1, Fraction f2)
{
    f1.to_improper();
    f2.to_improper();

    castToEqualDenominators(f1, f2);

    // Т.к. Делители одинаковы можно сравнивать только делимое
    return f1 == f2 || f1 < f2;
}

#define HOME_WORK_1
#define HOME_WORK_2

int main () 
{
#ifdef HOME_WORK_1
    Fraction B(2, 3, 4);
    double b = B;
    cout << b << endl;
#endif // HOME_WORK_1

#ifdef HOME_WORK_2
    Fraction B = 2.75;
    cout << B << endl;
#endif // HOME_WORK_2
    return 0;
}