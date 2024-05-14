/*!
@file Opening.h
@brief オープニング画面の設定
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	enum eOpeningState
	{
		move, // 移動
		fade, // フェード
		idel, // 待機
	};

	class TitleLogo : public TemplateObject
	{
	private:
		// 移動速度
		const float m_speed = 3.0f;
		
		// 最初のY座標
		const float m_startPosY = 1000.0f;

		// Y軸の移動制限
		const float m_maxPosY = 250.0f;

		// デルタタイム
		float m_deltaTime;

		eOpeningState m_openState;

		shared_ptr<Sprite> m_sprite;

		// 現在の位置
		Vec3 m_position;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
			m_openState = eOpeningState::move;
		}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief オープニングの状態を管理する関数
		*/
		void OpenStateBase(eOpeningState state);

		// 移動処理の関数
		void OpenStateMove();

		// フェード処理の関数
		void OpenStateFade();

		// 待機状態の関数
		void OpenStateIdel();
	};
}