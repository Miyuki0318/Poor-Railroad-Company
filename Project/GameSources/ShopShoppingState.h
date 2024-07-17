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
	private:
		// 強化内容の現在の費用
		vector<int> m_cost;
		// 強化費用の桁数
		vector<int> m_digit;

	public:
		/*!
		@brief	コンストラクタ
		*/
		ShopShoppingState() {
			m_cost.resize(Shop::eEnhancements::EnhancementsSize);
			m_digit.resize(Shop::eEnhancements::EnhancementsSize);
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopShoppingState() {}

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
		static shared_ptr<ShopShoppingState> Instance();

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
