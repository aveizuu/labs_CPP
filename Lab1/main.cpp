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

int main() {
	setlocale(LC_ALL, "Russian");
	double a, b, c;

	cout << "Введите коэффициенты квадратного уравнения a, b и c:";
	cin >> a >> b >> c;

	if (a == 0) {
		// Уравнение линейное
		solveLinear(b, c);
	}

	
	return 0;
}