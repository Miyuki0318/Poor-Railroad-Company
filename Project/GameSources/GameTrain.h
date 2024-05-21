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
		enum class State {
			Onrail, // レールに乗っている
			Derail, // 脱線
			Arrival // 駅到着
		};
		State m_state = State::Onrail;

	public:
		GameTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}

		GameTrain(const shared_ptr<Stage>& stagePtr,
			const Vec3& startPosition
		) :
			Train(stagePtr, startPosition)
		{
		}

		~GameTrain() {}

		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		/// <summary>
		/// 状態ごとの処理
		/// </summary>
		/// <param name="state">状態</param>
		void StateProcess(State state);

		/// <summary>
		/// レール上に居るときの処理
		/// </summary>
		void OnRailProcess();

		/// <summary>
		/// 次のレールを検索する処理
		/// </summary>
		/// <returns>見つかったかどうか</returns>
		bool SearchNextRail() override;

		/// <summary>
		/// ゴールに着いたか確認する処理
		/// </summary>
		/// <returns>着いたかどうか</returns>
		bool CheckGoalRail();

	};
}