/*!
@file GoalGuide.h
@brief ゴール駅方向を表示するガイドUI
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief ゴール方向ガイド
	*/
	class GoalGuide : public Sprite
	{
		weak_ptr<TemplateObject> m_goalStation; // ゴール駅
		weak_ptr<Sprite> m_arrowSprite; // 矢印のスプライト

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GoalGuide(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"GOAL_GUIDE_TX", Vec2(150.0f))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GoalGuide() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;
	};
}