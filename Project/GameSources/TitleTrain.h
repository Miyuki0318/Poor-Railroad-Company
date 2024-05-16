/*!
@file TitleTrain.h
@brief ��ԃI�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class TitleTrain : public Train
	{
		enum class State {
			None, // �ҋ@
			GameStart, // �Q�[���X�^�[�g
		};

		// ���݂̏��
		State m_state = State::None;

	public:
		TitleTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}
		~TitleTrain() {}

		void OnUpdate() override;

		/// <summary>
		/// ��Ԃ��Ƃ̏���
		/// </summary>
		/// <param name="state">���</param>
		void StateProcess(State state);

		/// <summary>
		/// �Q�[���X�^�[�g���̏���
		/// </summary>
		void GameStartProcess();
	};

}