/*!
@file Arrow.h
@brief ñÓàÛUIÇÃï\é¶
@author êVç»à®
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class SelectArrow : public TemplateObject
	{
		const float posX = 1920.0f / 6.0f;
		const float posY = 1080.0f / 4.0f;
		const Vec2 scale = Vec2(100.0f);

		weak_ptr<Sprite> m_rightArrow;
		weak_ptr<Sprite> m_leftArrow;

		weak_ptr<Sprite> m_selectArrow;

	public:
		SelectArrow(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void ChangeColor();
	};
}