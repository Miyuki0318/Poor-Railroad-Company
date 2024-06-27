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
			Gear //歯車
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

		// ショップの現在のステートを保存
		ShopState m_currentState;

		// ショップのステートマシンのポインタを所有
		unique_ptr<ShopStateMachine> m_ShopState;


	public:
		Shop(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			TemplateObject(stagePtr)
		{
			// 現在のステートをCloseに初期化
			m_currentState = Close;
		}

		// デストラクタ
		virtual ~Shop() {
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