/*!
@file UnBreakRock.cpp
@brief 破壊できない岩描画オブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "OzawaUtility.h"
#include "UnBreakRock.h"
#include "BaseStage.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;
	using namespace UnBreakRockData;

	// 生成時の処理
	void InstanceRock::OnCreate()
	{
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(UnBreakRockMeshMap.at(m_meshType));
		m_ptrDraw->SetTextureResource(L"UN_BREAK_ROCK_TX");
		m_ptrDraw->SetOwnShadowActive(true);
	}

	// 生成時の処理
	void UnBreakRock::OnCreate()
	{
		// 継承元の生成時の処理
		TemplateObject::OnCreate();

		// ステージの取得
		const shared_ptr<BaseStage>& stagePtr = GetTypeStage<BaseStage>();
		
		// メッシュ事のインスタンス描画用オブジェクトを生成
		m_instanceMap.emplace(eRockMeshType::Mesh1, stagePtr->AddGameObject<InstanceRock>(eRockMeshType::Mesh1));
		m_instanceMap.emplace(eRockMeshType::Mesh2, stagePtr->AddGameObject<InstanceRock>(eRockMeshType::Mesh2));
		m_instanceMap.emplace(eRockMeshType::Mesh3, stagePtr->AddGameObject<InstanceRock>(eRockMeshType::Mesh3));

		// 生成処理
		CreateUnBreakRock();
	}

	// インスタンス描画の生成
	void UnBreakRock::CreateUnBreakRock()
	{
		// 行列の初期化
		for (auto& instance : m_instanceMap)
		{
			instance.second.lock()->ClearMatrix();
		}

		// ステージマップcsvから生成
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		for (int row = 0; row < stageMap.size(); row++)
		{
			for (int col = 0; col < stageMap.at(row).size(); col++)
			{
				// 壊せない岩じゃないなら無視
				if (eStageID::UnBreakRock != STAGE_ID(stageMap.at(row).at(col))) continue;

				// ローテーション
				Quat quatRot;
				float rot = RangeRand(2, -1) * XM_PIDIV2;
				quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, rot, 0.0f));
				m_mtxRotation.rotation(quatRot);

				// スケール
				float scaleY = m_deffScale * RangeRand(m_randRange, 1.0f);
				m_mtxScale.scale(Vec3(m_deffScale, scaleY, m_deffScale));

				// ポジション
				m_mtxPosition.translation(Vec3(float(col), m_deffPosY, -float(row)));

				// 追加する用の行列
				Mat4x4 matrix = m_mtxScale * m_mtxRotation * m_mtxPosition;

				// メッシュタイプをランダムに設定
				eRockMeshType type = static_cast<eRockMeshType>(RangeRand(2, 0));
				m_instanceMap.at(type).lock()->AddMatrix(matrix);
			}
		}
	}
}