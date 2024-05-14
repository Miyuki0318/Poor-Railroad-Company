/*!
@file Opening.h
@brief �I�[�v�j���O��ʂ̐ݒ�
@author �V�Ȉ�
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