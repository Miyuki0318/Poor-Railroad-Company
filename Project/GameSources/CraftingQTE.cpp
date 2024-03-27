/*!
@file CraftingQTE.cpp
@brief クラフト中のQTE
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void CraftingQTE::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Sprite::OnCreate();

		// 頂点データの座標を加算
		Vec3 diffPos = Vec3(0.5f, 0.5f, 0.0f);
		for (auto& v : m_vertex.vertices)
		{
			v.position += diffPos;
		}
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// ステージの取得
		const auto& stagePtr = GetStage();

		// フレームとQTEポイントを生成
		//m_barFlame = stagePtr->AddGameObject<Sprite>(L"C_QTE_FLAME_TX", m_barScale + Vec2(5.0f), Vec3(0.0f));
		//m_qtePoint = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", Vec2(40.0f, 10.0f), Vec3(0.0f));
	}

	// 毎フレーム更新処理
	void CraftingQTE::OnUpdate()
	{

	}

	// QTEの更新処理
	void CraftingQTE::UpdateQTE()
	{

	}

	// QTEの開始処理
	void CraftingQTE::StartQTE(const Vec3& windowPos)
	{
		m_barRatio = 0.0f;
		m_qteRatio = Utility::RangeRand(0.9f, 0.55f);
	}

	// QTEの停止処理
	bool CraftingQTE::StopQTE()
	{
		return true;
	}
}