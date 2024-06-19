/*!
@file TitlePlayerPauseState.h
@brief プレイヤーの停止状態ステート
@author 小澤博貴
*/

#pragma once
#include "TitlePlayerStateMachine.h"

namespace basecross
{
	/*!
	@brief 移動状態のプレイヤーステート
	*/
	class TitlePlayerPauseState : public TitlePlayerState
	{
		Vec3 m_trainBackPosition; // 列車の後ろの位置

		/*!
		@brief コンストラクタ
		*/
		TitlePlayerPauseState() {}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayerPauseState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<TitlePlayerPauseState> Instance();

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