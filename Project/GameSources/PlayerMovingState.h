/*!
@file PlayerMovingState.h
@brief プレイヤーの移動状態ステート
@author 小澤博貴
*/

#pragma once
#include "PlayerState.h"

namespace basecross
{
	/*!
	@brief 移動状態のプレイヤーステート
	*/
	class PlayerMovingState : public PlayerState
	{
		/*!
		@brief コンストラクタ
		*/
		PlayerMovingState() {}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~PlayerMovingState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<PlayerMovingState> Instance();

		/*!
		@brief ステート名取得関数
		@return ステートの名前
		*/
		wstring GetStateName() const override;

		/*!
		@brief ステート開始時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Enter(const shared_ptr<Player>& player) override;

		/*!
		@brief ステート実行中に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Execute(const shared_ptr<Player>& player) override;

		/*!
		@brief ステート終了時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Exit(const shared_ptr<Player>& player) override;

		/*!
		@brief Aボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushA(const shared_ptr<Player>& player) override;

		/*!
		@brief Xボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushX(const shared_ptr<Player>& player) override;
	};
}