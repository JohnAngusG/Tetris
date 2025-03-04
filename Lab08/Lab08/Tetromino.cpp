#include "Tetromino.h"
#include "random"

Tetromino::Tetromino() {
    setShape(TetShape::I);
}

int Tetromino::color_counter = 0;
void Tetromino::setShape(TetShape shape) {
    this->shape = shape;
    int color_index = color_counter % static_cast<int>(TetColor::COUNT);

    color = static_cast<TetColor>(color_index);
    color_counter++;

    switch (shape) {

    case TetShape::O:
        blockLocs = { Point(0, 1), Point(1, 1), Point(0, 0), Point(1, 0) };
        break;

    case TetShape::I:
        blockLocs = { Point(0, -1), Point(0, 0), Point(0, 1), Point(0, 2) };
        break;

    case TetShape::S:
        blockLocs = { Point(-1, 0), Point(0, 0), Point(0, 1), Point(1, 1) };
        break;

    case TetShape::Z:
        blockLocs = { Point(-1, 1), Point(0, 1), Point(0, 0), Point(1, 0) };
        
        break;

    case TetShape::L:
        blockLocs = { Point(0, 1), Point(0, 0), Point(0, -1), Point(1, -1) };
        break;

    case TetShape::J:
        blockLocs = { Point(-1, -1), Point(0, -1), Point(0, 0), Point(0, 1) };
        break;

    case TetShape::T:
        blockLocs = { Point(-1, 0), Point(0, 0), Point(0, -1), Point(1, 0) };
        break;
    }
}

void Tetromino::rotateClockwise() {
    if (shape == TetShape::O) {
        return;
    }

    for (auto& coordinate : blockLocs) {
        coordinate.swapXY();
        coordinate.multiplyY(-1);
    }
}

void Tetromino::printToConsole() const {
    for (int y{ 3 }; y >= -3; y--) {
        std::string row = "";
        for (int x{ -3 }; x <= 3; x++) {
            bool found = false;
            for (auto coordinate : blockLocs) {
                if ((x == coordinate.getX()) && (y == coordinate.getY())) {
                    found = true;
                }
            }
            if (found) {
                row.push_back('*');
            }
            else {
                row.push_back('.');
            }
        }
        std::cout << row << '\n';
    }
}


TetShape Tetromino::getRandomShape() {  
    int new_shape = rand() % static_cast<int>(TetShape::COUNT);
    return static_cast<TetShape>(new_shape);
}
