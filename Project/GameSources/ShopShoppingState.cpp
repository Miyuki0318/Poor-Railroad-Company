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

		for (int i = 0; i < Shop::eEnhancements::EnhancementsSize; i++)
		{
			switch (i)
			{
			case Shop::eEnhancements::LimitChoices:
				// 資材所持上限のレベルを取得
				Lv.push_back(scenePtr.lock()->GetLimitChoicesLevel());
				break;

			case Shop::eEnhancements::Status:
				// 採掘/移動速度強化のレベルを取得
				Lv.push_back(scenePtr.lock()->GetStatusLevel());
				break;

			case Shop::eEnhancements::Gear:
				// 踏切素材初期所持数のレベルを取得
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
			// 10^(桁数分-1)の数値を取得する為の変数
			int place = 0;

			// 現在のレベルが最大になっていないかをチェック
			if (Lv.at(i) != ePL::Level5) {
				// 強化費用を取得
				m_cost.at(i) = shop->m_enhancementsCost.at(i).at(int(Lv.at(i)));
				// 桁数を初期化
				m_digit.at(i) = 0;
			}
			// 現在のレベルが最大の場合は0を表示
			else {
				m_cost.at(i) = 0;
				// 桁数を1に設定
				m_digit.at(i) = 1;
				// 0を0で割ることはできないので1を入れる
				place = 1;
			}
			// 強化費用の桁数
			int& digit = m_digit.at(i);
			// 現在のレベルが最大になっていないかをチェック
			if (Lv.at(i) != ePL::Level5) {
				// 強化費用を取得
				cost = m_cost.at(i);
				// 桁数を計算
				while (cost > 0) {
					digit++;
					cost /= 10;
				}

				// 10^(桁数-1)の数値を取得する
				place = int(pow(10, digit - 1));

				// 強化費用を再取得
				cost = m_cost.at(i);
			}

			// 費用を一番上の桁から生成
			for (int j = 0; j < digit; j++) {
				// 表示する数字を計算
				int num = cost / place;
				// 費用表示スプライトの生成
				auto& costSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale),
					Vec3(shop->m_enhancementsCostSpritePos.at(i) + (-shop->m_numberMargin * float(digit - j))), L"SHOP_NUMBER", num);
				// 数字スプライト保持用の配列に格納
				shop->m_enhancementsCostNumSprite.at(i).at(j) = costSprite;
				// 費用を再計算
				cost = m_cost.at(i) % place;
				// digitの桁数を減らす
				place /= 10;
			}

			// 桁数分のスプライトのレイヤーを設定
			for (int j = 0; j < digit; j++) {
				if (!shop->m_enhancementsCostNumSprite.at(i).at(j).lock()) continue;
				shop->m_enhancementsCostNumSprite.at(i).at(j).lock()->SetDrawLayer(6);
			}

			// 所持金を取得
			int money = scenePtr.lock()->GetMoney();

			// 現在のレベルが最大になっていないこと、費用と所持金を比較することで、強化可能かを判別
			if (Lv.at(i) != ePL::Level5 && m_cost.at(i) <= money) {
				// 強化可能
				shop->m_canEnhancements.at(i) = true;
			}
			else {
				// 強化不可
				shop->m_canEnhancements.at(i) = false;
			}
		}

		// 強化できない場所のスプライトを表示する
		for (int i = 0; i < shop->m_notSelectableSprite.size(); i++) {
			if (!shop->m_canEnhancements.at(i)) {
				// 選択箇所をグレーのスプライトで覆う
				shop->m_notSelectableSprite.at(i).lock()->SetDrawActive(true);
			}
		}

		// 数字スプライトのレイヤーを設定
		for (int i = 0; i < shop->m_playerLvNumSprite.size(); i++) {
			if (!shop->m_playerLvNumSprite.at(i).lock()) continue;
			shop->m_playerLvNumSprite.at(i).lock()->SetDrawLayer(6);
		}
	}

	// ステート更新時の処理
	void ShopShoppingState::Execute(const shared_ptr<Shop>& shop)
	{
		// Aボタン入力の取得(ゲームパッド未接続時は右クリックの取得)
		bool isPushA = Input::GetPushA();
		// Bボタン入力の取得(ゲームパッド未接続時は左クリックの取得)
		bool isPushB = Input::GetPushB();

		// コントローラーの接続に応じて選択処理を行う
		Input::GetPadConected() ? ControllerSelect(shop) : MouseSelect(shop);

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

		// 選択肢の点滅処理
		float transparency = 0.0f;
		float hulfLimit = m_totalTimeLimit / 2.0f;

		// 経過時間に応じて点滅の切り替え
		if (m_totalTime <= hulfLimit)
		{
			transparency = Utility::Lerp(shop->m_transparencyLimit, 0.0f, m_totalTime * 2.0f);
		}
		else {
			transparency = Utility::Lerp(0.0f, shop->m_transparencyLimit, (m_totalTime - hulfLimit) * 2.0f);
		}

		// 経過時間を更新
		m_totalTime += DELTA_TIME;

		// 経過時間が1秒を経過していた場合、経過時間をリセットする
		if (m_totalTime > m_totalTimeLimit) {
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
		shop->m_iconSprite.at(Shop::eOperationType::Controller).lock()->SetDrawActive(false);
		shop->m_iconSprite.at(Shop::eOperationType::Keyboard).lock()->SetDrawActive(false);

		for (int i = 0; i < shop->m_notSelectableSprite.size(); i++) {
			shop->m_notSelectableSprite.at(i).lock()->SetDrawActive(false);
		}
		for (int i = 0; i < shop->m_playerLvNumSprite.size(); i++) {
			shop->m_playerLvNumSprite.at(i).lock()->SetDrawActive(false);
		}
		for (int i = 0; i < shop->m_enhancementsCostNumSprite.size(); i++) {
			for (int j = 0; j < m_digit.at(i); j++) {
				shop->m_enhancementsCostNumSprite.at(i).at(j).lock()->SetDrawActive(false);
			}
		}
	}

	void ShopShoppingState::ControllerSelect(const shared_ptr<Shop>& shop) {
		// スプライトの表示を変更
		shop->m_iconSprite.at(Shop::eOperationType::Controller).lock()->SetDrawActive(true);
		shop->m_iconSprite.at(Shop::eOperationType::Keyboard).lock()->SetDrawActive(false);

		// LStickのY軸入力量を取得
		float stickValueY = Input::GetLStickValue().y;

		// 一定以上のLスティック入力があった場合のみ選択処理を行う
		if (abs(stickValueY) >= m_LStickLowerLimit) {
			if (!m_isInputLStick) {
				// LStickの入力方向に応じて選択肢を変更する
				if (stickValueY < 0.0f) {
					// 現在の選択内容に応じて次の選択内容を変更する
					switch (shop->m_currentEnhancements)
					{
					case Shop::eEnhancements::LimitChoices:
						// 選択内容を採掘/移動速度強化に変更
						shop->m_currentEnhancements = Shop::eEnhancements::Status;
						break;
					case Shop::eEnhancements::Status:
						// 選択内容を踏切素材初期所持数に変更
						shop->m_currentEnhancements = Shop::eEnhancements::Gear;
						break;
					case Shop::eEnhancements::Gear:
						// 選択内容を資材所持上限上昇に変更
						shop->m_currentEnhancements = Shop::eEnhancements::LimitChoices;
						break;
					default:
						break;
					}
				}
				else {
					// 現在の選択内容に応じて次の選択内容を変更する
					switch (shop->m_currentEnhancements)
					{
					case Shop::eEnhancements::LimitChoices:
						// 選択内容を踏切素材初期所持数に変更
						shop->m_currentEnhancements = Shop::eEnhancements::Gear;
						break;
					case Shop::eEnhancements::Status:
						// 選択内容を資材所持上限上昇に変更
						shop->m_currentEnhancements = Shop::eEnhancements::LimitChoices;
						break;
					case Shop::eEnhancements::Gear:
						// 選択内容を採掘/移動速度強化に変更
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
			// Lスティック入力の下限値ギリギリで入力され続けることを防止する為の処理
			if (stickValueY <= 0.1f) {
				// 入力量が下限値を下回っていた為、スティック入力が無かったと判定する
				m_isInputLStick = false;
			}
		}
	}

	void ShopShoppingState::MouseSelect(const shared_ptr<Shop>& shop) {
		// スプライトの表示を変更
		shop->m_iconSprite.at(Shop::eOperationType::Controller).lock()->SetDrawActive(false);
		shop->m_iconSprite.at(Shop::eOperationType::Keyboard).lock()->SetDrawActive(true);

		// マウスの座標を取得
		Vec2 mousePos = Input::GetMousePosition();
		// 選択範囲の半分のスケールを取得
		Vec2 helfScale = shop->m_selectPointScale / 2.0f;

		// 選択肢の数ループ
		for (int i = 0; i < shop->m_selectPointSpritePos.size(); i++) {
			// 選択肢の中心座標
			Vec2 selectPos = Vec2(shop->m_selectPointSpritePos.at(i).x, shop->m_selectPointSpritePos.at(i).y);
			// 選択範囲にマウスカーソルがあるかをチェック
			if (Utility::GetBetween(mousePos, selectPos + helfScale, selectPos - helfScale)) {
				if (shop->m_currentEnhancements != Shop::eEnhancements(i)) {
					// スティック入力の効果音を再生
					shop->StartSE(L"SELECT_SE", 1.0f);
				}
				// 選択内容を変更
				shop->m_currentEnhancements = Shop::eEnhancements(i);

				// スプライトの座標を選択内容に応じて更新
				shop->m_selectPointSprite.lock()->SetPosition(shop->m_selectPointSpritePos.at(shop->m_currentEnhancements));

				// 経過時間をリセットする
				m_totalTime = 0.0f;
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