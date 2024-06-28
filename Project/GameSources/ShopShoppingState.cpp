/*!
@file ShopShoppingState.cpp
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ステート開始時の処理
	void ShopShoppingState::Enter(const shared_ptr<Shop>& shop)
	{
		// 現在のステートをShoppingに変更
		shop->m_currentState = Shop::ShopState::Shopping;
		// スプライトの表示処理
		shop->m_purchaseScreenSprite->SetDrawActive(true);
		shop->m_selectPointSprite->SetDrawActive(true);

		// 配列を初期化
		shop->m_numbers.clear();

		// ステージを取得
		const shared_ptr<Stage>& stagePtr = shop->GetStage();
		// Sceneを取得
		const shared_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// 各Lv取得用変数
		vector<ePL> Lv;

		for (int i = 0; i < Shop::Enhancements::size; i++)
		{
			switch (i)
			{
			case Shop::Enhancements::Backpack:
				// バックパックのレベルを取得
				Lv.push_back(scenePtr->GetBackPackLevel());
				break;

			case Shop::Enhancements::Status:
				// ステータスのレベルを取得
				Lv.push_back(scenePtr->GetStatusLevel());
				break;

			case Shop::Enhancements::Gear:
				// ギアのレベルを取得
				Lv.push_back(scenePtr->GetStartGearLevel());
				break;
			default:
				break;
			}
			// Lv用数字スプライトの生成
			auto& lvSprite = stagePtr->AddGameObject<Number>(Vec2(shop->m_numberScale), Vec3(shop->m_playerLvPos.at(i)), L"SHOP_NUMBER", int(Lv.at(i)) + 1);
			// 数字スプライト保持用の配列に格納
			shop->m_numbers.push_back(lvSprite);

			int cost = shop->m_enhancementCost.at(i).at(int(Lv.at(i)));
			// 費用表示スプライトの生成
			auto& costSprite = stagePtr->AddGameObject<Number>(Vec2(shop->m_numberScale), Vec3(shop->m_enhancementCostPos.at(i)), L"SHOP_NUMBER", cost + 2);
			// 数字スプライト保持用の配列に格納
			shop->m_numbers.push_back(costSprite);



		}

		for (auto t : shop->m_numbers) {
			t.lock()->SetDrawLayer(11);
		}


	}

	// ステート更新時の処理
	void ShopShoppingState::Execute(const shared_ptr<Shop>& shop)
	{
		// Aボタンと左スティック入力の受付、それに伴う処理
		

		// 選択肢の点滅処理


	}

	// ステート終了時の処理
	void ShopShoppingState::Exit(const shared_ptr<Shop>& shop)
	{
		// スプライトの非表示処理
		shop->m_purchaseScreenSprite->SetDrawActive(false);
		shop->m_selectPointSprite->SetDrawActive(false);

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