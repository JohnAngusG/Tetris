#include "TetrisGame.h"
#include <cassert>

// STATIC INITIALIZATIONS

const int TetrisGame::BLOCK_WIDTH{ 32 };
const int TetrisGame::BLOCK_HEIGHT{32};			  
const double TetrisGame::MAX_SECONDS_PER_TICK{0.75};   
const double TetrisGame::MIN_SECONDS_PER_TICK{0.20};		




// MEMBER FUNCTIONS


TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset) : window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset){
	reset();
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf")) {
		assert(false && "Missing font");
	}

	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(425, 325);
}


void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(ghostShape, gameboardOffset, 128);
	drawTetromino(nextShape, nextShapeOffset);
	window.draw(scoreText);
}


void TetrisGame::onKeyPressed(const sf::Event& event) {
	if (event.key.code == sf::Keyboard::Up) {
		attemptRotate(currentShape);
		updateGhostShape();
	}

	if (event.key.code == sf::Keyboard::Down) {
		if (attemptMove(currentShape, 0, 1)) {
			updateGhostShape();
		}
		else {
			lock(currentShape);
		}
	}

	if (event.key.code == sf::Keyboard::Left) {
		attemptMove(currentShape, -1, 0);
		updateGhostShape();
	}

	if (event.key.code == sf::Keyboard::Right) {
		attemptMove(currentShape, 1, 0);
		updateGhostShape();
	}

	if (event.key.code == sf::Keyboard::Space) {
		drop(currentShape);
		lock(currentShape);
		updateGhostShape();

		
	}

}

void TetrisGame::processGameLoop(float secondSinceLastLoop) {
	secondsSinceLastTick += secondSinceLastLoop;


	// it's time to tick
	if (secondsSinceLastTick > secondsPerTick) {
		tick();
		secondsSinceLastTick -= secondsPerTick;

		


	}

	// deal with placed shape
	if (shapePlacedSinceLastGameLoop) {
		shapePlacedSinceLastGameLoop = false;
		if (spawnNextShape()) {
			pickNextShape();
			int rowsCompleted = board.removeCompletedRows();

			switch (rowsCompleted)
			{
			case 1:
				score += 40;
				break;
			case 2:
				score += 100;
				break;
			case 3:
				score += 300;
				break;
			case 4:
				score += 1200;
				break;
			}
			determineSecondsPerTick();
			updateScoreDisplay();

		}
		else {
			reset();
		}
	}

}


void TetrisGame::tick() {
	if (attemptMove(currentShape, 0, 1)) {}
	else {
		lock(currentShape);
	}
}


void TetrisGame::reset(){
	score = 0;
	updateScoreDisplay();
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	updateGhostShape();
	pickNextShape();
}


void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetromino::getRandomShape());
}


bool TetrisGame::spawnNextShape() {
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	updateGhostShape();
	return isPositionLegal(currentShape);
}


bool TetrisGame::attemptRotate(GridTetromino& shape) {
	GridTetromino temp = shape;
	temp.rotateClockwise();

	if (isPositionLegal(temp)) {
		shape.rotateClockwise();
		return true;
	}

	return false;
}


bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
	GridTetromino temp = shape;
	temp.move(x, y);

	if (isPositionLegal(temp) && !(shapePlacedSinceLastGameLoop)) {
		shape.move(x, y);
		return true;
	}

	return false;
}


void TetrisGame::drop(GridTetromino& shape) {
	while (attemptMove(shape, 0, 1)) {};
}


void TetrisGame::lock(GridTetromino& shape) {
	board.setContent(shape.getBlockLocsMappedToGrid(), static_cast<int>(shape.getColor()));
	shapePlacedSinceLastGameLoop = true;
}



// GRAPHICS

void TetrisGame::drawBlock(const Point& topLeft, int xOffSet, int yOffSet, TetColor color, int alpha) {	
	int block_color = static_cast<int>(color) * 32;
	blockSprite.setTextureRect(sf::IntRect(block_color, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setColor(sf::Color(255, 255, 255, alpha));
	blockSprite.setPosition(topLeft.getX()  + (xOffSet * BLOCK_WIDTH), topLeft.getY()  + (yOffSet * BLOCK_HEIGHT));
	window.draw(blockSprite);
}

void TetrisGame::drawGameboard() {
	for (int y{ 0 }; y < board.MAX_Y; y++) {
		for (int x{ 0 }; x < board.MAX_X; x++) {
			if (board.getContent(x, y)!= Gameboard::EMPTY_BLOCK) {
				drawBlock(TetrisGame::gameboardOffset, x, y, static_cast<TetColor>(board.getContent(x, y)));
			}
		}
	}
} 

void TetrisGame::drawTetromino(GridTetromino& tetromino, const Point& topLeft, int alpha) {
	for (Point& coordinate : tetromino.getBlockLocsMappedToGrid()) {
		drawBlock(topLeft, coordinate.getX(), coordinate.getY(), tetromino.getColor(), alpha);
	}
}

void TetrisGame::updateScoreDisplay() {
	std::string score_string = "score: " + std::to_string(score);
	scoreText.setString(score_string);
}


// State & gameplay/logic methods ================================

bool TetrisGame::isPositionLegal(const GridTetromino& shape) const {
	return board.areAllLocsEmpty(shape.getBlockLocsMappedToGrid()) && isWithinBorders(shape);
}

bool TetrisGame::isWithinBorders(const GridTetromino& shape) const {
	for (Point& coord : shape.getBlockLocsMappedToGrid()) {
		if (coord.getY() >= board.MAX_Y) {
			return false;
		}
		if (coord.getX() < 0 || coord.getX() >= board.MAX_X) {
			return false;
		}

	}
	return true;
}


void TetrisGame::determineSecondsPerTick() {
	double multiplier = score * 0.001;
	double newSecond{ MAX_SECONDS_PER_TICK - multiplier };

	if (newSecond < MIN_SECONDS_PER_TICK) {
		secondsPerTick = MIN_SECONDS_PER_TICK;
		return;
	}
	secondsPerTick = newSecond;
}

void TetrisGame::updateGhostShape() {
	ghostShape = currentShape;
	drop(ghostShape);
}