/*!
@file Shop.cpp
@brief ショップ機能の実装
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Shop::OnCreate()
	{
		// ステートマシンの初期化
		m_ShopState.reset(new ShopStateMachine(GetThis<Shop>()));
		// 最初のステートを設定
		m_ShopState->SetState(ShopCloseState::Instance());

		// ステージを取得
		const shared_ptr<Stage>& stagePtr = GetStage();


		// 選択肢の表示位置を設定
		m_selectPointPos = vector<Vec3>{
			Vec3(-179.0f, 181.0f, 0.0f),
			Vec3(-179.0f, -65.5f, 0.0f),
			Vec3(-179.0f, -320.0f, 0.0f)
		};

		// 現在Lvの表示位置を設定
		m_playerLvPos = vector<Vec3>{
			Vec3(-370.0f, -428.0f + 540.0f, 0.0f),
			Vec3(-370.0f, -673.0f + 540.0f, 0.0f),
			Vec3(-370.0f, -928.0f + 540.0f, 0.0f)
		};

		// 強化費用の表示位置を設定
		m_enhancementCostPos = vector<Vec3>{
			Vec3(160.0f, -428.0f + 540.0f, 0.0f),
			Vec3(160.0f, -673.0f + 540.0f, 0.0f),
			Vec3(160.0f, -928.0f + 540.0f, 0.0f)
		};



		m_purchaseScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_PURCHASESCREEN_TX", Vec2(1920.0f, 1080.0f), Vec3(-400.0f, 0.0f, 0.0f));
		m_purchaseScreenSprite->SetDrawLayer(10);
		m_purchaseScreenSprite->SetDrawActive(true);
		m_selectPointSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_SELECTPOINT_TX", Vec2(1420.0f, 816.0), m_selectPointPos.at(m_currentEnhancements));
		m_selectPointSprite->SetDrawLayer(12);
		m_selectPointSprite->SetDrawActive(false);
		m_confirmationScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CONFIRMATIONSCREEN_TX", Vec2(1920.0f, 1080.0f), Vec3(-400.0f, 0.0f, 0.0f));
		m_confirmationScreenSprite->SetDrawLayer(13);
		m_confirmationScreenSprite->SetDrawActive(false);
		m_backpackSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_BACKPACK_TX", Vec2(600.0f, 200.0f), Vec3(-170.0f, 100.0f, 0.0f));
		m_backpackSprite->SetDrawLayer(14);
		m_backpackSprite->SetDrawActive(false);
		m_statusSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_STATUS_TX", Vec2(600.0f, 200.0f), Vec3(-170.0f, 100.0f, 0.0f));
		m_statusSprite->SetDrawLayer(14);
		m_statusSprite->SetDrawActive(false);
		m_gearSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_GEAR_TX", Vec2(600.0f, 200.0f), Vec3(-170.0f, 100.0f, 0.0f));
		m_gearSprite->SetDrawLayer(14);
		m_gearSprite->SetDrawActive(false);
		m_yesSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_YES_TX", Vec2(600.0f, 150.0f), Vec3(-400.0f, -270.0f, 0.0f));
		m_yesSprite->SetDrawLayer(14);
		m_yesSprite->SetDrawActive(false);
		m_noSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_NO_TX", Vec2(600.0f, 150.0f), Vec3(80.0f, -270.0f, 0.0f));
		m_noSprite->SetDrawLayer(14);
		m_noSprite->SetDrawActive(false);



	}

	void Shop::OnUpdate()
	{
		// ステートマシンの更新処理を送る
		m_ShopState->Update();
	}
}