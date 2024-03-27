/*!
@file PlayerMiningState.h
@brief プレイヤーの採掘状態ステート
@author 小澤博貴
*/

#pragma once
#include "Player.h"

namespace basecross
{
	/*!
	@brief 採掘状態のプレイヤーステート
	*/
	class PlayerMiningState : public ObjState<Player>
	{
		/*!
		@brief コンストラクタ
		*/
		PlayerMiningState() {}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~PlayerMiningState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<PlayerMiningState> Instance();

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
	};
}