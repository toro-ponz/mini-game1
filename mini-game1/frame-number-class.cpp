#include "frame-number-class.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	FrameNumber::FrameNumber() {
		frameNumber = FRAME_NUMBER::CREDIT_INIT;
	}

	/**
	*  デストラクタ
	*/
	FrameNumber::~FrameNumber() {

	}

	/**
	*  フレームナンバーを指定する関数.
	*  @param fNum<FRAME_NUMBER> 登録するフレームナンバー
	*/
	void FrameNumber::setFrameNumber(FRAME_NUMBER fNum) {
		frameNumber = fNum;
	}

	/**
	*  現在のフレームナンバーを返す関数.
	*  @return <FRAME_NUMBER> 現在のフレームナンバー
	*/
	FrameNumber::FRAME_NUMBER FrameNumber::getFrameNumber() {
		return frameNumber;
	}

	/**
	*  現在のフレームナンバーと指定されたものが等しいか判定する関数.
	*  @param fNum<FRAME_NUMBER> 判定するフレームナンバー
	*  @return <bool> fNumが現在のフレームナンバーと等しければtrue
	*/
	bool FrameNumber::compareFrameNumber(FRAME_NUMBER fNum) {
		return frameNumber == fNum;
	}
}