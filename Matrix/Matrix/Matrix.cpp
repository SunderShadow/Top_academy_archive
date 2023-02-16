#include <iostream>

using namespace std;

template <int ROWS, int COLS>
class Matrix
{
    int matrix[ROWS][COLS];
public:
    Matrix()
    {
        for (auto &row: this->matrix)
            for (auto& it : row)
                it = 0;
    }

    Matrix(int matrix[ROWS][COLS]): matrix(matrix)
    {}

    void dump()
    {
        cout << '[' << endl;
        for (int i = 0; i < ROWS; i++)
        {
            cout << '\t';

            for (int j = 0; j < COLS; j++)
            {
                cout << this->matrix[i][j] << ", ";
            }

            cout << endl;
        }
        cout << ']' << endl;
    }

    operator int* ()
    {
        return this->matrix;
    }
};

template <int ROWS, int COLS>
Matrix<ROWS, COLS>& operator+(Matrix<ROWS, COLS>& m1, Matrix<ROWS, COLS>& m2)
{
    auto newMatrix = new Matrix<ROWS, COLS>;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            newMatrix[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return *newMatrix;
}

int main()
{
    Matrix<3, 3> a;
    Matrix<3, 3> b;

    (a + b).dump();
}