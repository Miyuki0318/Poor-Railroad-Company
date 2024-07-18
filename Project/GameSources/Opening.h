/*!
@file Opening.h
@brief �I�[�v�j���O��ʂ̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"
#include "Sprite.h"

namespace basecross {
	// �I�[�v�j���O�̏��
	enum class eOpeningState
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
		const float m_speed = 5.5f;
		
		// �ŏ���Y���W
		const float m_startPosY = 900.0f;

		// Y���̈ړ�����
		const float m_maxPosY = 150.0f;

		float m_brinkTime = 1.0f;

		// �f���^�^�C��
		float m_deltaTime;

		float m_vel = 0.0f;

		eOpeningState m_logoState;

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
			m_logoState = eOpeningState::move;
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

		// �Q�[���J�n���̏���
		void MoveTitleLogo();

		// �t�F�[�h����
		void FadeTitleLogo();

		// �{�^�������������̏���
		void PushButton();

		// �{�^���̓_�ŏ���
		void SpriteBrink();

		/*!
		@brief �^�C�g�����S�X�e�[�g�擾�֐�
		@return m_logoState
		*/
		eOpeningState GetLogoState() const
		{
			return m_logoState;
		}
	};
}