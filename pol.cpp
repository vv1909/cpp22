#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

class Polynom {
private:
    std::vector<double> coeffs;
    std::vector<int> degrees;
    char variable;
    static int object_count;

public:
    // Конструктор без параметров
    Polynom() : variable('x') {
        coeffs.push_back(0.0);
        degrees.push_back(0);
        ++object_count;
    }

    // Конструктор с коэффициентами (степени = 1)
    explicit Polynom(const std::vector<double>& coeffs) : coeffs(coeffs), variable('x') {
        degrees.resize(coeffs.size(), 1);
        ++object_count;
    }

    // Конструктор со степенями (коэффициенты = 1)
    explicit Polynom(const std::vector<int>& degrees) : degrees(degrees), variable('x') {
        coeffs.resize(degrees.size(), 1.0);
        ++object_count;
    }

    // Полный конструктор
    Polynom(const std::vector<double>& coeffs, const std::vector<int>& degrees, char var = 'x')
        : coeffs(coeffs), degrees(degrees), variable(var) {
        if (coeffs.size() != degrees.size()) {
            throw std::invalid_argument("Coefficient and degree vectors must have the same size");
        }
        ++object_count;
    }

    // Конструктор копирования
    Polynom(const Polynom& other)
        : coeffs(other.coeffs), degrees(other.degrees), variable(other.variable) {
        ++object_count;
    }

    // Деструктор
    ~Polynom() {
        --object_count;
    }

    // Оператор присваивания
    Polynom& operator=(const Polynom& other) {
        if (this != &other) {
            coeffs = other.coeffs;
            degrees = other.degrees;
            variable = other.variable;
        }
        return *this;
    }

    // Методы set/get
    void setCoeffs(const std::vector<double>& newCoeffs) {
        coeffs = newCoeffs;
    }

    void setDegrees(const std::vector<int>& newDegrees) {
        degrees = newDegrees;
    }

    void setVariable(char var) {
        variable = var;
    }

    std::vector<double> getCoeffs() const {
        return coeffs;
    }

    std::vector<int> getDegrees() const {
        return degrees;
    }

    char getVariable() const {
        return variable;
    }

    // Статический метод
    static int getObjectCount() {
        return object_count;
    }

    // Вывод многочлена
    void print() const {
        bool first = true;
        bool allZero = true;

        for (size_t i = 0; i < coeffs.size(); ++i) {
            double c = coeffs[i];
            int d = degrees[i];

            if (c == 0.0) continue;
            allZero = false;

            if (!first) {
                if (c > 0) std::cout << " + ";
                else std::cout << " - ";
            } else {
                if (c < 0) std::cout << "-";
            }

            double absC = std::abs(c);
            if (d == 0) {
                std::cout << absC;
            } else {
                if (absC != 1.0 || d == 0) {
                    std::cout << absC;
                }
                std::cout << variable;
                if (d != 1) {
                    std::cout << "^" << d;
                }
            }
            first = false;
        }

        if (allZero) {
            std::cout << "0";
        }
        std::cout << std::endl;
    }

    // Вычисление производной порядка degree
    Polynom computeDerivative(int degree) const {
        if (degree < 0) {
            throw std::invalid_argument("Derivative degree cannot be negative");
        }

        Polynom result = *this;

        for (int n = 0; n < degree; ++n) {
            std::vector<double> newCoeffs;
            std::vector<int> newDegrees;

            for (size_t i = 0; i < result.coeffs.size(); ++i) {
                double c = result.coeffs[i];
                int d = result.degrees[i];
                if (d > 0) {
                    newCoeffs.push_back(c * d);
                    newDegrees.push_back(d - 1);
                }
            }

            if (newCoeffs.empty()) {
                newCoeffs.push_back(0.0);
                newDegrees.push_back(0);
            }

            result.coeffs = newCoeffs;
            result.degrees = newDegrees;
        }

        return result;
    }

    // Вычисление значения в точке
    double computeAt(double val) const {
        double result = 0.0;
        for (size_t i = 0; i < coeffs.size(); ++i) {
            result += coeffs[i] * std::pow(val, degrees[i]);
        }
        return result;
    }
};

// Определение статической переменной
int Polynom::object_count = 0;

// Глобальная функция
int getMaxPolynomIdx(const std::vector<Polynom>& polynoms, double val) {
    if (polynoms.empty()) return -1;

    int maxIdx = 0;
    double maxVal = polynoms[0].computeAt(val);
    for (size_t i = 1; i < polynoms.size(); ++i) {
        double curVal = polynoms[i].computeAt(val);
        if (curVal > maxVal) {
            maxVal = curVal;
            maxIdx = static_cast<int>(i);
        }
    }
    return maxIdx;
}

// Демонстрация
int main() {
    std::cout << "Initial object count: " << Polynom::getObjectCount() << std::endl;

    Polynom p1;
    std::cout << "p1: ";
    p1.print();

    std::vector<double> coeffs = {3, -2, 1};
    Polynom p2(coeffs);
    std::cout << "p2 (coeffs 3, -2, 1): ";
    p2.print();

    std::vector<int> degrees = {0, 2, 3};
    Polynom p3(degrees);
    std::cout << "p3 (degrees 0,2,3): ";
    p3.print();

    std::vector<double> coeffs2 = {2.5, -1.0, 0.5};
    std::vector<int> degrees2 = {2, 1, 0};
    Polynom p4(coeffs2, degrees2, 't');
    std::cout << "p4 (2.5 t^2 - t + 0.5): ";
    p4.print();

    std::cout << "Object count after constructors: " << Polynom::getObjectCount() << std::endl;

    p1.setVariable('y');
    p1.setCoeffs({5, 3});
    p1.setDegrees({2, 0});
    std::cout << "Modified p1: ";
    p1.print();

    double x = 2.0;
    std::cout << "p1 at " << x << " = " << p1.computeAt(x) << std::endl;

    Polynom p4_deriv = p4.computeDerivative(1);
    std::cout << "p4 first derivative: ";
    p4_deriv.print();

    Polynom p4_deriv2 = p4.computeDerivative(2);
    std::cout << "p4 second derivative: ";
    p4_deriv2.print();

    std::vector<Polynom> vec = {p1, p2, p3, p4};
    double point = 1.0;
    int idx = getMaxPolynomIdx(vec, point);
    std::cout << "Index of polynom with max value at " << point << " is " << idx << std::endl;

    std::cout << "Object count before exit: " << Polynom::getObjectCount() << std::endl;

    return 0;
}