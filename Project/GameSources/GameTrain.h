/*!
@file GameTrain.h
@brief 列車オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class GameTrain : public Train
	{
		State m_state = State::Onrail;

	public:
		GameTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}
		~GameTrain() {}

		void OnUpdate() override;

		void StateProcess(State state) override;

		void OnRailProcess() override;

		bool CheckGoalRail() override;
	};
}