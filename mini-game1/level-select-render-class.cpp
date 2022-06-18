#include "level-select-render-class.h"
#include "input-device.h"
#include "global.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	LevelSelectRender::LevelSelectRender() :
			background(levelSelectImageDirectoryPath + "background.png"),
			selectedLevel(levelSelectImageDirectoryPath + "lv.png", 1, 80.0f){
		selectedLevel.setCenter(0, 40.0f, 40.0f);
		selectedLevel.setPosition(0, 250.0f, 250.0f);

		loading.fontCreate("マップ生成中……", 100.0f, 400.0f, 30, 1, "メイリオ");
	}

	/**
	*  デストラクタ
	*/
	LevelSelectRender::~LevelSelectRender() {

	}

	/**
	*  描画する関数.
	*/
	void LevelSelectRender::Rend(void) {
		Sprite->Begin(NULL);

		inputDevice->getPushState();

		background.Draw();
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH)) {
			if (level != 8)
				level++;
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH)) {
			if (level != 1)
				level--;
		}
		else if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH)) {
			loading.Rend();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_INIT);
		}
		selectedLevel.setRectFromChip(0, level);
		selectedLevel.Draw();

		Sprite->End();
	}

	int LevelSelectRender::getLevel() {
		return level;
	}
}