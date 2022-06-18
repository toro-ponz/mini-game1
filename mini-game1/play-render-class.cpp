#include "play-render-class.h"
#include "input-device.h"
#include "global.h"
#include <time.h>

namespace Frame {
	/**
	*  コンストラクタ
	*/
	PlayRender::PlayRender(int l) :
			back(playImageDirectoryPath + "back.png", 100, 50.0f),
			figures(playImageDirectoryPath + "figures.png", 100, 50.0f),
			my(playImageDirectoryPath + "back.png", 1, 50.0f),
			clear(playImageDirectoryPath + "clear.png", 1, 500.0f, 100.0f),
			descriptionImage(playImageDirectoryPath + "description.png", 1, 150.0f) {
		level = l;
		const int fontSize = 25;
		const int fontWeight = 1;
		const string fontName = "メイリオ";
		if (level == 1) {
			const string descString[10] = {
				"このゲームはn×nマスに敷き詰められた数字を全て使用して、最終的に結果が2になればクリアのゲームです。",
				"あらかじめ決められたコマ(初期値1)を使用して、上下左右に移動させることができます。",
				"動かしたコマは、動かされたコマの数字と計算が行われ、動かすコマの数字がその結果になります。",
				"赤は加算(+)、青は減算(-)、\n緑は乗算(×)、黄色は除算(÷)です。",
				"加算された結果が2桁の場合、10の位は破棄されます。\n(例：8+3=11→1)",
				"減算された結果が負の数の場合、結果は0になります。\n(例：2-5=-3→0)",
				"乗算された結果が2桁の場合、10の位は破棄されます。\n(例：4×7=28→8)",
				"除算された結果が少数の場合、四捨五入されます。\n(例：7÷4=1.75→2)\n動かすコマの数を移動先のコマの数で割ります。",
				"既に通ったマスは通ることができません。",
				"では、Lv2からプレイしてみてください。"
			};
			description.reserve(10);
			for (int i = 0; i < 10; i++) {
				FontTextureCreate buf = FontTextureCreate();
				description.push_back(buf);
				description[i].fontCreate(descString[i], 50.0f, 50.0f, fontSize, fontWeight, fontName);
				description[i].setWidthOrHeight(400.0f);
				description[i].setColor(D3DCOLOR_ARGB(255, 0, 0, 0));
			}
			descriptionImage.setRectFromChip(0, 1);
			descriptionImage.setCenter(0, 75.0f, 75.0f);
			descriptionImage.setPosition(0, 250.0f, 400.0f);
			DWORD interval = 1000;
			descriptionImage.setAnimationInterval(0, 1, 9, &interval, 1, 9);
		}
		else {
			const float pos[] = { 200.0f, 175.0f, 150.0f, 125.0f, 100.0f, 75.0f, 50.0f, 25.0f, 0.0f };
			createGame(level);
			for (int i = 0; i < level; i++) {
				for (int j = 0; j < level; j++) {
					back.setRectFromChip(level * i + j, fourArith[i][j] + 1);
					figures.setRectFromChip(level * i + j, data[i][j] + 1);
					back.setPosition(level* i + j, pos[level - 2] + (50.0f * i), pos[level - 2] + (50.0f * j));
					figures.setPosition(level * i + j, pos[level - 2] + (50.0f * i), pos[level - 2] + (50.0f * j));
					passed[i][j] = false;
				}
			}
			for (int i = level * level; i < 100; i++) {
				back.disableDraw(i);
				figures.disableDraw(i);
			}
			posX = startX;
			posY = startY;
			passed[startY][startX] = true;
			back.setRectFromChip(startY * level + startX, 1);
			my.setRectFromChip(0, 6);
			my.setPosition(0, pos[level - 2] + (50.0f * startY), pos[level - 2] + (50.0f * startX));
			currentFigure = 1;
			state = STATE::PLAYING;
		}
	}

	/**
	*  デストラクタ
	*/
	PlayRender::~PlayRender() {

	}

