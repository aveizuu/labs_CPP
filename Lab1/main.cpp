#include <iostream>
#include <cmath>
#include <clocale>
using namespace std;

// Функция, которая решает линейное уравнение с данными коэффициентами
void solveLinear(double b, double c) {
	if (b == 0) {
		if (c == 0) {
			cout << "Уравнение имеет бесконечное число решений";
		}
		else {
			cout << "Уравнение не имеет решений";
		}
	}
	else {
		cout << "Уравнение имеет коорень x = " << -c/b;
	}
}

// Функция, которая считает дискриминант
double calculateDiscriminant(double a, double b, double c) {
	return b * b - 4.0 * a * c;
}

// Функция для решения квадратного уравнения с положительным дискриминантом
void solveTwoRoots(double a, double b, double discriminant){
	double x1 = (-b + sqrt(discriminant)) / (2.0 * a);
	double x2 = (-b - sqrt(discriminant)) / (2.0 * a);
	cout << "Квадратное уравнение имеет два действительных корня:\nx1 = " << x1 << " x2 = " << x2;
}

// Функция для решения квадратного уравнения с дискриминантом равным нулю
void solveOneRoots(double a, double b) {
	double x1 = -b / (2.0 * a);
	cout << "Квадратное уравнение имеет один действительный корень:\nx1 = " << x1;
}

int main() {
	setlocale(LC_ALL, "Russian");
	double a, b, c;

	cout << "Введите коэффициенты квадратного уравнения a, b и c:";
	cin >> a >> b >> c;

	if (a == 0) {
		// Уравнение линейное
		solveLinear(b, c);
	}
	// Иначе, уравнение квадратное
	else {
		double discriminant = calculateDiscriminant(a, b, c);
		
		if (discriminant > 0) {
			solveTwoRoots(a, b, discriminant); // Два корня
		}
		else if (discriminant == 0) {
			solveOneRoots(a, b); // Один корень
		}
	}
	
	return 0;
}