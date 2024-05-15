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
		enum class State {
			Onrail, // ���[���ɏ���Ă���
			Derail, // �E��
			Arrival // �w����
		};
		State m_state = State::Onrail;

	public:
		GameTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
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

		/// <summary>
		/// �S�[���ɒ��������m�F���鏈��
		/// </summary>
		/// <returns>���������ǂ���</returns>
		bool CheckGoalRail();

	};
}