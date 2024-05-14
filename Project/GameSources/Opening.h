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
		// �ړ����x
		const float m_speed = 3.0f;
		
		// �ŏ���Y���W
		const float m_startPosY = 1000.0f;

		// Y���̈ړ�����
		const float m_maxPosY = 250.0f;

		// �f���^�^�C��
		float m_deltaTime;

		eOpeningState m_openState;

		shared_ptr<Sprite> m_sprite;

		// ���݂̈ʒu
		Vec3 m_position;

	public:
		TitleLogo(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_deltaTime = 0.0f;
			m_openState = eOpeningState::move;
		}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �I�[�v�j���O�̏�Ԃ��Ǘ�����֐�
		*/
		void OpenStateBase(eOpeningState state);

		// �ړ������̊֐�
		void OpenStateMove();

		// �t�F�[�h�����̊֐�
		void OpenStateFade();

		// �ҋ@��Ԃ̊֐�
		void OpenStateIdel();
	};
}