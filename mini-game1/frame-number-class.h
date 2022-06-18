#pragma once

namespace Frame {
	class FrameNumber {
	public:
		FrameNumber();
		~FrameNumber();
		enum class FRAME_NUMBER {
			CREDIT_INIT,
			CREDIT_MAIN,
			TITLE_INIT,
			TITLE_MAIN,
			LEVEL_SELECT_INIT,
			LEVEL_SELECT_MAIN,
			PLAY_INIT,
			PLAY_MAIN,
			RESULT_INIT,
			RESULT_MAIN,
			CONFIG_INIT,
			CONFIG_MAIN,
			FINISH
		};
		void setFrameNumber(FRAME_NUMBER);
		FRAME_NUMBER getFrameNumber();
		bool compareFrameNumber(FRAME_NUMBER);

	private:
		FRAME_NUMBER frameNumber;
	};
}