/*!
@file Train.h
@brief ��ԃI�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class Train : public TemplateObject
	{
		/*!
		@brief ��Ԃ̏��
		*/
		enum class State {
			Onrail, // ���[���ɏ���Ă���
			Derail // �E��
		};

		// �X�e�[�g�ϐ�
		State m_state = State::Derail;
		State m_beforeState = State::Derail;

		const float m_MoveSpeed;

		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Train(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(-4.0f, 1.0f, 0.0f), Vec3(0.0f), Vec3(2.0f, 2.0f, 1.0f)),
			m_MoveSpeed(3.0f)
		{
		}

		~Train() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �I�u�W�F�N�g�ƏՓ˒��ɌĂ΂��֐�
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		void Move(State state);
	};
}
