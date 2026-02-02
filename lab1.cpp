#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <exception>

using namespace std;

/* ================= ВЛАСНІ ВИНЯТКИ ================= */

class FileException : public exception {
    string msg;
public:
    explicit FileException(string m) : msg(std::move(m)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

/* ================= КЛАС ТАБЛИЦІ ================= */

class Table {
    vector<double> x, y;

public:
    void load(const string& filename) {
        ifstream f(filename);
        if (!f.is_open())
            throw FileException("Файл не відкрився: " + filename);

        double a, b;
        while (f >> a >> b) {
            x.push_back(a);
            y.push_back(b);
        }
        f.close();

        if (x.size() < 2)
            throw FileException("Недостатньо даних у файлі: " + filename);
    }

    double interpolate(double value) const {
        for (size_t i = 0; i + 1 < x.size(); i++) {
            if (x[i] <= value && value <= x[i + 1]) {
                return y[i] +
                       (y[i + 1] - y[i]) * (value - x[i]) /
                       (x[i + 1] - x[i]);
            }
        }
        return 0;
    }
};

/* ================= АЛГОРИТМ 1 ================= */

class Algorithm1 {
public:
    static double Qnr(double x, double y) {
        if (y == 0) return 1;

        if (x > y && 10 * pow(y, 4) - x >= 0)
            return x * x * sqrt(10 * pow(y, 4) - x);

        if (x <= y && 3 * x > y && 10 * pow(x, 4) - y >= 0)
            return pow(x, 3) * log(10 * pow(x, 4) - y);

        if (x <= y && 3 * x <= y && pow(y, 4) - 2 * x >= 0)
            return y * y * sqrt(pow(y, 4) - 2 * x);

        throw logic_error("Algorithm1::Qnr – недопустимі аргументи");
    }

    static double Qnk(double x, double y) {
        return 10.5 * Qnr(2 * x, y) - 3.75 * Qnr(x, 2 * y);
    }

    static double Rnk(double x, double y, double z) {
        return x * Qnk(x, y) + y * Qnk(y, x) + z * Qnk(z, x);
    }
};

/* ================= АЛГОРИТМ 2 ================= */

class Algorithm2 {
public:
    static double Qnr(double x, double y) {
        if (y == 0) return 1;
        if (x > y) return x * x * (10 * y * y - x / 2);
        if (x <= y && 3 * x > y) return pow(x, 4) * y;
        return pow(y, 4) * x;
    }

    static double Qnk(double x, double y) {
        return 12 * Qnr(2.5 * x, y) - 3 * Qnr(x, 1.5 * y);
    }

    static double Rnk(double x, double y, double z) {
        return 1.15 * x * Qnk(x, y)
             + 0.95 * y * Qnk(y, x)
             + 0.9 * z * Qnk(z, x);
    }
};

/* ================= АЛГОРИТМ 3 ================= */

class Algorithm3 {
public:
    static double U1(double x) { return atan(asin(sin(3 * x))); }
    static double T1(double x) { return atan(acos(sin(2 * x))); }

    static double Wnr(double x, double y) {
        if (x > y)
            return T1(x) - 0.9 * U1(x) * U1(y);
        return 0.9 * T1(x) * T1(y) - U1(x);
    }

    static double Wnk(double x, double y) {
        return 10 * Wnr(2.5 * x, y) - 4 * Wnr(x, 2.5 * y);
    }
};

/* ================= АЛГОРИТМ 4 ================= */

class Algorithm4 {
    Table U2, T2;

public:
    Algorithm4(const Table& u, const Table& t) : U2(u), T2(t) {}

    double Wnr(double x, double y) const {
        if (x > y)
            return 0.9 * T2.interpolate(x)
                 - U2.interpolate(x) * U2.interpolate(y);
        return T2.interpolate(x) * T2.interpolate(y)
             - 0.9 * U2.interpolate(x);
    }

    double Wnk(double x, double y) const {
        return 10 * Wnr(x, y) - 3 * Wnr(x, y);
    }

    double Gnk(double x, double y, double z) const {
        double s = x*x + y*y + z*z;
        if (s < 0.001) return 0;

        double r = sqrt(s);
        return x * Wnk(x / r, y / r)
             + y * Wnk(y / r, x / r)
             + z * Wnk(z / r, x / r);
    }
};

/* ================= TFUN / FUNC ================= */

class FuncCalculator {
public:
    static double Tfun(double u, double v, const string& text) {
        ifstream f("dat3.dat");
        int r = 0;

        if (f.is_open()) {
            string w; int val;
            while (f >> w >> val)
                if (w == text) { r = val; break; }
            f.close();
        }

        return u*u + v*v - r*(u + v) + r*r;
    }

    static double func(double u, double v, const string& text) {
        if (fabs(u) <= 0.5)
            return Tfun(0, v, text);

        if (u < v)
            return Tfun(u, v, text);

        return Tfun(u, 0, text) - Tfun(0, v, "set");
    }
};

int main1() {
    try {
        double x, y, z;
        string text;
        cin >> x >> y >> z >> text;

        /* --- u --- */
        double u = Algorithm1::Rnk(x, y, z);

        /* --- v з перевірками |x| ≤ 10 --- */
        double v;
        try {
            if (abs(x) <= 10) {
                Table t1, t2;
                t1.load("dat1.dat");
                t2.load("dat2.dat");
                Algorithm4 alg4(t1, t2);
                v = alg4.Gnk(x, y, z);
            } else {
                v = Algorithm3::Wnk(x, 2 * z);
            }
        }
        catch (const FileException&) {
            v = Algorithm3::Wnk(x, 2 * z);
        }

        double result = FuncCalculator::func(u, v, text);
        cout << "Result = " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Помилка: " << e.what() << endl;
    }

    return 0;
}
