/*!
@file PlayerState.h
@brief プレイヤーの状態ステート
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// 名前用
	class Player;

	/*!
	@brief プレイヤー用のステート
	*/
	class PlayerState : public ObjState<Player>
	{
	public:

		/*!
		@brief	コンストラクタ
		*/
		PlayerState() {}

		/*!
		@brief	デストラクタ
		*/
		virtual ~PlayerState() {}

		/*!
		@brief ステートに入った時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void Enter(const shared_ptr<Player>& player) = 0;

		/*!
		@brief Updateの時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void Execute(const shared_ptr<Player>& player) = 0;

		/*!
		@brief ステートを出る時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void Exit(const shared_ptr<Player>& player) = 0;

		/*!
		@brief Aボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void OnPushA(const shared_ptr<Player>& player) = 0;

		/*!
		@brief Xボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		virtual void OnPushX(const shared_ptr<Player>& player) = 0;

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
	class PlayerStateMachine : public StateMachine<Player>
	{
	public:

		/*!
		@brief コンストラクタ
		@param プレイヤーのポインタ
		*/
		explicit PlayerStateMachine(const shared_ptr<Player>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	デストラクタ
		*/
		virtual ~PlayerStateMachine() {}
		
		/*!
		@brief ステートを変更する
		@param 設定するステート
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<Player>>(newState));
		}

		/*!
		@brief	Aボタン入力がされた時
		*/
		void PushA() const
		{
			// プレイヤーとステートの取得
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<PlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// ステートにAボタン入力時の処理を送る
				state->OnPushA(player);
			}
		}

		/*!
		@brief	Xボタン入力がされた時
		*/
		void PushX() const
		{
			// プレイヤーとステートの取得
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<PlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// ステートにAボタン入力時の処理を送る
				state->OnPushX(player);
			}
		}
	};
}