/*!
@file Shop.h
@brief ショップ機能の実装
@author 小宅碧
*/

#pragma once
#include "TemplateObject.h"
#include "ShopState.h"
#include "Number.h"
#include "CSVLoader.h"


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
		// ステート管理用定数
		enum eShopState {
			Close, // ショップ未選択
			Shopping, // 購入内容選択中
			Confirmation, // 購入内容確認中
		};

	private:
		// 強化内容定数
		enum eEnhancements {
			LimitChoices, // 資材所持上限
			Status, // 採掘/移動速度強化
			Gear, //歯車
			EnhancementsSize,
		};

		// 確認用定数
		enum eConfirmState {
			Yes, // はい
			No, // いいえ
			ConfirmSize,
		};

		// 操作種類用定数
		enum eOperationType {
			Controller, // コントローラー用
			Keyboard, // キーボード用
			OperationSize,
		};

		// ステートマシンのフレンド化
		friend ShopState;
		friend ShopCloseState;
		friend ShopShoppingState;
		friend ShopConfirmationState;

		// 現在のステートを保持
		eShopState m_currentState;
		// 選択されている強化内容を保持する
		eEnhancements m_currentEnhancements;
		// 各強化内容のレベルを保持
		vector<int> m_playerLv;
		// 各強化内容の費用を保持
		vector<vector<int>> m_enhancementsCost;
		// 各強化内容が強化可能かを保持
		vector<bool>  m_canEnhancements;

		// 強化費用の最大桁数を保持
		int m_maxDigit;
		// 選択場所表示スプライトの透明度の上限値
		const float m_transparencyLimit;
		// 選択肢スプライトのスケールを拡大する最大倍率
		const float m_spriteSizeRange;

		// 数字スプライトのスケール
		const Vec2 m_numberScale;
		// 数字スプライト同士の間隔
		const Vec3 m_numberMargin;

		// 選択場所表示スプライトの表示座標
		const vector<Vec3> m_selectPointSpritePos;
		// 現在Lvスプライトの表示座標
		const vector<Vec3> m_playerLvSpritePos;
		// 強化確認スプライトの表示座標
		const vector<Vec3> m_confirmSpritePos;
		// 強化確認スプライトの初期スケール
		const Vec2 m_confirmSpriteScale;
		// 強化内容スプライトのスケール
		const Vec2 m_enhancementsContentsScale;
		// 強化内容スプライトの表示座標
		const Vec3 m_enhancementsContentsPos;
		// 選択肢スプライトのスケール
		const Vec2 m_selectPointScale;
		// スクリーンスプライトの表示座標
		const Vec3 m_screenSpritePos;
		// 次のレベルの表示座標
		const Vec3 m_nextLevelSpritePos;
		// アイコンの表示座標
		const Vec3 m_iconPos;
		// アイコンのスケール
		const Vec2 m_iconScale;
		// 強化費用スプライトの表示座標
		vector<Vec3> m_enhancementsCostSpritePos;
		// 次の強化費用の表示座標
		Vec3 m_nextCostSpritePos;

		// 強化費用の数値スプライト
		vector<vector<weak_ptr<Number>>> m_enhancementsCostNumSprite;
		// 各強化Lv用の数値スプライト
		vector<weak_ptr<Number>> m_playerLvNumSprite;
		// 購入画面用スプライト
		weak_ptr<Sprite> m_purchaseScreenSprite;
		// 選択場所表示用スプライト
		weak_ptr<Sprite> m_selectPointSprite;
		// 選択場所強化不可用スプライト(選択箇所をグレーのスプライトで覆う)
		vector<weak_ptr<Sprite>> m_notSelectableSprite;
		// マウス・コントローラー切替時の表示アイコン用スプライト
		vector<weak_ptr<Sprite>> m_iconSprite;
		// 購入内容確認画面用スプライト
		weak_ptr<Sprite> m_confirmationScreenSprite;
		// 選択された強化内容の次のLv用の数値スプライト
		weak_ptr<Number> m_enhancementsNextLvSprite;
		// 選択された強化内容の強化費用の数値スプライト
		vector<weak_ptr<Number>> m_enhancementsNextCostSprite;
		// 強化内容「資材所持上限上昇」用スプライト
		weak_ptr<Sprite> m_LimitChoicesSprite;
		// 強化内容「ステータス」用スプライト
		weak_ptr<Sprite> m_statusSprite;
		// 強化内容「踏切素材初期所持数」用スプライト
		weak_ptr<Sprite> m_gearSprite;
		// 選択肢用スプライト
		map<eConfirmState, weak_ptr<Sprite>> m_confirmSprite;

		// ショップ用のステートマシンのポインタを所有
		unique_ptr<ShopStateMachine> m_ShopState;

	public:
		Shop(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			// 継承元のTemplateObjectにステージのポインタを渡す
			TemplateObject(stagePtr),
			// スプライトの透明度の上限値を設定
			m_transparencyLimit(0.5f),
			// スプライトを拡大する最大倍率を設定
			m_spriteSizeRange(1.25f),
			// 数字スプライトのスケールを設定
			m_numberScale(Vec2(80.0f)),
			// 数字スプライト同士の間隔を設定
			m_numberMargin(Vec3(m_numberScale.x * 0.8f, 0.0f, 0.0f)),
			// 選択肢の標準のスケール
			m_confirmSpriteScale(Vec2(360.0f, 90.0f)),
			// 強化内容のスケール
			m_enhancementsContentsScale(Vec2(875.0f, 125.0f)),
			// 選択肢スプライトのスケール
			m_selectPointScale(Vec2(1170.0f, 238.0)),
			// スクリーンスプライトの表示座標を設定
			m_screenSpritePos(Vec3(-400.0f, 0.0f, 0.0f)),
			// 強化費用の表示座標を設定
			m_nextCostSpritePos(Vec3(120.0f, -90.0f, 0.0f)),
			// 次のレベルの表示座標を設定
			m_nextLevelSpritePos(Vec3(-350.0f, -90.0f, 0.0f)),
			// アイコンの表示座標の設定
			m_iconPos(Vec3(100.0f, 380.0f, 0.0f)),
			// アイコンのスケールの設定
			m_iconScale(Vec2(100.0f, 100.0f)),
			// 強化内容の標準座標を設定
			m_enhancementsContentsPos(Vec3(-170.0f, 100.0f, 0.0f)),
			// 選択肢の表示位置を設定
			m_selectPointSpritePos{
				Vec3(-179.0f, 181.0f, 0.0f),
				Vec3(-179.0f, -66.0f, 0.0f),
				Vec3(-179.0f, -320.0f, 0.0f)
			},
			// 現在Lvの表示位置を設定
			m_playerLvSpritePos{
				Vec3(-370.0f, 117.0f, 0.0f),
				Vec3(-370.0f, -128.0f, 0.0f),
				Vec3(-370.0f, -383.0f, 0.0f)
			},
			// 選択肢スプライトの表示位置を設定
			m_confirmSpritePos{
				Vec3(-400.0f, -300.0f, 0.0f), // 選択肢「はい」
				Vec3(100.0f, -300.0f, 0.0f) // 選択肢「いいえ」
			},
			// 強化費用の表示位置を設定
			m_enhancementsCostSpritePos{
				Vec3(100.0f, 117.0f, 0.0f),
				Vec3(100.0f, -128.0f, 0.0f),
				Vec3(100.0f, -383.0f, 0.0f)
			}


		{
			// csvから強化費用を取得
			m_enhancementsCost = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("ManagingMoney"));

			// 変数を初期化
			m_currentState = eShopState::Close;
			m_currentEnhancements = eEnhancements::Status;
			m_maxDigit = 0;

			// 配列のサイズを設定
			m_playerLv.resize(eEnhancements::EnhancementsSize);
			m_canEnhancements.resize(eEnhancements::EnhancementsSize);
			m_notSelectableSprite.resize(eEnhancements::EnhancementsSize);
			m_enhancementsCostNumSprite.resize(eEnhancements::EnhancementsSize);
			m_playerLvNumSprite.resize(eEnhancements::EnhancementsSize);

			m_iconSprite.resize(eOperationType::OperationSize);
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
		@brief	 強化費用の最大桁数を求める関数
		*/
		int DemandDigit();

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
		eShopState GetState() const {
			return m_currentState;
		};
	};
}