/*!
@file TitlePlayerMovingState.h
@brief プレイヤーの移動状態ステート
@author 小澤博貴
*/

#pragma once
#include "TitlePlayerStateMachine.h"
#include "TitlePlayer.h"

namespace basecross
{
	/*!
	@brief 移動状態のプレイヤーステート
	*/
	class TitlePlayerMovingState : public TitlePlayerState
	{
		/*!
		@brief コンストラクタ
		*/
		TitlePlayerMovingState() {}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayerMovingState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<TitlePlayerMovingState> Instance();

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

		/*!
		@brief 移動更新
		@param プレイヤーのポインタ
		*/
		void UpdateMoving(const shared_ptr<TitlePlayer>& player);
	};
}