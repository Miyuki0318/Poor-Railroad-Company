/*!
@file Train.h
@brief 列車オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
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
		size_t m_isRailNum;
		float m_moveRatio;

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
			m_isRailNum = 0;
			m_movePos.first.zero();
			m_movePos.second.zero();
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

		Vec3 GetDefaultPosition()
		{
			return m_DefaultPosition;
		}
	};
}
