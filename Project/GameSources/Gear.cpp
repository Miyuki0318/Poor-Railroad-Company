/*!
@file Gear.cpp
@brief 踏切に使用する歯車
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void Gear::OnCreate()
	{
		// 継承元の初期化処理
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"GEAR");
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		// 影用描画コンポーネントの設定
		m_ptrShadow = AddComponent<Shadowmap>();
		m_ptrShadow->SetMeshResource(L"GEAR");
	}

	// アニメーションの更新
	void Gear::UpdateAnimation()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// ギアの非アクティブ化
	void Gear::GetGearSleap()
	{
		SetDrawActive(false);
		SetUpdateActive(false);
	}

	// ギアのアクティブ化
	void Gear::ResetGear()
	{
		SetDrawActive(true);
		SetUpdateActive(true);
	}

	// 生成時の処理
	void GearManager::OnCreate()
	{
		// ステージとステージcsvの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();

		// プレイヤーの取得
		m_playerPtr = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");

		// 二重ループ
		for (int row = 0; row < stageMap.size(); row++)
		{
			for (int col = 0; col < stageMap.at(row).size(); col++)
			{
				// 歯車以外のIDなら無視
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (id != eStageID::Gear) continue;

				// 歯車を生成し、マップに追加
				m_gearMap.emplace(ROWCOL2LINE(row, col), stagePtr->AddGameObject<Gear>(ROWCOL2POS(row, col)));
			}
		}

		// 生成した歯車に差分行列を設定
		for (auto& gear : m_gearMap)
		{
			auto& ptr = gear.second.lock();
			ptr->m_ptrDraw->SetMeshToTransformMatrix(m_gearModelMat);
			ptr->m_ptrShadow->SetMeshToTransformMatrix(m_gearModelMat);
		}
	}

	// 更新処理
	void GearManager::OnUpdate()
	{
		// プレイヤーの取得
		const auto& playerPtr = m_playerPtr.lock();
		if (!playerPtr) return;

		// プレイヤー座標を取得
		Vec3 playerPos = playerPtr->GetPosition();
		for (auto& gear : m_gearMap)
		{
			// 存在しない、非アクティブなら無視
			auto& ptr = gear.second.lock();
			if (!ptr) continue;
			if (!ptr->GetUpdateActive()) continue;

			// 距離を比較し、取得範囲じゃなければアニメーションの更新
			float length = (ptr->GetPosition() - playerPos).length();
			if (length > m_getGearRange)
			{
				ptr->UpdateAnimation();
				continue;
			}

			// 取得処理
			GetGearPlayar(gear);
		}
	}

	// 歯車の取得処理
	void GearManager::GetGearPlayar(const pair<string, weak_ptr<Gear>>& gear)
	{
		// ステージcsvの書き換え
		size_t row, col;
		GetLineStringToRowCol(row, col, gear.first);
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		stageMap.at(row).at(col) = UnSTAGE_ID(eStageID::None);

		// プレイヤーに歯車取得処理を送る
		const auto& playerPtr = dynamic_pointer_cast<GamePlayer>(m_playerPtr.lock());
		playerPtr->AddItemCount(eItemType::Gear);

		// 歯車に取得後の処理を送り、SEを再生
		auto& ptr = gear.second.lock();
		ptr->GetGearSleap();
		ptr->StartSE(L"C_SUCCES_SE", 1.0f);
	}

	// リセット処理
	void GearManager::ResetGears()
	{
		// 全ての歯車にリセット処理を送る
		for (auto& gear : m_gearMap)
		{
			gear.second.lock()->ResetGear();
		}
	}
}