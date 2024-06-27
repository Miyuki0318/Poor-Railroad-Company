/*!
@file ShopCloseState.cpp
@brief ショップの未選択状態ステート
@author 小宅碧
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ステート開始時の処理
	void ShopCloseState::Enter(const shared_ptr<Shop>& shop)
	{


	}

	// ステート更新時の処理
	void ShopCloseState::Execute(const shared_ptr<Shop>& shop)
	{


	}

	// ステート終了時の処理
	void ShopCloseState::Exit(const shared_ptr<Shop>& shop)
	{


	}

	// インスタンス生成
	shared_ptr<ShopCloseState> ShopCloseState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<ShopCloseState> instance(new ShopCloseState);

		// 新しく生成されたthisポインタ
		return instance;
	}
}