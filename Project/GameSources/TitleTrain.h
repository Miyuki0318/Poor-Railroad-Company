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
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

	public:
		TitleTrain(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			Train(stagePtr, position)
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