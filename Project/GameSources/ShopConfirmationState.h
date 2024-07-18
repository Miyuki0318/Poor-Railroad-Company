/*!
@file ShopConfirmationState.h
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#pragma once
#include "ShopState.h"

namespace basecross
{
	class ShopConfirmationState : public ShopState
	{
	private:
		// 選択先
		Shop::eConfirmState m_confirmation;
		// 強化内容の現在のレベル
		int m_currentLv;
		// 強化内容の現在の費用
		int m_cost;
		// 強化費用の桁数
		int m_digit;

	public:
		/*!
		@brief	コンストラクタ
		*/
		ShopConfirmationState() {
			// 変数の初期化
			m_confirmation = Shop::eConfirmState::No;
			m_currentLv = 0;
			m_cost = 0;
			m_digit = 0;
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopConfirmationState() {}

		/*!
		@brief ステートに入った時に実行される
		@param ショップのポインタ
		*/
		void Enter(const shared_ptr<Shop>& shop) override;

		/*!
		@brief Updateの時に実行される
		@param ショップのポインタ
		*/
		void Execute(const shared_ptr<Shop>& shop) override;

		/*!
		@brief ステートを出る時に実行される
		@param ショップのポインタ
		*/
		void Exit(const shared_ptr<Shop>& shop) override;

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<ShopConfirmationState> Instance();

		/*!
		@brief コントローラー用操作関数
		@param
		*/
		void  ControllerSelect(const shared_ptr<Shop>& shop);

		/*!
		@brief コントローラー用操作関数
		*/
		void  MouseSelect(const shared_ptr<Shop>& shop);

	};
}
