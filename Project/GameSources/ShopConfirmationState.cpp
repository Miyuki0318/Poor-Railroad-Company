/*!
@file ShopConfirmationState.cpp
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#include "stdafx.h"
#include "TitleStage.h"
#include "Scene.h"
#include "Input.h"
#include "Number.h"
#include "Shop.h"
#include "ShopShoppingState.h"
#include "ShopConfirmationState.h"
#include "MoneyCountUI.h"

namespace basecross
{
	// ステート開始時の処理
	void ShopConfirmationState::Enter(const shared_ptr<Shop>& shop)
	{
		// 現在のステートをConfirmationに変更
		shop->m_currentState = Shop::eShopState::Confirmation;

		// 変数の初期化
		m_isEnhancements = false;

		// スプライトの表示処理
		shop->m_confirmationScreenSprite.lock()->SetDrawActive(true);
		shop->m_yesSprite.lock()->SetDrawActive(true);
		shop->m_noSprite.lock()->SetDrawActive(true);
		switch (shop->m_currentEnhancements)
		{
		case Shop::eEnhancements::Backpack:
			shop->m_backpackSprite.lock()->SetDrawActive(true);
			break;

		case Shop::eEnhancements::Status:
			shop->m_statusSprite.lock()->SetDrawActive(true);
			break;

		case Shop::eEnhancements::Gear:
			shop->m_gearSprite.lock()->SetDrawActive(true);
			break;

		default:
			break;
		}


		// ステージを取得
		const weak_ptr<TitleStage>& stagePtr = shop->GetTypeStage<TitleStage>();
		// Sceneを取得
		const weak_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// 選択内容に応じて取得するデータを変える
		switch (shop->m_currentEnhancements)
		{
		case Shop::eEnhancements::Backpack:
			// 現在のレベルを取得
			m_currentLv = int(scenePtr.lock()->GetBackPackLevel());
			break;

		case Shop::eEnhancements::Status:
			// 現在のレベルを取得
			m_currentLv = int(scenePtr.lock()->GetStatusLevel());
			break;

		case Shop::eEnhancements::Gear:
			// 現在のレベルを取得
			m_currentLv = int(scenePtr.lock()->GetStartGearLevel());
			break;

		default:
			break;
		}

		// Lv用数字スプライトの生成
		const weak_ptr<Number>& lvSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale), Vec3(shop->m_nextLevelSpritePos), L"SHOP_NUMBER", m_currentLv + 1);
		// 次のレベル表示スプライト用の変数に格納
		shop->m_enhancementsNextLvSprite = lvSprite;

		// 強化費用を取得
		m_cost = shop->m_enhancementsCost.at(shop->m_currentEnhancements).at(m_currentLv);

		// 10^(最大桁数分-1)の数値を取得
		int digit = int(pow(10, shop->m_maxDigit - 1));

		// 費用を一番上の桁から生成
		for (int i = 0; i < shop->m_maxDigit; i++) {
			// 表示する数字を計算
			int num = m_cost / digit;
			// 費用表示スプライトの生成
			auto& costSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale),
				Vec3(shop->m_nextCostSpritePos + (shop->m_numberMargin * float(i))), L"SHOP_NUMBER", num);
			// 数字スプライト保持用の配列に格納
			shop->m_enhancementsNextCostSprite.at(i) = costSprite;
			// digitの桁数を減らす
			digit /= 10;
		}

		// 数字スプライトのレイヤー設定
		shop->m_enhancementsNextLvSprite.lock()->SetDrawLayer(15);
		for (int i = 0; i < shop->m_enhancementsCostNumSprite.size(); i++) {
			if (!shop->m_enhancementsNextCostSprite.at(i).lock()) continue;
			shop->m_enhancementsNextCostSprite.at(i).lock()->SetDrawLayer(15);
		}
	}

	// ステート更新時の処理
	void ShopConfirmationState::Execute(const shared_ptr<Shop>& shop)
	{
		// Sceneを取得
		const weak_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// Aボタン入力があった場合、ショップを閉じる
		if (Input::GetPushA()) {
			shop->SetState(ShopShoppingState::Instance());
			// 決定時の効果音を再生
			shop->StartSE(L"PUSH_SE", 1.0f);
			return;
		}

		// Bボタン入力があった場合、選択内容を強化
		if (Input::GetPushB()) {
			if (m_isEnhancements) {
				// 選択内容に応じて取得するデータを変える
				switch (shop->m_currentEnhancements)
				{
				case Shop::eEnhancements::Backpack:
					// レベルを上げる処理
					scenePtr.lock()->SetBackPackLevel(ePL(m_currentLv + 1));
					break;

				case Shop::eEnhancements::Status:
					// レベルを上げる処理
					scenePtr.lock()->SetStatusLevel(ePL(m_currentLv + 1));
					break;

				case Shop::eEnhancements::Gear:
					// レベルを上げる処理
					scenePtr.lock()->SetStartGearLevel(ePL(m_currentLv + 1));
					break;

				default:
					break;
				}

				// 所持金を取得
				int money = scenePtr.lock()->GetMoney();
				// 所持金を減少
				money -= m_cost;
				// 所持金の減少を適用
				scenePtr.lock()->SetMoney(money);
				// UIに更新処理を送る
				auto& ui = scenePtr.lock()->GetActiveStage()->GetSharedGameObject<MoneyCountUI>(L"MoneyUI");
				ui->SetNumberGoal(money);
				// 強化完了時の効果音を鳴らす
				shop->StartSE(L"BUY_SE", 2.0f);

				// ステートを変更
				shop->SetState(ShopShoppingState::Instance());
				return;
			}
			else {
				// 決定時の効果音を再生
				shop->StartSE(L"PUSH_SE", 1.0f);

				// ステートを変更
				shop->SetState(ShopShoppingState::Instance());
				return;

			}
		}

		// 一定以上のLスティック入力があった場合のみ選択処理を行う
		if (abs(Input::GetLStickValue().x) >= m_LStickLowerLimit) {
			if (!m_isInputLStick) {
				if (m_isEnhancements) {
					// 選択肢の拡縮処理
					m_isEnhancements = false;
				}
				else {
					m_isEnhancements = true;
				}
				// 経過時間をリセットする
				m_totalTime = 0.0f;

				// スティック入力の効果音を再生
				shop->StartSE(L"SELECT_SE", 1.0f);
			}
			// 入力量が下限値を超えていた為、スティック入力があったと判定する
			m_isInputLStick = true;
		}
		else {
			// 入力量が下限値を下回っていた為、スティック入力が無かったと判定する
			m_isInputLStick = false;
		}

		// 選択肢の拡大率
		float spriteSizeRange = 0.0f;

		// 経過時間に応じて点滅の切り替え
		if (m_totalTime <= 0.5f)
		{
			spriteSizeRange = Utility::Lerp(shop->m_spriteSizeRange, 1.0f, m_totalTime * 2.0f);
		}
		else {
			if (m_totalTime <= 1.0f) {
				spriteSizeRange = Utility::Lerp(1.0f, shop->m_spriteSizeRange, (m_totalTime - 0.5f ) * 2.0f);
			}
		}

		// 経過時間を更新
		m_totalTime += DELTA_TIME;

		// 経過時間が1秒を経過していた場合、経過時間をリセットする
		if (m_totalTime > 1.0f) {
			m_totalTime = 0.0f;
		}

		// 透明度を更新
		if (m_isEnhancements) {
			shop->m_yesSprite.lock()->SetScale(shop->m_confirmSpritedefScale * spriteSizeRange);
			shop->m_noSprite.lock()->SetScale(shop->m_confirmSpritedefScale);
		}
		else {
			shop->m_yesSprite.lock()->SetScale(shop->m_confirmSpritedefScale);
			shop->m_noSprite.lock()->SetScale(shop->m_confirmSpritedefScale * spriteSizeRange);
		}		
	}

	// ステート終了時の処理
	void ShopConfirmationState::Exit(const shared_ptr<Shop>& shop)
	{
		// スプライトの非表示処理
		shop->m_confirmationScreenSprite.lock()->SetDrawActive(false);
		shop->m_yesSprite.lock()->SetDrawActive(false);
		shop->m_noSprite.lock()->SetDrawActive(false);

		switch (shop->m_currentEnhancements)
		{
		case Shop::eEnhancements::Backpack:
			shop->m_backpackSprite.lock()->SetDrawActive(false);
			break;

		case Shop::eEnhancements::Status:
			shop->m_statusSprite.lock()->SetDrawActive(false);
			break;

		case Shop::eEnhancements::Gear:
			shop->m_gearSprite.lock()->SetDrawActive(false);
			break;

		default:
			break;
		}

		shop->m_enhancementsNextLvSprite.lock()->SetDrawActive(false);
		for (int i = 0; i < shop->m_enhancementsCostNumSprite.size(); i++) {
			if (!shop->m_enhancementsNextCostSprite.at(i).lock()) continue;
			shop->m_enhancementsNextCostSprite.at(i).lock()->SetDrawActive(false);
		}

	}

	// インスタンス生成
	shared_ptr<ShopConfirmationState> ShopConfirmationState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<ShopConfirmationState> instance(new ShopConfirmationState);

		// 新しく生成されたthisポインタ
		return instance;
	}
}