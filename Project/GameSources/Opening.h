/*!
@file Opening.h
@brief �I�[�v�j���O��ʂ̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	enum class eLogoState
	{
		move,
		push,
		fade,
		idel
	};

	class TitleLogo : public TemplateObject
	{
	private:
		const float m_fadeTime = 3.0f;

		// �ړ����x
		const float m_speed = 10.0f;
		
		// �ŏ���Y���W
		const float m_startPosY = 900.0f;

		// Y���̈ړ�����
		const float m_maxPosY = 250.0f;

		// �f���^�^�C��
		float m_deltaTime;

		eLogoState m_logoState;

		shared_ptr<Sprite> m_sprite;

		// ���݂̈ʒu
		Vec3 m_position;

		bool m_fade;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
			m_logoState = eLogoState::move;
			m_fade = false;
		}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		// �ړ������̊֐�
		void MoveTitleLogo();

		// �t�F�[�h�����̊֐�
		void FadeTitleLogo();

		// �{�^�������������̏���
		void PushButton();

	};
}