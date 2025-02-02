#pragma once
#include "TitlePlayer.h"

namespace basecross
{
	class OnlinePlayer : public TitlePlayer
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		OnlinePlayer(const shared_ptr<Stage>& stagePtr) :
			TitlePlayer(stagePtr)
		{
		}

		void OnCreate() override;

		void OnUpdate() override;

	private:

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove() override {}
	};
}