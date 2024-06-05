/*!
@file CraftWindow.cpp
@brief �N���t�g�E�B���h�E
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void CraftWindow::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// �`�惂�[�h��ύX
		m_ptrDraw->SetRasterizerState(RasterizerState::DoubleDraw);

		// �����x��ύX
		SetDiffuseColor(Col4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// ���t���[���X�V����
	void CraftWindow::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		CraftUI::OnUpdate();
	}
}