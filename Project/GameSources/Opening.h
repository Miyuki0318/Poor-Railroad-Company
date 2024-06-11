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
		exit,
		idel
	};

	class TitleLogo : public TemplateObject
	{
	private:
		const float m_fadeTime = 3.0f;

		// �ړ����x
		const float m_speed = 5.0f;
		
		// �ŏ���Y���W
		const float m_startPosY = 900.0f;

		// Y���̈ړ�����
		const float m_maxPosY = 250.0f;

		float m_brinkTime = 1.0f;

		// �f���^�^�C��
		float m_deltaTime;

		float m_vel = 0.0f;

		eLogoState m_logoState;

		shared_ptr<Sprite> m_titleLogo;
		shared_ptr<Sprite> m_pushLogo;

		// ���݂̈ʒu
		Vec3 m_titlePos;
		Vec3 m_pushPos;

		Col4 m_color;

		bool m_brinkingFlag;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
			m_logoState = eLogoState::move;
			m_brinkingFlag = false;
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

		void SpriteBrink();
	};
}