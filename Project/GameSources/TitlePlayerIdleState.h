/*!
@file TitlePlayerIdleState.h
@brief プレイヤーの待機状態ステート
@author 小澤博貴
*/

#pragma once
#include "TitlePlayerStateMachine.h"

namespace basecross
{
	/*!
	@brief 移動状態のプレイヤーステート
	*/
	class TitlePlayerIdleState : public TitlePlayerState
	{
		/*!
		@brief コンストラクタ
		*/
		TitlePlayerIdleState() {}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayerIdleState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<TitlePlayerIdleState> Instance();

		/*!
		@brief ステート名取得関数
		@return ステートの名前
		*/
		wstring GetStateName() const override;

		/*!
		@brief ステート開始時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Enter(const shared_ptr<TitlePlayer>& player) override;

		/*!
		@brief ステート実行中に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Execute(const shared_ptr<TitlePlayer>& player) override;

		/*!
		@brief ステート終了時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Exit(const shared_ptr<TitlePlayer>& player) override;

		/*!
		@brief Bボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushB(const shared_ptr<TitlePlayer>& player) override;
	};
}