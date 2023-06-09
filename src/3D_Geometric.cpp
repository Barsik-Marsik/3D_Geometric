//============================================================================
// Name        : 3DGeometric.cpp
// Author      : Zorin Ilya
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <cmath>
#include <iostream>
#include <limits>
//#include <numbers>


#define EPS 1.0e-15

constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double pi = std::acos(-1);
// constexpr double pi = std::numbers::pi;


enum class CurveType {
	Circle,
	Ellipce,
	Helix
};

std::ostream& operator<<(std::ostream& output, const CurveType& type) {
	switch (type)
	{
		case CurveType::Circle:
			output << "Circle";
			break;
		case CurveType::Ellipce:
			output << "Ellipce";
			break;
		case CurveType::Helix:
			output << "Helix";
			break;
	}
	return output;
}

struct Point {
	double X;
	double Y;

	Point(double x = 0.0, double y = 0.0)
		: X(x)
		, Y(y)
	{
	}
};

Point operator+(const Point& p1, const Point& p2) {
	return {p1.X + p2.X, p1.Y + p2.Y};
}

std::ostream& operator<<(std::ostream& output, const Point& point) {
	output << "{" << point.X << ", " << point.Y << "}";
	return output;
}

class Curve {
public:
	Curve(CurveType type)
		: type_(type)
	{
	}

	virtual ~Curve() {}

	virtual Point GetPoint(double t) const {};

	virtual double GetDerivative(double t) const {};

	CurveType GetType() const {
		return type_;
	}

private:
	CurveType type_;
};


class Circle : public Curve{
public:
	Circle(double r)
		: Curve(CurveType::Circle)
		, center_({0.0, 0.0})
		, radius_(r)
	{
	}

	Circle(const Point& center = {0.0, 0.0}, double radius = 0.0)
		: Curve(CurveType::Circle)
		, center_(center)
		, radius_(radius)
	{
	}

	virtual ~Circle() {}

	double GetRadius() const {
		return radius_;
	}

	Point GetCenter() const {
		return center_;
	}

	// return a point per parameter t along the curve
	virtual Point GetPoint(double t) const {
		Point p;
		p.X = std::abs(std::cos(t)) > EPS ? radius_ * std::cos(t) : 0.0;
		p.Y = std::abs(std::sin(t)) > EPS ? radius_ * std::sin(t) : 0.0;
		return center_ + p;
	}

	// The derivate at a point is the tangent of the slope of tangent line
	// at that point. Equation of a tangent line: Ax + By + C = 0
	// Vector R{A; B} from the center of the circle to the point p
	virtual double GetDerivative(double t) const {
		Point p = GetPoint(t);
		double A = p.X - center_.X;
		double B = p.Y - center_.Y;
		double C = -A * p.X - B * p.Y;
		// If B == 0, the tangent line parallel to Y axis
		if (std::abs(B) < EPS) {
			return INF;
		}
		// If A == 0, the tangent line parallel to X axis
		if (std::abs(A) < EPS) {
			return 0.0;
		}
		// If C != 0, the tangent line cuts off segments a and b on the X and Y axes
		// tang = b/a
		if (std::abs(C) > EPS) {
			double a = -C / A;
			double b = -C / B;
			if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
				return -b / a;
			}
			return b / a;
		}
		// If C == 0, the tangent line passes through the point {0; 0}
		// Equation: Ax + By = 0 => y = -Ax / B
		// tang = -A / B
		return -A / B;
	}

private:
	Point center_;
	double radius_;
};

template<typename Curve>
void GetDerivateOfCurve(const Curve& curve, double t) {
	std::cout << "GetDerivate: " << curve.GetDerivative(t) << std::endl;
}

int main() {
	Circle circle_1(1);
	std::cout << "R=1, (0.0; 0.0), pi: " << circle_1.GetPoint(pi/6) << std::endl;

	Point p1 = {5.0, 5.0};
	Circle circle_2(p1, 5);
	std::cout << "R=5, (5.0; 5.0), pi: " << circle_2.GetPoint(pi/6) << std::endl;

	Curve curve_2(CurveType::Circle);
	GetDerivateOfCurve(circle_2, pi/4);

	return 0;
}
