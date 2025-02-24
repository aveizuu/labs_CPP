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
		cout << discriminant;
	}
	
	return 0;
}