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
		const float m_speed = 3.0f;
		
		const float m_height = 1000.0f;

		const float m_maxPosY = 250.0f;

		float m_deltaTime;

		shared_ptr<Sprite> m_sprite;

		Vec3 m_position;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		virtual void LogoMove();
	};
}