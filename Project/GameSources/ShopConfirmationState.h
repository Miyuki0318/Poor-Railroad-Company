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
		// 強化するかの可否
		bool m_isEnhancements;
		// 強化内容の現在のレベル
		int m_currentLv;
		// 強化内容の現在のレベル
		int m_cost;

	public:
		/*!
		@brief	コンストラクタ
		*/
		ShopConfirmationState() {
			// 変数の初期化
			m_isEnhancements = false;
			m_currentLv = 0;
			m_cost = 0;
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopConfirmationState() {}

		/*!
		@brief ステートに入った時に実行される
		@param プレイヤーのポインタ
		*/
		void Enter(const shared_ptr<Shop>& shop) override;

		/*!
		@brief Updateの時に実行される
		@param プレイヤーのポインタ
		*/
		void Execute(const shared_ptr<Shop>& shop) override;

		/*!
		@brief ステートを出る時に実行される
		@param プレイヤーのポインタ
		*/
		void Exit(const shared_ptr<Shop>& shop) override;

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<ShopConfirmationState> Instance();
	};
}
