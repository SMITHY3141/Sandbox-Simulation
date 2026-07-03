

#include "vectors.hpp"
#include <cmath>


Vector operator+(const Vector &self, const Vector &other) {
    return {self.x + other.x, self.y + other.y};

}

Vector operator-(const Vector &self, const Vector &other) {
    return {self.x - other.x, self.y - other.y};
}

Vector operator*(const Vector &self, float value) {
    return {self.x * value, self.y * value};
}

Vector operator*(float value, const Vector &self) {
    return self * value;
}

Vector operator/(const Vector &self, float value) {
    return {self.x / value, self.y / value};
}

Vector operator/(float value, const Vector &self) {
    return self * value;
}

float Vector::dot(const Vector &other) const {
    return this->x * other.x + this->y * other.y;
}

float Vector::length() const {
    return std::sqrt(this->dot(*this));
}

Vector Vector::normalised() const {
    return Vector(*this) / this->length();
}

Vector& Vector::operator+=(const Vector &other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}
Vector& Vector::operator-=(const Vector &other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
Vector& Vector::operator*=(float value) {
    this->x *= value;
    this->y *= value;
    return *this;
}
Vector& Vector::operator/=(float value) {
    this->x /= value;
    this->y /= value;
    return *this;
}


Vector Vector::fromPolar(float magnitude, float angle) {
    return {std::cos(angle) * magnitude, std::sin(angle) * magnitude};
}
