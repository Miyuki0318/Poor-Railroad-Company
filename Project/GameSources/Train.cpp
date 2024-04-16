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

		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		m_isRailNum = railMap.begin()->first;
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = railMap.begin()->second;

		// タグの設定
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{
		Debug::Log(L"Start", m_movePos.first);
		Debug::Log(L"End", m_movePos.second);

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

		if (state == State::Derail)
		{
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
			return;
		}

		if (state == State::Onrail)
		{
			OnRailState();
		}
	}

	void Train::OnRailState()
	{
		// 線形補間で移動
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		m_moveRatio += DELTA_TIME / m_MoveInSeconds;

		// 割合が1以上になったら0で初期化
		if (m_moveRatio >= 1.0f)
		{
			m_moveRatio = 0.0f;

			// 次のレールを設定する、設定不可なら脱線ステート
			if (!SetNextRail()) m_state = State::Derail;
		}

		// 座標の更新
		SetPosition(pos);
	}

	bool Train::SetNextRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		if (railMap.empty()) return false;

		// 始点と終点の設定、終点が無い場合はfalseを返す
		m_movePos.first = railMap.at(m_isRailNum++);
		if (railMap.find(m_isRailNum) != railMap.end())
		{
			m_movePos.second = railMap.at(m_isRailNum);
			return true;
		}

		return false;
	}
}