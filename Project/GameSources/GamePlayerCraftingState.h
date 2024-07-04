/*!
@file GamePlayerCraftingState.h
@brief プレイヤーのクラフト状態ステート
@author 小澤博貴
*/

#pragma once
#include "GamePlayerStateMachine.h"
#include "GamePlayer.h"

namespace basecross
{
	/*!
	@brief クラフト状態のプレイヤーステート
	*/
	class GamePlayerCraftingState : public GamePlayerState
	{
		// クラフト開始ボタン入力
		enum class eCurrentCraftInput : unsigned char
		{
			None,		// 未入力
			PushStartA,	// Aボタン入力で開始した
			PushStartB,	// Bボタン入力で開始した
			PushStartY,	// Yボタン入力で開始した
		};

		// クラフト開始ボタン
		eCurrentCraftInput m_currentInput;
		eCurrentCraftInput m_pastInput;

		bool m_isStartCraft; // クラフト開始できるかの真偽

		/*!
		@brief コンストラクタ
		*/
		GamePlayerCraftingState() 
		{
			m_isStartCraft = true;
			m_currentInput = eCurrentCraftInput::None;
			m_pastInput = eCurrentCraftInput::None;
		}


	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~GamePlayerCraftingState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<GamePlayerCraftingState> Instance();

		/*!
		@brief ステート名取得関数
		@return ステートの名前
		*/
		wstring GetStateName() const override;

		/*!
		@brief ステート開始時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Enter(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief ステート実行中に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Execute(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief ステート終了時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Exit(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief Aボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushA(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief Bボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushB(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief Xボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushX(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief Yボタン入力がされた時に実行される
		@param プレイヤーのポインタ
		*/
		void OnPushY(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief ボタン入力を行い、QTEを停止させる関数
		*/
		void PushedQTE(const shared_ptr<GamePlayer>& player);

		/*!
		@brief クラフトQTEが終わっているかの確認関数
		*/
		void CheckedCraftQTE(const shared_ptr<GamePlayer>& player);

		/*!
		@brief クラフトQTE停止時に実行する関数
		*/
		void StoppedCraftQTE(const shared_ptr<GamePlayer>& player);
	};
}