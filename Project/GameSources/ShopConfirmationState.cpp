/*!
@file ShopConfirmationState.cpp
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ステート開始時の処理
	void ShopConfirmationState::Enter(const shared_ptr<Shop>& shop)
	{
		shop->m_currentState = Shop::ShopState::Close;

	}

	// ステート更新時の処理
	void ShopConfirmationState::Execute(const shared_ptr<Shop>& shop)
	{


	}

	// ステート終了時の処理
	void ShopConfirmationState::Exit(const shared_ptr<Shop>& shop)
	{


	}

	// インスタンス生成
	shared_ptr<ShopConfirmationState> ShopConfirmationState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<ShopConfirmationState> instance(new ShopConfirmationState);

		// 新しく生成されたthisポインタ
		return instance;
	}
}