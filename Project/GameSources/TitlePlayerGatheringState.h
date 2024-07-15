/*!
@file TitlePlayerGatheringState.h
@brief プレイヤーの採取状態ステート
@author 小澤博貴
*/

#pragma once
#include "TitlePlayerStateMachine.h"
#include "TitlePlayer.h"

namespace basecross
{
	/*!
	@brief 採取状態のプレイヤーステート
	*/
	class TitlePlayerGatheringState : public TitlePlayerState
	{
		/*!
		@brief コンストラクタ
		*/
		TitlePlayerGatheringState()
		{
		}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayerGatheringState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<TitlePlayerGatheringState> Instance();

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