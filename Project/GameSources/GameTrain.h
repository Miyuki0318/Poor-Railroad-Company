/*!
@file GameTrain.h
@brief 列車オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {

	// 各ステートクラス
	class GameTrainStraightState;
	class GameTrainCurveStandbyState;
	class GameTrainCurvingState;
	class GameTrainCurveExitState;

	// カーブに使う座標群
	struct CurvePoints
	{
		Vec3 pointA;
		Vec3 pointB;
		Vec3 pointC;

		CurvePoints() {}

		CurvePoints(const Vec3& pA, const Vec3& pB, const Vec3& pC) :
			pointA(pA),
			pointB(pB),
			pointC(pC)
		{
		}
	};

	class GameTrain : public Train
	{
		CurvePoints m_curvePoints;	// カーブに使う座標

		// ステートマシン
		unique_ptr<StateMachine<GameTrain>> m_trainState;

		// ステートクラスをフレンド化
		friend GameTrainStraightState;
		friend GameTrainCurveStandbyState;
		friend GameTrainCurvingState;
		friend GameTrainCurveExitState;

	public:
		GameTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
			m_curvePoints = {};
		}

		GameTrain(const shared_ptr<Stage>& stagePtr,
			const Vec3& startPosition
		) :
			Train(stagePtr, startPosition)
		{
			m_curvePoints = {};
		}

		~GameTrain() 
		{
			m_trainState.reset();
		}

		void OnCreate() override;

		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		/// <summary>
		/// 状態ごとの処理
		/// </summary>
		/// <param name="state">状態</param>
		void StateProcess(State state);

		/// <summary>
		/// レール上に居るときの処理
		/// </summary>
		void OnRailProcess();

		/// <summary>
		/// 次のレールを検索する処理
		/// </summary>
		/// <returns>見つかったかどうか</returns>
		bool SearchNextRail() override;

		bool NextRailSettings(const map<string, RailData>& railMap, eRailAngle nextAngle);

		/// <summary>
		/// ゴールに着いたか確認する処理
		/// </summary>
		/// <returns>着いたかどうか</returns>
		bool CheckGoalRail();
	};
}