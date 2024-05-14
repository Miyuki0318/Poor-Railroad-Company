/*!
@file TitleTrain.h
@brief 列車オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class TitleTrain : public Train
	{
		enum class State {
			None, // 待機
			GameStart, // ゲームスタート
		};

		State m_state = State::None;

	public:
		TitleTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}
		~TitleTrain() {}

		void OnUpdate() override;

		void StateProcess(State state);

		void GameStartProcess();
	};

}