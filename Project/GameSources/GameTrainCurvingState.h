/*!
@file GameTrainCurvingState.h
@brief ゲーム中の列車のカーブ中のステート
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "GameTrain.h"

namespace basecross
{
	/*!
	@brief ゲーム中のカーブレール走行中のステート
	*/
	class GameTrainCurvingState : public ObjState<GameTrain>
	{
		float m_pastRotY;	// 過去の回転Y軸
		CurvePoints m_curvePoints; // カーブに使う座標

		/*!
		@brief コンストラクタ
		*/
		GameTrainCurvingState() 
		{
			m_pastRotY = 0.0f;
			m_curvePoints = {};
		}

	public:

		/*!
		@brief デストラクタ
		*/
		virtual ~GameTrainCurvingState() {}

		/*!
		@brief インスタンス関数
		@return 新しく生成されたthisポインタ
		*/
		static shared_ptr<GameTrainCurvingState> Instance();

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