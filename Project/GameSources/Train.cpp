/*!
@file Train.cpp
@brief 列車の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Train::OnCreate()
	{
		TemplateObject::OnCreate();
		SetPosition(m_DefaultPosition);
		SetScale(m_DefaultScale);

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_BLUE);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();

		// タグの設定
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{
		StateProcess(m_state);
		m_beforeState = m_state;
	}

	void Train::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"Tarminal"))
		{
			m_state = State::Derail;
		}
		else if (gameObject->FindTag(L"GoalRail"))
		{
			m_state = State::Arrival;
		}
	}

	void Train::StateProcess(State state)
	{
		if (state == State::Arrival) return;

		if (state == State::Derail && m_state != m_beforeState)
		{
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
			return;
		}

		if (state == State::Onrail)
		{
			m_position.x += DELTA_TIME * m_MoveSpeed;
			SetPosition(m_position);
		}
	}
}