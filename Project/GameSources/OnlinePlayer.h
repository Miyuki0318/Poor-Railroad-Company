#pragma once
#include "Player.h"

namespace basecross
{
	class OnlinePlayer : public Player
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		OnlinePlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{
		}

		void OnCreate() override;

		void OnUpdate() override;

	private:

		/*!
		@brief プレイヤーに付加する機能生成関数
		*/
		void CreatePlayerFeatures() override {}

		/*!
		@brief インディケーターへの取得と呼び出し関数
		*/
		void IndicatorOrder() override {}

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove() override;

		void UpdateRotation() override;

		void SetAnimation();
	};
}