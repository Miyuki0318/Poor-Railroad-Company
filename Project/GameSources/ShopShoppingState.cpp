/*!
@file ShopShoppingState.cpp
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#include "stdafx.h"
#include "TitleStage.h"
#include "Scene.h"
#include "Input.h"
#include "Company.h"
#include "Shop.h"
#include "ShopCloseState.h"
#include "ShopShoppingState.h"
#include "ShopConfirmationState.h"

namespace basecross
{
	// ステート開始時の処理
	void ShopShoppingState::Enter(const shared_ptr<Shop>& shop)
	{
		// ステージを取得
		const weak_ptr<TitleStage>& stagePtr = shop->GetTypeStage<TitleStage>();
		// Sceneを取得
		const weak_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// 現在のステートをShoppingに変更
		shop->m_currentState = Shop::eShopState::Shopping;

		// タイトルステージのプログレスを変更
		auto& progless = stagePtr.lock()->GetTitleProgress();
		progless = eTitleProgress::shop;

		// スプライトの表示処理
		shop->m_purchaseScreenSprite.lock()->SetDrawActive(true);
		shop->m_selectPointSprite.lock()->SetDrawActive(true);


		// スプライトの座標を選択内容に応じて更新
		shop->m_selectPointSprite.lock()->SetPosition(shop->m_selectPointSpritePos.at(shop->m_currentEnhancements));

		// 各Lv取得用変数
		vector<ePL> Lv;

		for (int i = 0; i < Shop::eEnhancements::size; i++)
		{
			switch (i)
			{
			case Shop::eEnhancements::Backpack:
				// バックパックのレベルを取得
				Lv.push_back(scenePtr.lock()->GetBackPackLevel());
				break;

			case Shop::eEnhancements::Status:
				// ステータスのレベルを取得
				Lv.push_back(scenePtr.lock()->GetStatusLevel());
				break;

			case Shop::eEnhancements::Gear:
				// ギアのレベルを取得
				Lv.push_back(scenePtr.lock()->GetStartGearLevel());
				break;
			default:
				break;
			}
			// Lv用数字スプライトの生成
			auto& lvSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale), Vec3(shop->m_playerLvSpritePos.at(i)), L"SHOP_NUMBER", int(Lv.at(i)) + 1);
			// 数字スプライト保持用の配列に格納
			shop->m_playerLvNumSprite[i] = lvSprite;

			// 強化費用を入れる変数
			int cost = 0;
			// 現在のレベルが最大になっていないかをチェック
			if (Lv.at(i) != ePL::Level5) {
				// 強化費用を取得
				cost = shop->m_enhancementsCost.at(i).at(int(Lv.at(i)));
			}

			// 10^(最大桁数分-1)の数値を取得
			int digit = int(pow(10, shop->m_maxDigit - 1));

			// 費用を一番上の桁から生成
			for (int j = 0; j < shop->m_maxDigit; j++) {
				// 表示する数字を計算
				int num = cost / digit;
				// 費用表示スプライトの生成
				auto& costSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale),
					Vec3(shop->m_enhancementsCostSpritePos.at(i) + (shop->m_numberMargin * float(j))), L"SHOP_NUMBER", num);
				// 数字スプライト保持用の配列に格納
				shop->m_enhancementsCostNumSprite.at(i).at(j) = costSprite;
				// digitの桁数を減らす
				digit /= 10;
			}

			// 所持金を取得
			int money = scenePtr.lock()->GetMoney();

			// 現在のレベルが最大になっていないかをチェック
			if (Lv.at(i) != ePL::Level5) {
				// 費用と所持金を比較し、強化可能かを判別
				if (shop->m_enhancementsCost.at(i).at(int(Lv.at(i))) <= money) {
					shop->m_canEnhancements.at(i) = true;
				}
			}
			else {
				shop->m_canEnhancements.at(i) = false;
			}
		}

		// 強化できない場所のスプライトを表示する
		for (int i = 0; i < shop->m_notSelectableSprite.size(); i++) {
			if (!shop->m_canEnhancements.at(i)) {
				shop->m_notSelectableSprite.at(i).lock()->SetDrawActive(true);
			}
		}

		// 数字スプライトのレイヤー設定
		for (int i = 0; i < shop->m_playerLvNumSprite.size(); i++) {
			if (!shop->m_playerLvNumSprite.at(i).lock()) continue;
			shop->m_playerLvNumSprite.at(i).lock()->SetDrawLayer(11);
		}
		for (int i = 0; i < shop->m_enhancementsCostNumSprite.size(); i++) {
			for (int j = 0; j < shop->m_enhancementsCostNumSprite.at(i).size(); j++) {
				if (!shop->m_enhancementsCostNumSprite.at(i).at(j).lock()) continue;
				shop->m_enhancementsCostNumSprite.at(i).at(j).lock()->SetDrawLayer(11);
			}
		}
	}

	// ステート更新時の処理
	void ShopShoppingState::Execute(const shared_ptr<Shop>& shop)
	{
		// Aボタン入力の取得
		bool isPushA = Input::GetPushA();
		// Bボタン入力の取得
		bool isPushB = Input::GetPushB();
		// LStickのY軸入力量を取得
		float stickValueY = Input::GetLStickValue().y;

		// Aボタン入力があった場合、ショップを閉じる
		if (isPushA) {
			shop->StartSE(L"CLOSE_SE", 1.0f);
			// ステージを取得
			const weak_ptr<TitleStage>& stagePtr = shop->GetTypeStage<TitleStage>();
			// タイトルステージのプログレスを変更
			auto& progless = stagePtr.lock()->GetTitleProgress();
			progless = eTitleProgress::normal;
			// 会社のSELECTタグを外す
			weak_ptr<Company> companyPtr = stagePtr.lock()->GetSharedGameObject<Company>(L"Company");
			companyPtr.lock()->RemoveTag(L"SELECT");

			// ステートを変更
			shop->SetState(ShopCloseState::Instance());
			return;
		}

		// Bボタン入力があった場合、購入確認画面に遷移
		if (isPushB) {
			if (shop->m_canEnhancements.at(shop->m_currentEnhancements)) {
				// 決定時の効果音を再生
				shop->StartSE(L"PUSH_SE", 1.0f);

				// ステートを変更
				shop->SetState(ShopConfirmationState::Instance());
				return;
			}
			else {
				// 選択不可の効果音を再生
				shop->StartSE(L"C_FAILURE_SE", 1.0f);				
			}
		}

		// 一定以上のLスティック入力があった場合のみ選択処理を行う
		if (abs(stickValueY) >= m_LStickLowerLimit) {
			if (!m_isInputLStick) {
				// LStickの入力方向に応じて選択肢を変更する
				if (stickValueY < 0.0f) {
					// 現在の選択内容に応じて次の選択内容を変更する
					switch (shop->m_currentEnhancements)
					{
					case Shop::eEnhancements::Backpack:
						// 選択内容をステータスに変更
						shop->m_currentEnhancements = Shop::eEnhancements::Status;
						break;
					case Shop::eEnhancements::Status:
						// 選択内容をギアに変更
						shop->m_currentEnhancements = Shop::eEnhancements::Gear;
						break;
					case Shop::eEnhancements::Gear:
						// 選択内容をバックパックに変更
						shop->m_currentEnhancements = Shop::eEnhancements::Backpack;
						break;
					default:
						break;
					}
				}
				else {
					// 現在の選択内容に応じて次の選択内容を変更する
					switch (shop->m_currentEnhancements)
					{
					case Shop::eEnhancements::Backpack:
						// 選択内容をギアに変更
						shop->m_currentEnhancements = Shop::eEnhancements::Gear;
						break;
					case Shop::eEnhancements::Status:
						// 選択内容をバックパックに変更
						shop->m_currentEnhancements = Shop::eEnhancements::Backpack;
						break;
					case Shop::eEnhancements::Gear:
						// 選択内容をステータスに変更
						shop->m_currentEnhancements = Shop::eEnhancements::Status;
						break;
					default:
						break;
					}
				}

				// スプライトの座標を選択内容に応じて更新
				shop->m_selectPointSprite.lock()->SetPosition(shop->m_selectPointSpritePos.at(shop->m_currentEnhancements));

				// スティック入力の効果音を再生
				shop->StartSE(L"SELECT_SE", 1.0f);

				// 経過時間をリセットする
				m_totalTime = 0.0f;
			}
			// 入力量が下限値を超えていた為、スティック入力があったと判定する
			m_isInputLStick = true;
		}
		else {
			// 入力量が下限値を下回っていた為、スティック入力が無かったと判定する
			m_isInputLStick = false;
		}

		// 選択肢の点滅処理
		float transparency = 0.0f;

		// 経過時間に応じて点滅の切り替え
		if (m_totalTime <= 0.5f)
		{
			transparency = Utility::Lerp(shop->m_transparencyLimit, 0.0f, m_totalTime * 2.0f);
		}
		else {
			if (m_totalTime <= 1.0f) {
				transparency = Utility::Lerp(0.0f, shop->m_transparencyLimit, (m_totalTime - 0.5f) * 2.0f);
			}
		}

		// 経過時間を更新
		m_totalTime += DELTA_TIME;

		// 経過時間が1秒を経過していた場合、経過時間をリセットする
		if (m_totalTime > 1.0f) {
			m_totalTime = 0.0f;
		}

		// 透明度を更新
		shop->m_selectPointSprite.lock()->SetDiffuseColor(Col4(1.0f, 1.0f, 1.0f, transparency));
	}

	// ステート終了時の処理
	void ShopShoppingState::Exit(const shared_ptr<Shop>& shop)
	{
		// スプライトの非表示処理
		shop->m_purchaseScreenSprite.lock()->SetDrawActive(false);
		shop->m_selectPointSprite.lock()->SetDrawActive(false);
		for (int i = 0; i < shop->m_notSelectableSprite.size(); i++) {
			shop->m_notSelectableSprite.at(i).lock()->SetDrawActive(false);
		}
		for (int i = 0; i < shop->m_playerLvNumSprite.size(); i++) {
			shop->m_playerLvNumSprite.at(i).lock()->SetDrawActive(false);
		}
		for (int i = 0; i < shop->m_enhancementsCostNumSprite.size(); i++) {
			for (int j = 0; j < shop->m_enhancementsCostNumSprite.at(i).size(); j++) {
				shop->m_enhancementsCostNumSprite.at(i).at(j).lock()->SetDrawActive(false);
			}
		}

	}

	// インスタンス生成
	shared_ptr<ShopShoppingState> ShopShoppingState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<ShopShoppingState> instance(new ShopShoppingState);

		// 新しく生成されたthisポインタ
		return instance;
	}
}