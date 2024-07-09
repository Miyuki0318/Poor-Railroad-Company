/*!
@file ShopShoppingState.h
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#pragma once
#include "ShopState.h"

namespace basecross
{
	class ShopShoppingState : public ShopState
	{

	public:
		/*!
		@brief	コンストラクタ
		*/
		ShopShoppingState() {
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopShoppingState() {}

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
		static shared_ptr<ShopShoppingState> Instance();
	};
}
