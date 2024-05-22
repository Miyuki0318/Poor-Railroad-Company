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

		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		m_railPos = LINE(ROW(m_DefaultPosition.z), COL(m_DefaultPosition.x));
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = m_DefaultPosition;

		// タグの設定
		AddTag(L"Train");
	}

	bool Train::SearchNextRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
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
			string line = LINE(row, col);
			if (railMap.find(line) != railMap.end())
			{
				// レールを設定
				m_movePos.first = railMap.at(m_railPos);
				m_movePos.second = railMap.at(line);
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