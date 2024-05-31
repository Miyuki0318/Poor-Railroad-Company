/*!
@file Train.cpp
@brief 列車の親実装
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
		m_ptrDraw = AddComponent<PNTStaticModelDraw>();
		m_ptrDraw->SetMeshResource(L"TRAIN");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		//m_ptrDraw->SetDiffuse(COL_BLUE);

		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		m_railPos = POS2LINE(m_DefaultPosition);
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = m_DefaultPosition;

		// タグの設定
		AddTag(L"Train");
	}

	void Train::MoveProcess(State nextState)
	{
		// 線形補間で移動
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		m_moveRatio = MathF::Repeat01(m_moveRatio, m_moveSpeed);

		// 範囲外になったら
		if (MathF::GetOutRange())
		{
			//m_moveRatio = 0.0f;
			// 次のレールを見つけられなかったら次のステートに
			if (!SearchNextRail()) m_state = nextState;

			SetNextRailDirection(); // 次のレールの方向を設定
		}

		// 座標の更新
		SetPosition(pos);
	}

	bool Train::SearchNextRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return false;
		
		// 始点と終点の設定、終点が無い場合はfalseを返す
		const auto& drs = m_drMap.at(m_direction);
		for (const auto& dr : drs)
		{
			// 行と列
			size_t row, col;
			row = ROW(m_movePos.second.z + dr.y);
			col = COL(m_movePos.second.x + dr.x);

			// 行列でキーを設定
			string line = ROWCOL2LINE(row, col);
			if (railMap.find(line) != railMap.end())
			{
				// レールを設定
				m_movePos.first = railMap.at(m_railPos).thisPos;
				m_movePos.second = railMap.at(line).thisPos;
				m_railPos = line;

				return true;
			}
		}

		return false;
	}

	void Train::SetNextRailDirection()
	{
		// 始点と終点のレールからラジアン角を求める
		float rad = atan2f(m_movePos.first.z - m_movePos.second.z, m_movePos.first.x - m_movePos.second.x);
		if (m_radMap.find(rad) != m_radMap.end())
		{
			// ラジアン角から方向enumを設定
			m_direction = m_radMap.at(rad);
		}
	}
}