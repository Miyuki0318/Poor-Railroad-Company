/*!
@file Opening.h
@brief オープニング画面の設定
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class TitleLogo : public TemplateObject
	{
	private:
		const float m_fadeTime = 3.0f;

		// 移動速度
		const float m_speed = 10.0f;
		
		// 最初のY座標
		const float m_startPosY = 900.0f;

		// Y軸の移動制限
		const float m_maxPosY = 250.0f;

		// デルタタイム
		float m_deltaTime;

		shared_ptr<Sprite> m_sprite;

		// 現在の位置
		Vec3 m_position;

		bool m_fade;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
			m_fade = false;
		}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		// 移動処理の関数
		void MoveTitleLogo();

		// フェード処理の関数
		void FadeTitleLogo();

		void PushButton();
	};
}