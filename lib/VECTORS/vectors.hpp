


#ifndef VECTORS_HPP
#define VECTORS_HPP

// Only 2D

struct Vector {
    float x;
    float y;


    float dot(const Vector &other) const;
    float length() const;
    Vector normalised() const;

    Vector& operator+=(const Vector &other); // can be outside struct if need be
    Vector& operator-=(const Vector &other);
    Vector& operator*=(float value);
    Vector& operator/=(float value);

    static Vector fromPolar(float magnitude, float angle);


};

// trailing const since it does not modify the current object
Vector operator+(const Vector &self, const Vector &other);
Vector operator-(const Vector &self, const Vector &other);
Vector operator*(const Vector &self, float value);
Vector operator*(float value, const Vector &self);
Vector operator/(const Vector &self, float value);
Vector operator/(float value, const Vector &self);



#endif // VECTORS_HPP
