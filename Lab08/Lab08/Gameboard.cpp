#include "Gameboard.h"
#include <iomanip>
#include <vector>
#include <cassert>

Gameboard::Gameboard() { empty(); }

void Gameboard::empty() {
    for (int y{ 0 }; y < MAX_Y; y++) {
        for (int x{ 0 }; x < MAX_X; x++) {
            grid[y][x] = EMPTY_BLOCK;
        }
    }
}

void Gameboard::printToConsole() const {
    for (int y{ 0 }; y < MAX_Y; y++) {
        for (int x{ 0 }; x < MAX_X; x++) {
            if (grid[y][x] == EMPTY_BLOCK) {
                std::cout << '.' << std::setw(2);
            }
            else {
                std::cout << grid[y][x] << std::setw(2);
            }
        }
        std::cout << '\n';
    }
}

int Gameboard::getContent(const Point& coordinate) const {
    assert(isValidPoint(coordinate), "Invalid point");
    return grid[coordinate.getY()][coordinate.getX()];
}

int Gameboard::getContent(int x, int y) const {
    assert(isValidPoint(x, y), "Invalid Point");
    return grid[y][x];
}

void Gameboard::setContent(const Point& newPoint, int content) {
    if (!isValidPoint(newPoint)) {
        return;
    }
    grid[newPoint.getY()][newPoint.getX()] = content;
}

void Gameboard::setContent(int x, int y, int content) {
    if (isValidPoint(x, y)) {
        grid[y][x] = content;
    }
}

void Gameboard::setContent(const std::vector<Point>& locations, int content) {
    for (const Point& point : locations) {
        if (isValidPoint(point)) {
            grid[point.getY()][point.getX()] = content;
        }
    }
}

bool Gameboard::areAllLocsEmpty(const std::vector<Point>& points) const {
    for (auto& point : points) {
        if (isValidPoint(point)) {
            if (grid[point.getY()][point.getX()] != EMPTY_BLOCK) {
                return false;
            }
        }
    }
    return true;
}

int Gameboard::removeCompletedRows() {
    std::vector<int> rows{ getCompletedRowIndices() };
    removeRows(rows);
    return rows.size();
}

bool Gameboard::isValidPoint(const Point& item) const {
    return isValidPoint(item.getX(), item.getY());
}

bool Gameboard::isValidPoint(int x, int y) const {
    bool y_works = (y >= 0) && (y < MAX_Y);
    bool x_works = (x >= 0) && (x < MAX_X);

    return y_works && x_works;
}

bool Gameboard::isRowCompleted(int row) const {
    for (int i{ 0 }; i < MAX_X; i++) {
        if (grid[row][i] == EMPTY_BLOCK) {
            return false;
        }
    }
    return true;
}

void Gameboard::fillRow(int row, int content) {
    for (int i{ 0 }; i < MAX_X; i++) {
        grid[row][i] = content;
    }
}

std::vector<int> Gameboard::getCompletedRowIndices() const {
    std::vector<int> rows{};
    for (int row{ 0 }; row < MAX_Y; row++) {
        if (isRowCompleted(row)) {
            rows.push_back(row);
        }
    }
    return rows;
}

void Gameboard::copyRowIntoRow(int copyFrom, int copyInto) {
    for (int i{ 0 }; i < MAX_X; i++) {
        grid[copyInto][i] = grid[copyFrom][i];
    }
}

void Gameboard::removeRow(int row) {
    if (row >= MAX_Y) {
        return;
    }
    for (int y{ row - 1 }; y >= 0; y--) {
        copyRowIntoRow(y, y + 1);
    }
    fillRow(0, EMPTY_BLOCK);
}

void Gameboard::removeRows(const std::vector<int>& rows) {
    for (int row : rows) {
        removeRow(row);
    }
}
