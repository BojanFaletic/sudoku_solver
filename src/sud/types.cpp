#include "types.hpp"
#include <fmt/format.h>
#include <sstream>

namespace sud
{
    // Point
    Point Point::operator*(const square_t &value) const
    {
        return Point(row * value, col * value);
    }

    Point Point::operator+(const Point &other) const
    {
        return Point(row + other.row, col + other.col);
    }

    std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << fmt::format("({}, {})", point.row, point.col);
        return os;
    }

    bool Point::operator==(const Point &other) const
    {
        return row == other.row && col == other.col;
    }


    // PointIterator
    PointIterator &PointIterator::operator++()
    {
        if (point.col == SUDOKU_SIZE - 1)
        {
            point.col = 0;
            point.row++;
        }
        else
        {
            point.col++;
        }
        return *this;
    }

    bool PointIterator::operator!=(const PointIterator &other) const
    {
        return point.col != other.point.col || point.row != other.point.row;
    }

    Point PointIterator::operator*() const
    {
        return point;
    }

    PointIterator PointIterator::begin() const
    {
        return PointIterator({0, 0});
    }

    PointIterator PointIterator::end() const
    {
        return PointIterator({SUDOKU_SIZE, 0});
    }

}; // namespace sud
