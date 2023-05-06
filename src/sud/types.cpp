#include "types.hpp"

namespace sud
{
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
        return point.row == other.point.row && point.col == other.point.col;
    }

    Point PointIterator::operator*() const
    {
        return point;
    }

    PointIterator PointIterator::begin() const
    {
        return PointIterator(Point(0, 0));
    }

    PointIterator PointIterator::end() const
    {
        return PointIterator(Point(SUDOKU_SIZE, SUDOKU_SIZE));
    }

}; // namespace sud
