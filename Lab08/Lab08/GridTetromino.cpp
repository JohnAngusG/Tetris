#include "GridTetromino.h"

GridTetromino::GridTetromino() {
	gridLoc.setXY(0, 0);
}

void GridTetromino::setGridLoc(int x, int y) {
	gridLoc.setXY(x, y);
}

void GridTetromino::setGridLoc(const Point& newLoc) {
	gridLoc.setXY(newLoc.getX(), newLoc.getY());
}

void GridTetromino::move(int xOffset, int yOffset) {
	gridLoc.setXY(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const {
	std::vector<Point> result{};
	for (const Point& coordinate : blockLocs) {
		result.push_back(Point(coordinate.getX() + gridLoc.getX(), coordinate.getY() + gridLoc.getY()));
	}
	return result;
}
