/*!
@file ShopCloseState.h
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#pragma once
#include "ShopState.h"

namespace basecross
{
	class ShopCloseState : public ShopState
	{
	public:
		/*!
		@brief	コンストラクタ
		*/
		ShopCloseState() {	}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopCloseState() {}

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
		static shared_ptr<ShopCloseState> Instance();
	};
}
