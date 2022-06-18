#pragma once

#include "sprite-drawing-class.h"
#include "font-texture-create-class.h"

using namespace DxSprite;
using namespace Font;

namespace Frame {
	class PlayRender {
	public:
		PlayRender(int);
		~PlayRender();
		void Rend();

	private:
		void game();
		void createGame(int);
		int add(int, int);
		int sub(int, int);
		int mult(int, int);
		int div(int, int);
		int decideDirection(int);
		int fourArithmeticOperation(int);
		int fourArithmeticOperationPlay(int);

		SpriteDrawing back;
		SpriteDrawing figures;
		SpriteDrawing my;
		SpriteDrawing clear;
		SpriteDrawing descriptionImage;
		vector<FontTextureCreate> description;
		vector<vector<int>> data;
		vector<vector<int>> fourArith;
		vector<vector<bool>> passed;
		const int goal = 2;
		int level = 0;
		int descNum = 0;

		int startX, startY;
		int endX, endY;
		int posX, posY;
		int prevX, prevY;
		int nextX, nextY;
		int currentFigure;
		int moveCount = 0;
		int piece;

		enum class STATE {
			PLAYING,
			CLEARED,
			NOT_CLEARED
		} state;
	};
}