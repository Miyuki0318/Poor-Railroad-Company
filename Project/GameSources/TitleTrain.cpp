/*!
@file TitleTrain.cpp
@brief 列車の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleTrain::OnUpdate()
	{
		auto construction = GetStage()->GetSharedGameObject<Construction>(L"Construction");
		if (FindTag(tagName))
		{
			m_state = State::GameStart;
		}
		StateProcess(m_state);
	}

	void TitleTrain::StateProcess(State state)
	{
		if (state == State::None) return;

		if (state == State::GameStart)
		{
			GameStartProcess();
		}
	}

	void TitleTrain::GameStartProcess()
	{
		// 線形補間で移動
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		//m_moveRatio += DELTA_TIME * m_MoveSpeed;
		m_moveRatio = MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).value;

		// 範囲外になったら
		if (MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).outRange)
		{
			// 次のレールを見つける、設定不可なら待機
			if (!SearchNextRail()) m_state = State::None;

			SetNextRailDirection();
		}

		// 座標の更新
		SetPosition(pos);

		PostEvent(5.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"GameStage");
	}
}