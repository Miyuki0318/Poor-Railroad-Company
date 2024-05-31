/*!
@file Train.h
@brief ��Ԃ̐e
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
	protected:
		enum class State {
			None,		// �ҋ@
			GameStart,	// �Q�[���X�^�[�g
			OnRail,		// ���H��
			Derail,		// �E��
			Arrival,	// ����
		};
		State m_state;

		const Vec3 m_DefaultPosition;
		const Vec3 m_DefaultScale;
		const float m_MoveSpeed;

		pair<Vec3, Vec3> m_movePos;
		string m_railPos;
		float m_moveRatio;
		eDirection m_direction;
		map<float, eDirection> m_radMap;
		map<eDirection, vector<Vec2>> m_drMap;
		Mat4x4 m_modelMat;

		Vec3 m_moveDirection;

		shared_ptr<PNTStaticModelDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Train(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_DefaultPosition(Vec3(3.0f, 1.0f, -7.0f)),
			m_DefaultScale(Vec3(1.0f, 1.0f, 1.0f)),
			m_MoveSpeed(0.2f),
			m_moveDirection(Vec3(0.0f))
		{
			m_state = State::None;

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

			m_modelMat.affineTransformation(
				Vec3(0.04f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-0.25f, 0.0f, -0.075f)
			);
		}
		Train(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr),
			m_DefaultPosition(position),
			m_DefaultScale(Vec3(1.0f, 1.0f, 1.0f)),
			m_MoveSpeed(0.2f),
			m_moveDirection(Vec3(0.0f))
		{
			m_state = State::None;

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

			m_modelMat.affineTransformation(
				Vec3(0.04f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-0.25f, 0.0f, -0.075f)
			);
		}

		~Train() {}

		virtual void OnCreate() override;

		/// <summary>
		/// �ړ�����
		/// </summary>
		/// <param name="nextState">�ړ��悪������Ȃ��ꍇ�Ɉڍs����X�e�[�g</param>
		void MoveProcess(State nextState);

		/// <summary>
		/// ���̃��[�����������鏈��
		/// </summary>
		/// <returns>�����������ǂ���</returns>
		virtual bool SearchNextRail();

		/// <summary>
		/// ���̃��[���ւ̕�����ݒ肷�鏈��
		/// </summary>
		virtual void SetNextRailDirection();

		/// <summary>
		/// �����ʒu�̎擾
		/// </summary>
		/// <returns>�����ʒu</returns>
		virtual Vec3 GetDefaultPosition()
		{
			return m_DefaultPosition;
		}
	};
}
