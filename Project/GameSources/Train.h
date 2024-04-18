/*!
@file Train.h
@brief ��ԃI�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

#define FLONT_CSV Vec2(0.0f, 1.0f)
#define BACK_CSV Vec2(0.0f, -1.0f)
#define RIGHT_CSV Vec2(1.0f, 0.0f)
#define LEFT_CSV Vec2(-1.0f, 0.0f)

namespace basecross {

	enum class eDirection
	{
		Right,	// �E
		Left,	// ��
		Flont,	// �O
		Back,	// ��
	};

	class Train : public TemplateObject
	{
		const Vec3 m_DefaultPosition;
		const Vec3 m_DefaultScale;
		const float m_MoveSpeed;

		/*!
		@brief ��Ԃ̏��
		*/
		enum class State {
			Onrail, // ���[���ɏ���Ă���
			Derail, // �E��
			Arrival // �w����
		};

		pair<Vec3, Vec3> m_movePos;
		string m_railPos;
		float m_moveRatio;
		eDirection m_direction;
		map<float, eDirection> m_radMap;
		map<eDirection, vector<Vec2>> m_drMap;

		// �X�e�[�g�ϐ�
		State m_state = State::Onrail;
		State m_beforeState = State::Onrail;

		Vec3 m_moveDirection;

		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Train(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_DefaultPosition(Vec3(3.0f, 1.5f, -7.0f)),
			m_DefaultScale(Vec3(2.0f, 1.0f, 1.0f)),
			m_MoveSpeed(2.0f),
			m_moveDirection(Vec3(0.0f))
		{
			m_moveRatio = 0.0f;
			m_railPos = { 0, 0 };
			m_direction = eDirection::Right;
			m_movePos.first.zero();
			m_movePos.second.zero();

			m_radMap.insert(make_pair(XM_PI, eDirection::Right));
			m_radMap.insert(make_pair(0.0f, eDirection::Left));
			m_radMap.insert(make_pair(XM_PIDIV2, eDirection::Flont));
			m_radMap.insert(make_pair(-XM_PIDIV2, eDirection::Back));

			m_drMap.insert(make_pair(eDirection::Right, vector<Vec2>{RIGHT_CSV, FLONT_CSV, BACK_CSV}));
			m_drMap.insert(make_pair(eDirection::Left, vector<Vec2>{LEFT_CSV, FLONT_CSV, BACK_CSV}));
			m_drMap.insert(make_pair(eDirection::Flont, vector<Vec2>{RIGHT_CSV, LEFT_CSV, FLONT_CSV}));
			m_drMap.insert(make_pair(eDirection::Back, vector<Vec2>{RIGHT_CSV, LEFT_CSV, BACK_CSV}));
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

		void OnRailState();

		bool SetNextRail();

		bool CheckGoalRail();

		void SetDirection();

		Vec3 GetDefaultPosition()
		{
			return m_DefaultPosition;
		}
	};
}
