/*!
@file Train.h
@brief 列車オブジェクト
@author 矢吹悠葉
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
		Right,	// 右
		Left,	// 左
		Flont,	// 前
		Back,	// 奥
	};

	class Train : public TemplateObject
	{
		const Vec3 m_DefaultPosition;
		const Vec3 m_DefaultScale;
		const float m_MoveSpeed;

		/*!
		@brief 列車の状態
		*/
		enum class State {
			Onrail, // レールに乗っている
			Derail, // 脱線
			Arrival // 駅到着
		};

		pair<Vec3, Vec3> m_movePos;
		string m_railPos;
		float m_moveRatio;
		eDirection m_direction;
		map<float, eDirection> m_radMap;
		map<eDirection, vector<Vec2>> m_drMap;

		// ステート変数
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
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief オブジェクトと衝突中に呼ばれる関数
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		void StateProcess(State state);

		void OnRailState();

		bool SetNextRail();

		void SetDirection();

		Vec3 GetDefaultPosition()
		{
			return m_DefaultPosition;
		}
	};
}
