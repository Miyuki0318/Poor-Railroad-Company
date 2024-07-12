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
		// 強化内容定数
		enum eEnhancements {
			LimitChoices, // 資材所持上限
			Status, // 採掘/移動速度強化
			Gear, //歯車
			size
		};

		// ステート管理用定数
		enum eShopState {
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
		float m_transparencyLimit;
		// 選択肢スプライトのスケールを拡大する最大倍率
		float m_spriteSizeRange;

		// 数字スプライトのスケール
		Vec2 m_numberScale;
		// 数字スプライト同士の間隔
		Vec3 m_numberMargin;

		// 選択場所表示スプライトの表示座標
		vector<Vec3> m_selectPointSpritePos;
		// 現在Lvスプライトの表示座標
		vector<Vec3> m_playerLvSpritePos;
		// 強化費用スプライトの表示座標
		vector<Vec3> m_enhancementsCostSpritePos;
		// 強化確認スプライトの表示座標
		vector<Vec3> m_confirmSpritePos;
		// 強化確認スプライトの初期スケール
		const Vec2 m_confirmSpritedefScale;
		// 強化内容スプライトのスケール
		const Vec2 m_enhancementsContentsScale;
		// 強化内容スプライトの座標
		const Vec3 m_enhancementsContentsPosition;
		// 選択肢スプライトのスケール
		const Vec2 m_selectPointDefScale;
		// スクリーンスプライトの表示座標
		const Vec3 m_screenDefPosition;
		// YES選択肢スプライトの表示座標
		const Vec3 m_yesSpritePosition;
		// NO選択肢スプライトの表示座標
		const Vec3 m_noSpritePosition;
		// 強化費用の表示座標
		Vec3 m_nextCostSpritePos;
		// 次のレベルの表示座標
		Vec3 m_nextLevelSpritePos;

		// 強化費用の数値スプライト
		vector<vector<weak_ptr<Number>>> m_enhancementsCostNumSprite;
		// 各強化Lv用の数値スプライト
		vector<weak_ptr<Number>> m_playerLvNumSprite;
		// 購入画面用スプライト
		weak_ptr<Sprite> m_purchaseScreenSprite;
		// 選択場所表示用スプライト
		weak_ptr<Sprite> m_selectPointSprite;
		// 選択場所強化不可用スプライト
		vector<weak_ptr<Sprite>> m_notSelectableSprite;
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
		// 選択肢「はい」用スプライト
		weak_ptr<Sprite>m_yesSprite;
		// 選択肢「いいえ」用スプライト
		weak_ptr<Sprite>m_noSprite;

		// ショップ用のステートマシンのポインタを所有
		unique_ptr<ShopStateMachine> m_ShopState;


	public:
		Shop(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			TemplateObject(stagePtr),
			m_confirmSpritedefScale(Vec2(360.0f, 90.0f)),	// 選択肢の標準のスケール
			m_enhancementsContentsScale(Vec2(875.0f, 125.0f)),	// 強化内容のスケール
			m_selectPointDefScale(Vec2(1170.0f, 238.0)),		// 選択肢スプライトのスケール
			m_screenDefPosition(Vec3(-400.0f, 0.0f, 0.0f)),		// スクリーンスプライトの表示座標を設定
			m_nextCostSpritePos(Vec3(180.0f, -90.0f, 0.0f)),	// 強化費用の表示座標を設定
			m_nextLevelSpritePos(Vec3(-350.0f, -90.0f, 0.0f)),	// 次のレベルの表示座標を設定
			m_yesSpritePosition(Vec3(-400.0f, -270.0f, 0.0f)),	// 「はい」スプライトの表示座標
			m_noSpritePosition(Vec3(80.0f, -270.0f, 0.0f)),		// 「いいえ」スプライトの表示座標
			m_enhancementsContentsPosition(Vec3(-170.0f, 100.0f, 0.0f))	// 強化内容の標準座標を設定
		{
			// csvから強化費用を取得
			m_enhancementsCost = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("ManagingMoney"));

			// 変数を初期化
			m_currentState = eShopState::Close;
			m_currentEnhancements = eEnhancements::Status;
			m_maxDigit = 0;
			m_transparencyLimit = 0.5f;
			m_spriteSizeRange = 1.25f;

			// 数字スプライトのスケールを設定
			m_numberScale = Vec2(80.0f);

			// 数字スプライト同士の間隔を設定
			m_numberMargin = Vec3(m_numberScale.x * 0.8f, 0.0f, 0.0f);

			// サイズを強化内容の種類分に設定
			m_playerLv.resize(eEnhancements::size);
			m_canEnhancements.resize(eEnhancements::size);
			m_notSelectableSprite.resize(eEnhancements::size);
			m_enhancementsCostNumSprite.resize(eEnhancements::size);
			m_playerLvNumSprite.resize(eEnhancements::size);

			// 選択肢の表示位置を設定
			m_selectPointSpritePos = {
				Vec3(-179.0f, 181.0f, 0.0f),
				Vec3(-179.0f, -66.0f, 0.0f),
				Vec3(-179.0f, -320.0f, 0.0f)
			};

			// 現在Lvの表示位置を設定
			m_playerLvSpritePos = {
				Vec3(-370.0f, 117.0f, 0.0f),
				Vec3(-370.0f, -128.0f, 0.0f),
				Vec3(-370.0f, -383.0f, 0.0f)
			};

			// 強化費用の表示位置を設定
			m_enhancementsCostSpritePos = {
				Vec3(400.0f, 117.0f, 0.0f),
				Vec3(400.0f, -128.0f, 0.0f),
				Vec3(400.0f, -383.0f, 0.0f)
			};

			// 選択肢スプライトの表示位置を設定
			m_confirmSpritePos = {
				Vec3(-550.0f, -300.0f, 0.0f), // 選択肢「はい」
				Vec3(-250.0f, -300.0f, 0.0f) // 選択肢「いいえ」
			};

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