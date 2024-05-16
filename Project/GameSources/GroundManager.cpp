/*!
@file GroundManager.cpp
@brief 地面管理オブジェクトクラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void InstanceGround::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(m_groundColor);
	}

	// 生成時の処理
	void GroundManager::OnCreate()
	{
		// オブジェクトの生成
		const shared_ptr<Stage>& stagePtr = GetStage();
		m_groundMap.emplace(eStageID::Grass, stagePtr->AddGameObject<InstanceGround>(COL_GREAN));
		m_groundMap.emplace(eStageID::Rock, stagePtr->AddGameObject<InstanceGround>(COL_YELOW));
		m_groundMap.emplace(eStageID::Water, stagePtr->AddGameObject<InstanceGround>(COL_BG));

		// 地面マップ
		auto& groundMap = GetTypeStage<StageCSV>()->GetGroundMap();

		// 二重ループ
		for (size_t row = 0; row < groundMap.size(); row++)
		{
			for (size_t col = 0; col < groundMap.at(row).size(); col++)
			{
				// 座標の設定
				Vec3 addPos = Vec3(float(col), m_defPosY, -float(row));

				// トランスフォーム行列の設定
				Mat4x4 matrix, mtxPosition;
				mtxPosition.translation(addPos);

				// 行列の設定と追加
				matrix = m_mtxScale * m_mtxRotation * mtxPosition;

				// IDに応じて行列を追加
				eStageID id = STAGE_ID(groundMap.at(row).at(col));
				for (auto& groud : m_groundMap)
				{
					if (groud.first == id)
					{
						groud.second.lock()->AddMatrix(matrix);
					}
				}
			}
		}
	}
}