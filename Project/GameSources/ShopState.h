/*!
@file eShopState.h
@brief ショップの状態ステート
@author 小宅碧
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// 名前用
	class Shop;

	/*!
	@brief ショップ用のステート
	*/
	class ShopState : public ObjState<Shop>
	{
	protected:
		// Lスティック入力の下限値
		float m_LStickLowerLimit;
		// Lスティック入力量と下限値を比較した値をLスティックの入力の有無として保持
		bool m_isInputLStick;
		// 強化内容が変更されてからの経過時間
		float m_totalTime;

	public:

		/*!
		@brief	コンストラクタ
		*/
		ShopState() {
			// Lスティック入力の下限値を設定
			m_LStickLowerLimit = 0.3f;

			// 変数の初期化
			m_isInputLStick = false;
			m_totalTime = 0.0f;
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopState() {}

		/*!
		@brief ステートに入った時に実行される
		@param ショップのポインタ
		*/
		virtual void Enter(const shared_ptr<Shop>& shop) = 0;

		/*!
		@brief Updateの時に実行される
		@param ショップのポインタ
		*/
		virtual void Execute(const shared_ptr<Shop>& shop) = 0;

		/*!
		@brief ステートを出る時に実行される
		@param ショップのポインタ
		*/
		virtual void Exit(const shared_ptr<Shop>& shop) = 0;
	};

	/*!
	@brief ショップ用のステートマシン
	*/
	class ShopStateMachine : public StateMachine<Shop>
	{
	public:

		/*!
		@brief コンストラクタ
		@param ショップのポインタ
		*/
		explicit ShopStateMachine(const shared_ptr<Shop>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~ShopStateMachine() {}

		/*!
		@brief ステートを変更する
		@param 設定するステート
		*/
		void SetState(const shared_ptr<ShopState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<Shop>>(newState));
		}
	};
}