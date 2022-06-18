#include "credit-render-class.h"
#include "input-device.h"
#include "global.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	CreditRender::CreditRender() :
		Logo(creditImageDirectoryPath + "logo.png"),
		Black(colorDirectoryPath + "black.png") {
		Logo.setPosition(0, 250.0f, 250.0f);
		Logo.setCenter(0, 250.0f, 62.5f);

		Black.setRectFromPixel(0, 0, 500, 0, 500);
		Black.setPosition(0, 0.0f, 0.0f);
		state = 0;
		oldTime = timeGetTime();
	}

	/**
	*  デストラクタ
	*/
	CreditRender::~CreditRender() {

	}

	/**
	*  描画する関数.
	*/
	void CreditRender::Rend(void) {
		Sprite->Begin(NULL);

		DWORD time = timeGetTime();
		inputDevice->getPushState();

		Logo.Draw();
		Black.Draw();

		switch (state) {
		case 0:
			if (oldTime + 4 < time) {
				for (int i = 0; i < (int)((time - oldTime) / 4); i++) {
					if (!Black.addAlpha(0, -1)) {
						state++;
						break;
					}
				}
				oldTime = time;
			}
			break;
		case 1:
			if (oldTime + 2000 < time) {
				oldTime = time;
				state++;
			}
			break;
		case 2:
			if (oldTime + 4 < time) {
				for (int i = 0; i < (int)((time - oldTime) / 4); i++) {
					if (!Black.addAlpha(0, 1)) {
						state++;
						break;
					}
				}
				oldTime = time;
			}
			break;
		case 3:
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_INIT);
			break;
		}

		if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH))
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_INIT);

		Sprite->End();
	}
}