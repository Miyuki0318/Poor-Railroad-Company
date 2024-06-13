/*!
@file GameTrain.h
@brief 列車オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"
#include "RailManager.h"
#include "SmokeEffect.h"

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
		float m_acsel; // 加速度
		string m_trainPos;

		const map<string, RailData>* m_railDataMap;
		
		set<wstring> m_whistleSEKey;
		
		weak_ptr<RailManager> m_railManager;
		weak_ptr<SmokeEffect> m_smokeEffect;
		weak_ptr<SoundItem> m_whistleSE;

		// ステートマシン
		unique_ptr<StateMachine<GameTrain>> m_trainState;

		// ステートクラスをフレンド化
		friend GameTrainStraightState;
		friend GameTrainCurveStandbyState;
		friend GameTrainCurvingState;
		friend GameTrainCurveExitState;

	public:
		GameTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr),
			m_railDataMap(nullptr)
		{
			m_acsel = 0.0f;

			m_whistleSEKey.insert(L"WHISTLE_SE");
			m_whistleSEKey.insert(L"SHORT_WHISTLE_SE");
		}

		GameTrain(const shared_ptr<Stage>& stagePtr,
			const Vec3& startPosition
		) :
			Train(stagePtr, startPosition),
			m_railDataMap(nullptr)
		{
			m_acsel = 0.0f;

			m_whistleSEKey.insert(L"WHISTLE_SE");
			m_whistleSEKey.insert(L"SHORT_WHISTLE_SE");
		}

		~GameTrain() 
		{
			m_trainState.reset();
		}

		void OnCreate() override;

		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		void ResetTrain();

		void WhistleSmokeEffect();

		int GetNextedRailCount();

		void DeRailWhistleSE();

		/// <summary>
		/// 状態ごとの処理
		/// </summary>
		/// <param name="state">状態</param>
		void StateProcess(State state);

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

		string GetRailPosLine() const
		{
			return m_railPos;
		}

	private:

		const map<string, RailData>& GetRailDataMap() const;
	};
}