/*!
@file Shop.h
@brief ショップ機能の実装
@author 小宅碧
*/

#pragma once
#include "TemplateObject.h"
#include "ShopState.h"


namespace basecross {
	// フレンド化するクラス名の定義
	class ShopState;
	class ShopCloseState;
	class ShopShoppingState;
	class ShopConfirmationState;

	// ショップクラス
	class Shop : public TemplateObject
	{
	public:
		// 強化内容定数
		enum Enhancements {
			Backpack, // バックパック
			Status, // ステータス
			Gear, //歯車
			size
		};

		// ステート管理用定数
		enum ShopState {
			Close, // ショップ未選択
			Shopping, // 購入中
			Confirmation, // 購入内容確認中
		};

	private:
		// ステートマシンのフレンド化
		friend ShopState;
		friend ShopCloseState;
		friend ShopShoppingState;
		friend ShopConfirmationState;

		// 現在のステートを保持
		ShopState m_currentState;
		// 選択されている強化内容を保持する
		Enhancements m_currentEnhancements;
		// 各強化内容のレベルを保持
		vector<int> m_playerLv;
		// 各強化の費用を保持
		vector<vector<int>> m_enhancementCost;

		// 数字スプライトのスケール
		Vec2 m_numberScale;
		// 数字スプライト同士の間隔
		Vec3 m_numberMargin;

		// 選択肢スプライトの表示座標
		vector<Vec3> m_selectPointPos;
		// 現在Lvスプライトの表示座標
		vector<Vec3> m_playerLvPos;
		// 強化費用スプライトの表示座標
		vector<Vec3> m_enhancementCostPos;

		// 数値スプライト保持用
		vector<weak_ptr<Number>> m_numbers;
		// 購入画面用スプライト
		shared_ptr<Sprite> m_purchaseScreenSprite;
		// 選択場所表示用スプライト
		shared_ptr<Sprite> m_selectPointSprite;
		// 購入内容確認用スプライト
		shared_ptr<Sprite> m_confirmationScreenSprite;
		// 強化内容「バックパック」用スプライト
		shared_ptr<Sprite> m_backpackSprite;
		// 強化内容「ステータス」用スプライト
		shared_ptr<Sprite> m_statusSprite;
		// 強化内容「ギア」用スプライト
		shared_ptr<Sprite> m_gearSprite;
		// 選択肢「はい」用スプライト
		shared_ptr<Sprite>m_yesSprite;
		// 選択肢「いいえ」用スプライト
		shared_ptr<Sprite>m_noSprite;

		// ショップ用のステートマシンのポインタを所有
		unique_ptr<ShopStateMachine> m_ShopState;


	public:
		Shop(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			TemplateObject(stagePtr)
		{
			// csvから強化費用を取得
			m_enhancementCost = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("ManagingMoney"));

			// 現在のステートをCloseに初期化
			m_currentState = ShopState::Close;

			// 現在の選択内容をBackpackに初期化
			m_currentEnhancements = Enhancements::Status;

			// 数字スプライトのスケールを設定
			m_numberScale = Vec2(80.0f);

			// 
			m_numberMargin = Vec3(m_numberScale * 0.55f);

			// サイズを強化内容の種類分に設定
			m_playerLv.resize(Enhancements::size);
			m_selectPointPos.resize(Enhancements::size);
			m_enhancementCostPos.resize(Enhancements::size);


		}

		// デストラクタ
		virtual ~Shop() {
			// ステートをリセット
			m_ShopState.reset();
		}

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 毎フレーム実行される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief	 ステートを変更する関数
		@param[in]	newState 新しいステートのポインタ
		*/
		void SetState(const shared_ptr<ObjState<Shop>>& newState) {
			m_ShopState->ChangeState(newState);
		};

		/*!
		@brief	 現在のステートを取得する関数
		@return 現在のステートのポインタ
		*/
		ShopState GetState() {
			return m_currentState;
		};


	};
}