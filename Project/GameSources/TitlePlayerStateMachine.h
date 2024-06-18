/*!
@file TitlePlayerStateMachine.h
@brief プレイヤーの状態ステート
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// 名前用
	class TitlePlayer;

	/*!
	@brief プレイヤー用のステート
	*/
	class TitlePlayerState : public ObjState<TitlePlayer>
	{
	public:

		/*!
		@brief	コンストラクタ
		*/
		TitlePlayerState() {}

		/*!
		@brief	デストラクタ
		*/
		virtual ~TitlePlayerState() {}

		/*!
		@brief ステートに入った時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void Enter(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief Updateの時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void Execute(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief ステートを出る時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void Exit(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief Aボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void OnPushB(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief	ステート名を得る
		@param[in]	ステート名
		@return	なし
		*/
		virtual wstring GetStateName()const { return L""; }
	};

	/*!
	@brief プレイヤー用のステートマシン
	*/
	class TitlePlayerStateMachine : public StateMachine<TitlePlayer>
	{
	public:

		/*!
		@brief コンストラクタ
		@param プレイヤーのポインタ
		*/
		explicit TitlePlayerStateMachine(const shared_ptr<TitlePlayer>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~TitlePlayerStateMachine() {}

		/*!
		@brief ステートを変更する
		@param 設定するステート
		*/
		void SetState(const shared_ptr<TitlePlayerState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<TitlePlayer>>(newState));
		}

		/*!
		@brief	Aボタン入力がされた時
		*/
		void PushB() const
		{
			// プレイヤーとステートの取得
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<TitlePlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// ステートにAボタン入力時の処理を送る
				state->OnPushB(player);
			}
		}
	};
}