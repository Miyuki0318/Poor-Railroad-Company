/*!
@file GroundManager.cpp
@brief 地面管理オブジェクトクラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

#define END_INDEX 10 // 区切るまでの数
#define WIN_RANGE 30 // カメラから画面外までのゲーム上での距離

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void InstanceGround::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetTextureResource(m_groundTexture);
		m_ptrDraw->SetOwnShadowActive(true);
	}

	// 生成時の処理
	void GroundManager::OnCreate()
	{
		// オブジェクトの生成
		const shared_ptr<Stage>& stagePtr = GetStage();

		// 地面マップ
		auto& groundMap = GetTypeStage<BaseStage>()->GetGroundMap();
		map<int, weak_ptr<InstanceGround>> grass, unGrass, sand, water, rock;
		for (int i = 0; i < groundMap.front().size(); i += END_INDEX)
		{
			// 区切り数ずつ追加
			grass.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_GRASS_TX"));
			unGrass.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_GRASS_TX"));
			sand.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_SAND_TX"));
			water.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_WATER_TX"));
			rock.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_ROCK_TX"));
		}

		// 各タイプで生成したマップをStageIDをキーに二次元マップ化
		m_groundMap.emplace(eStageID::Grass, grass);		// 草地
		m_groundMap.emplace(eStageID::UnGrass, unGrass);	// 通過不可の草地
		m_groundMap.emplace(eStageID::Sand, sand);			// 砂地
		m_groundMap.emplace(eStageID::Watering, water);		// 水場
		m_groundMap.emplace(eStageID::Rock, rock);			// 岩地


		// 二重ループ
		for (int row = 0; row < groundMap.size(); row++)
		{
			for (int col = 0; col < groundMap.at(row).size(); col++)
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
					// eStageIDが一致しないなら無視
					if (groud.first != id) continue;

					// タイプ事にループ
					for (auto& type : groud.second)
					{
						// colがインデックスからインデックス+区切る数の範囲内か
						if (type.first <= col && type.first + END_INDEX > col)
						{
							type.second.lock()->AddMatrix(matrix);
						}
					}
				}
			}
		}
	}

	// 毎フレーム更新処理
	void GroundManager::OnUpdate()
	{
		// カメラの注視点座標X軸
		int atX = int(GetStage()->GetView()->GetTargetCamera()->GetAt().x);

		// カメラの注視点X軸から計測して、大まかに画面内であれば表示する
		for (auto& groud : m_groundMap)
		{
			for (auto& type : groud.second)
			{
				// 開始インデックスが画面内であるか
				type.second.lock()->SetDrawActive(GetBetween(type.first, atX + WIN_RANGE, atX - WIN_RANGE));
			}
		}
	}
}