	/**
	*  描画する関数.
	*/
	void PlayRender::Rend(void) {
		Sprite->Begin(NULL);

		inputDevice->getPushState();
		if (level == 1) {
			for (int i = 0; i < 10; i++) {
				description[i].disableDraw();
			}

			if (descNum >= 10)
				frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_INIT);
			else
				description[descNum].enableDraw();

			for (int i = 0; i < 10; i++) {
				description[i].Rend();
			}
			descriptionImage.Draw();

			if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH))
				descNum++;
		}
		else {
			if (state == STATE::PLAYING)
				game();
			back.Draw();
			my.Draw();
			figures.Draw();
			if (state == STATE::CLEARED) {
				clear.setRectFromChip(0, 1);
				clear.Draw();
				if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH))
					frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_INIT);
			}
			else if (state == STATE::NOT_CLEARED) {
				clear.setRectFromChip(0, 2);
				clear.Draw();
				if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH))
					frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_INIT);
			}
		}

		Sprite->End();
	}

	void PlayRender::game() {
		int result = 100;
		//手詰まり
		if ((posX == 0 || passed[posY][posX - 1]) &&
			(posX == piece - 1 || passed[posY][posX + 1]) &&
			(posY == 0 || passed[posY - 1][posX]) &&
			(posY == piece - 1 || passed[posY + 1][posX])) {
			state = STATE::NOT_CLEARED;
		}
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH)) {
			if (posX != 0 && !passed[posY][posX - 1]) {
				nextX = posX - 1;
				nextY = posY;
				result = fourArithmeticOperationPlay(fourArith[nextY][nextX]);
				my.addPositionY(0, -50.0f);
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH)) {
			if (posX != piece - 1 && !passed[posY][posX + 1]) {
				nextX = posX + 1;
				nextY = posY;
				result = fourArithmeticOperationPlay(fourArith[nextY][nextX]);
				my.addPositionY(0, 50.0f);
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_LEFT, InputDevice::KEY_STATE::STATE_PUSH)) {
			if (posY != 0 && !passed[posY - 1][posX]) {
				nextX = posX;
				nextY = posY - 1;
				result = fourArithmeticOperationPlay(fourArith[nextY][nextX]);
				my.addPositionX(0, -50.0f);
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_RIGHT, InputDevice::KEY_STATE::STATE_PUSH)) {
			if (posY != piece - 1 && !passed[posY + 1][posX]) {
				nextX = posX;
				nextY = posY + 1;
				result = fourArithmeticOperationPlay(fourArith[nextY][nextX]);
				my.addPositionX(0, 50.0f);
			}
		}

		if (result != 100) {
			currentFigure = result;
			back.setRectFromChip(nextY * level + nextX, 1);
			figures.setRectFromChip(nextY * level + nextX, currentFigure + 1);
			passed[nextY][nextX] = true;
			posX = nextX;
			posY = nextY;
			moveCount++;
			if (moveCount == level * level - 1) {
				if (currentFigure == 2) {
					state = STATE::CLEARED;
				}
				else {
					state = STATE::NOT_CLEARED;
				}
			}
		}
	}

	void PlayRender::createGame(int p) {
		piece = p;
		srand((unsigned int)timeGetTime());
		data = vector<vector<int>>(piece);
		fourArith = vector<vector<int>>(piece);
		passed = vector<vector<bool>>(piece);
		for (int i = 0; i < piece; i++) {
			data[i] = vector<int>(piece);
			fourArith[i] = vector<int>(piece);
			passed[i] = vector<bool>(piece);
		}
		while (true) {
			for (int i = 0; i < piece; i++) {
				for (int j = 0; j < piece; j++) {
					passed[i][j] = false;
				}
			}
			startY = rand() % piece;
			startX = rand() % piece;
			posY = startY;
			posX = startX;
			data[startY][startX] = 1;
			currentFigure = 1;
			fourArith[startY][startX] = 0;
			passed[startY][startX] = true;
			bool collision = false;
			for (int i = 0; i < piece * piece - 1; i++) {
				//手詰まり
				if ((posX == 0 || passed[posY][posX - 1]) &&
					(posX == piece - 1 || passed[posY][posX + 1]) &&
					(posY == 0 || passed[posY - 1][posX]) &&
					(posY == piece - 1 || passed[posY + 1][posX])) {
					collision = true;
					break;
				}
				currentFigure = decideDirection(rand() % 4 + 1);
				passed[posY][posX] = true;
				posY = nextY;
				posX = nextX;
			}
			endY = posY;
			endX = posX;
			if (!collision && currentFigure == 2)
				break;
		}
	}

	int PlayRender::add(int x, int y) {
		return (x + y) % 10;
	}

	int PlayRender::sub(int x, int y) {
		if (x < y)
			return 0;
		else
			return x - y ;
	}

	int PlayRender::mult(int x, int y) {
		return (x * y) % 10;
	}

	int PlayRender::div(int x, int y) {
		return (int)round((float)x / y);
	}
	
	int PlayRender::decideDirection(int direction) {
		srand((unsigned int)timeGetTime() * rand());
		switch (direction) {
		case 1:
			if (posX != 0 && !passed[posY][posX - 1]) {
				nextY = posY;
				nextX = posX - 1;
				return fourArithmeticOperation(rand() % 4 + 1);
			}
			else {
				return decideDirection(rand() % 4 + 1);
			}
		case 2:
			if (posX != piece - 1 && !passed[posY][posX + 1]) {
				nextY = posY;
				nextX = posX + 1;
				return fourArithmeticOperation(rand() % 4 + 1);
			}
			else {
				return decideDirection(rand() % 4 + 1);
			}
		case 3:
			if (posY != 0 && !passed[posY - 1][posX]) {
				nextY = posY - 1;
				nextX = posX;
				return fourArithmeticOperation(rand() % 4 + 1);
			}
			else {
				return decideDirection(rand() % 4 + 1);
			}
		case 4:
			if (posY != piece - 1 && !passed[posY + 1][posX]) {
				nextY = posY + 1;
				nextX = posX;
				return fourArithmeticOperation(rand() % 4 + 1);
			}
			else {
				return decideDirection(rand() % 4 + 1);
			}
		default:
			return 0;
		}
	}
	
	int PlayRender::fourArithmeticOperation(int number) {
		data[nextY][nextX] = rand() % 9 + 1;
		switch (number) {
		case 1:
			fourArith[nextY][nextX] = number;
			return add(currentFigure, data[nextY][nextX]);
		case 2:
			fourArith[nextY][nextX] = number;
			return sub(currentFigure, data[nextY][nextX]);
		case 3:
			fourArith[nextY][nextX] = number;
			return mult(currentFigure, data[nextY][nextX]);
		case 4:
			fourArith[nextY][nextX] = number;
			return div(currentFigure, data[nextY][nextX]);
		default:
			return 0;
		}
	}
	
	int PlayRender::fourArithmeticOperationPlay(int number) {
		switch (number) {
		case 1:
			return add(currentFigure, data[nextY][nextX]);
		case 2:
			return sub(currentFigure, data[nextY][nextX]);
		case 3:
			return mult(currentFigure, data[nextY][nextX]);
		case 4:
			return div(currentFigure, data[nextY][nextX]);
		default:
			return 0;
		}
	}
}