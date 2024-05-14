/*!
@file Opening.h
@brief �I�[�v�j���O��ʂ̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	enum eOpeningState
	{
		move, // �ړ�
		fade, // �t�F�[�h
		idel, // �ҋ@
	};

	class TitleLogo : public TemplateObject
	{
	private:
		const float m_speed = 3.0f;
		
		const float m_height = 1000.0f;

		const float m_maxPosY = 250.0f;

		float m_deltaTime;

		eOpeningState m_openState;

		shared_ptr<Sprite> m_sprite;

		Vec3 m_position;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
			m_openState = eOpeningState::move;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void OpenStateBase(eOpeningState state);

		void OpenStateMove();
		void OpenStateFade();
		void OpenStateIdel();
	};
}