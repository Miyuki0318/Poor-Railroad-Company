/*!
@file Arrow.h
@brief îóUIÌ\¦
@author VÈ¨
*/

#pragma once
#include "TemplateObject.h"
#include "Sprite.h"

namespace basecross {
	class SelectArrow : public TemplateObject
	{
		const float posX = 750.0f;
		const float posY = 0.0f;
		const Vec2 scale = Vec2(200.0f);

		weak_ptr<Sprite> m_rightArrow;
		weak_ptr<Sprite> m_leftArrow;

		weak_ptr<Sprite> m_selectArrow;

		float oldStick;
		bool m_currentStick;
		bool m_oldStick;

	public:
		SelectArrow(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			oldStick(0.0f),
			m_currentStick(false),
			m_oldStick(false)
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void ChangeColor();

		void PushSE();
	};
}