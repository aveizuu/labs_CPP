#ifndef MAIN_H 
#define MAIN_H

#include <iostream>
#include <cmath>

// Функция, которая решает линейное уравнение с данными коэффициентами
void solveLinear(double b, double c);

// Функция, которая считает дискриминант
double calculateDiscriminant(double a, double b, double c);

// Функция для решения квадратного уравнения с положительным дискриминантом
void solveTwoRoots(double a, double b, double discriminant);

// Функция для решения квадратного уравнения с дискриминантом равным нулю
void solveOneRoot(double a, double b);

// Функция для решения квадратного уравнения с дискриминантом меньше нуля
void solveComplexRoots(double a, double b, double discriminant);

#endif
