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

		// 現在の状態
		State m_state = State::None;

	public:
		TitleTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}
		~TitleTrain() {}

		void OnUpdate() override;

		/// <summary>
		/// 状態ごとの処理
		/// </summary>
		/// <param name="state">状態</param>
		void StateProcess(State state);

		/// <summary>
		/// ゲームスタート時の処理
		/// </summary>
		void GameStartProcess();
	};

}