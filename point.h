#ifndef POINT_H
#define POINT_H

template <typename T> struct Point
{
    T x;
    T y;

    Point() = default;
    ~Point() = default;
    Point(const Point &) = default;
    Point &operator=(const Point &) = default;
    Point(T x, T y) : x(x), y(y)
    {
    }
    bool operator==(const Point &other) const noexcept
    {
        return x == other.x && y == other.y;
    }
    Point &operator+=(const Point &other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point operator+(const Point &other) const noexcept
    {
        Point tmp = *this;
        return tmp += other;
    }
    Point &operator-=(const Point &other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Point operator-(const Point &other) const noexcept
    {
        Point tmp = *this;
        return tmp -= other;
    }
    Point &operator*=(T a) noexcept
    {
        x *= a;
        y *= a;
        return *this;
    }
    Point operator*(T a) const noexcept
    {
        Point tmp = *this;
        return tmp *= a;
    }
    Point operator-() const noexcept
    {
        return Point{-x, -y};
    }
};

#endif // POINT_H