/*!
@file GameTrain.h
@brief ��ԃI�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class GameTrain : public Train
	{
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

		~GameTrain() {}

		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& gameObject) override;

		/// <summary>
		/// ��Ԃ��Ƃ̏���
		/// </summary>
		/// <param name="state">���</param>
		void StateProcess(State state);

		/// <summary>
		/// ���[����ɋ���Ƃ��̏���
		/// </summary>
		void OnRailProcess();

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