#include "render-class.h"
#include "global.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	Render::Render() {

	}

	/**
	*  デストラクタ
	*/
	Render::~Render() {
		if (cr != NULL) {
			delete cr;
			cr = NULL;
		}
		if (lsr != NULL) {
			delete lsr;
			lsr = NULL;
		}
		if (pr != NULL) {
			delete pr;
			pr = NULL;
		}
	}

	/**
	*  描画する関数.
	*/
	void Render::Rend(void) {
		switch (frameNumber.getFrameNumber()) {
		case FrameNumber::FRAME_NUMBER::CREDIT_INIT:
			cr = new CreditRender();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT_MAIN);
		case FrameNumber::FRAME_NUMBER::CREDIT_MAIN:
			cr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT_MAIN)) {
				delete cr;
				cr = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::LEVEL_SELECT_INIT:
			lsr = new LevelSelectRender();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_MAIN);
		case FrameNumber::FRAME_NUMBER::LEVEL_SELECT_MAIN:
			lsr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::LEVEL_SELECT_MAIN)) {
				level = lsr->getLevel();
				delete lsr;
				lsr = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::PLAY_INIT:
			pr = new PlayRender(level);
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_MAIN);
		case FrameNumber::FRAME_NUMBER::PLAY_MAIN:
			pr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_MAIN)) {
				delete pr;
				pr = NULL;
			}
			break;
		default:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
	}
}