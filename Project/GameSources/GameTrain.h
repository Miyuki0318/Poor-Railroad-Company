/*!
@file GameTrain.h
@brief ��ԃI�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"
#include "RailManager.h"

namespace basecross {

	// �e�X�e�[�g�N���X
	class GameTrainStraightState;
	class GameTrainCurveStandbyState;
	class GameTrainCurvingState;
	class GameTrainCurveExitState;

	// �J�[�u�Ɏg�����W�Q
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
		float m_acsel; // �����x

		const map<string, RailData>* m_railDataMap;
		weak_ptr<RailManager> m_railManager;

		// �X�e�[�g�}�V��
		unique_ptr<StateMachine<GameTrain>> m_trainState;

		// �X�e�[�g�N���X���t�����h��
		friend GameTrainStraightState;
		friend GameTrainCurveStandbyState;
		friend GameTrainCurvingState;
		friend GameTrainCurveExitState;

	public:
		GameTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}

		GameTrain(const shared_ptr<Stage>& stagePtr,
			const Vec3& startPosition
		) :
			Train(stagePtr, startPosition)
		{
		}

		~GameTrain() 
		{
			m_trainState.reset();
		}

		void OnCreate() override;

		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		const map<string, RailData>& GetRailDataMap() const;

		/// <summary>
		/// ��Ԃ��Ƃ̏���
		/// </summary>
		/// <param name="state">���</param>
		void StateProcess(State state);

		/// <summary>
		/// ���̃��[�����������鏈��
		/// </summary>
		/// <returns>�����������ǂ���</returns>
		bool SearchNextRail() override;

		bool NextRailSettings(const map<string, RailData>& railMap, eRailAngle nextAngle);

		/// <summary>
		/// �S�[���ɒ��������m�F���鏈��
		/// </summary>
		/// <returns>���������ǂ���</returns>
		bool CheckGoalRail();
	};
}