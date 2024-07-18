/*!
@file Shop.cpp
@brief ショップ機能の実装
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Shop.h"
#include "ShopCloseState.h"
#include "OzawaUtility.h"

namespace basecross {
	void Shop::OnCreate()
	{
		// ステートマシンの初期化
		m_ShopState.reset(new ShopStateMachine(GetThis<Shop>()));
		// 最初のステートを設定
		m_ShopState->SetState(ShopCloseState::Instance());

		// 強化費用の最大桁数を取得
		m_maxDigit = DemandDigit();

		// 最大桁数分のサイズに拡張する
		for (int i = 0; i < eEnhancements::EnhancementsSize; i++) {
			m_enhancementsCostNumSprite.at(i).resize(m_maxDigit);
		}
		m_enhancementsNextCostSprite.resize(m_maxDigit);

		// 強化費用の表示座標を最大桁数分ずらす設定
		for (int i = 0; i < m_enhancementsCostSpritePos.size(); i++) {
			m_enhancementsCostSpritePos.at(i).x += m_maxDigit * m_numberScale.x;
		}
		m_nextCostSpritePos.x += m_maxDigit * m_numberScale.x;

		// ステージを取得
		const shared_ptr<Stage>& stagePtr = GetStage();

		// 購入画面用スプライトの設定
		m_purchaseScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_PURCHASESCREEN_TX", WINDOW_SIZE, m_screenSpritePos);
		m_purchaseScreenSprite.lock()->SetDrawLayer(5);
		m_purchaseScreenSprite.lock()->SetDrawActive(false);
		// 選択場所表示用スプライトの設定
		m_selectPointSprite = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", m_selectPointScale, m_selectPointSpritePos.at(m_currentEnhancements));
		m_selectPointSprite.lock()->SetDrawLayer(7);
		m_selectPointSprite.lock()->SetDrawActive(false);
		// 強化不可選択肢グレーアウト用スプライトの設定
		for (int i = 0; i < m_notSelectableSprite.size(); i++) {
			m_notSelectableSprite.at(i) = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", m_selectPointScale, m_selectPointSpritePos.at(i));
			m_notSelectableSprite.at(i).lock()->SetDrawLayer(8);
			m_notSelectableSprite.at(i).lock()->SetDiffuseColor(Col4(0.5f, 0.5f, 0.5f, 0.5f));
			m_notSelectableSprite.at(i).lock()->SetDrawActive(false);
		}
		// アイコン用スプライトの設定
		m_iconSprite.at(eOperationType::Controller) = stagePtr->AddGameObject<Sprite>(L"ABUTTON_TX", m_iconScale, m_iconPos);
		m_iconSprite.at(eOperationType::Controller).lock()->SetDrawLayer(8);
		m_iconSprite.at(eOperationType::Controller).lock()->SetDrawActive(false);
		m_iconSprite.at(eOperationType::Keyboard) = stagePtr->AddGameObject<Sprite>(L"RCLICK_TX", m_iconScale, m_iconPos);
		m_iconSprite.at(eOperationType::Keyboard).lock()->SetDrawLayer(8);
		m_iconSprite.at(eOperationType::Keyboard).lock()->SetDrawActive(false);
		// 購入内容確認画面用スプライトの設定
		m_confirmationScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CONFIRMATIONSCREEN_TX", WINDOW_SIZE, m_screenSpritePos);
		m_confirmationScreenSprite.lock()->SetDrawLayer(9);
		m_confirmationScreenSprite.lock()->SetDrawActive(false);
		// 強化内容「資材所持上限上昇」用スプライトの設定
		m_LimitChoicesSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_LIMITCHOICES_TX", m_enhancementsContentsScale, m_enhancementsContentsPos);
		m_LimitChoicesSprite.lock()->SetDrawLayer(10);
		m_LimitChoicesSprite.lock()->SetDrawActive(false);
		// 強化内容「ステータス」用スプライトの設定
		m_statusSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_SPEEDCHOICES_TX", m_enhancementsContentsScale, m_enhancementsContentsPos);
		m_statusSprite.lock()->SetDrawLayer(10);
		m_statusSprite.lock()->SetDrawActive(false);
		// 強化内容「踏切素材初期所持数」用スプライトの設定
		m_gearSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CROSSINGCHOICES_TX",m_enhancementsContentsScale, m_enhancementsContentsPos);
		m_gearSprite.lock()->SetDrawLayer(10);
		m_gearSprite.lock()->SetDrawActive(false);
		// 選択肢スプライトの設定
		m_confirmSprite.emplace(eConfirmState::Yes, stagePtr->AddGameObject<Sprite>(L"SHOP_YES_TX", m_confirmSpriteScale, m_confirmSpritePos.at(eConfirmState::Yes)));
		m_confirmSprite.at(eConfirmState::Yes).lock()->SetDrawLayer(10);
		m_confirmSprite.at(eConfirmState::Yes).lock()->SetDrawActive(false);
		m_confirmSprite.emplace(eConfirmState::No, stagePtr->AddGameObject<Sprite>(L"SHOP_NO_TX", m_confirmSpriteScale, m_confirmSpritePos.at(eConfirmState::No)));
		m_confirmSprite.at(eConfirmState::No).lock()->SetDrawLayer(10);
		m_confirmSprite.at(eConfirmState::No).lock()->SetDrawActive(false);
	}

	void Shop::OnUpdate()
	{
		// ステートマシンの更新処理を送る
		m_ShopState->Update();
	}

	int Shop::DemandDigit() {
		// 最大費用
		int maxCost = 0;
		// 最大桁数
		int maxDigit = 0;

		// 最大費用を計算
		for (int i = 0; i < m_enhancementsCost.size(); i++) {
			for (int j = 0; j < m_enhancementsCost.at(i).size(); j++) {
				// 現在の最大費用と比較
				if (maxCost < m_enhancementsCost.at(i).at(j)) {
					// 最大費用を更新
					maxCost = m_enhancementsCost.at(i).at(j);
				}
			}
		}

		// 最大桁数を計算
		while (maxCost > 0) {
			maxDigit++;
			maxCost /= 10;
		}

		// 最大桁数を返す
		return maxDigit;
	}


}