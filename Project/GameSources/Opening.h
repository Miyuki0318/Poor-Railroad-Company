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
		shared_ptr<Sprite> m_sprite;

		Vec3 m_position;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		virtual void LogoMove();
	};
}