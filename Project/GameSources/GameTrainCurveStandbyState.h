/*!
@file GameTrainCurveStandbyState.h
@brief ゲーム中の列車のカーブ開始時のステート
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "GameTrain.h"

namespace basecross
{
	/*!
	@brief ゲーム中のカーブレール開始時のステート
	*/
	class GameTrainCurveStandbyState : public ObjState<GameTrain>
	{
		/*!
		@brief コンストラクタ
		*/
		GameTrainCurveStandbyState() {}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~GameTrainCurveStandbyState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<GameTrainCurveStandbyState> Instance();

		/*!
		@brief ステート名取得関数
		@return ステートの名前
		*/
		wstring GetStateName() const override;

		/*!
		@brief ステート開始時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Enter(const shared_ptr<GameTrain>& train) override;

		/*!
		@brief ステート実行中に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Execute(const shared_ptr<GameTrain>& train) override;

		/*!
		@brief ステート終了時に呼び出される関数
		@param ステートマシン参照オブジェクトのポインタ
		*/
		void Exit(const shared_ptr<GameTrain>& train) override;
	};
}