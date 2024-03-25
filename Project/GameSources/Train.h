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
		/*!
		@brief 列車の状態
		*/
		enum class State {
			Onrail, // レールに乗っている
			Derail // 脱線
		};

		// ステート変数
		State m_state = State::Onrail;
		State m_beforeState = State::Onrail;

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

		void Move(State state);
	};
}
