/*!
@file TitlePlayer.h
@brief タイトル用プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "Player.h"
#include "TitlePlayerStateMachine.h"

namespace basecross
{
	// プレイヤーの状態ステートクラス(名前のみ宣言)
	class TitlePlayerIdleState;			// 待機状態
	class TitlePlayerMovingState;		// 移動状態
	class TitlePlayerPauseState;		// 停止状態
	class TitlePlayerGatheringState;	// 採掘状態

	/*!
	@brief タイトル用プレイヤー
	*/
	class TitlePlayer : public Player
	{
		// ステートマシン
		unique_ptr<TitlePlayerStateMachine> m_playerState;

		friend TitlePlayerIdleState;
		friend TitlePlayerMovingState;
		friend TitlePlayerPauseState;
		friend TitlePlayerGatheringState;

	public:
		
		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayer() 
		{
			m_playerState.reset();
		}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief プレイヤーに付加する機能生成関数
		*/
		void CreatePlayerFeatures() override;

		/*!
		@brief インディケーターへの取得と呼び出し関数
		*/
		void IndicatorOrder() override;

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove() override;

		/*!
		@brief State変更関数
		@param 新しいステートのポインタ
		*/
		void SetState(const shared_ptr<TitlePlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}
	};
}