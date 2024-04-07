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
		const Vec3 m_DefaultPosition;
		const Vec3 m_DefaultScale;

		/*!
		@brief ��Ԃ̏��
		*/
		enum class State {
			Onrail, // ���[���ɏ���Ă���
			Derail, // �E��
			Arrival // �w����
		};

		// �X�e�[�g�ϐ�
		State m_state = State::Onrail;
		State m_beforeState = State::Onrail;

		const float m_MoveSpeed;

		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Train(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_DefaultPosition(Vec3(3.0f, 1.5f, -7.0f)),
			m_DefaultScale(Vec3(2.0f, 1.0f, 1.0f)),
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

		void StateProcess(State state);

		Vec3 GetDefaultPosition()
		{
			return m_DefaultPosition;
		}
	};
}
