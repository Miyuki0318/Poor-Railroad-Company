/*!
@file CraftWindow.cpp
@brief �N���t�g�E�B���h�E
@author ���V���M
*/

#include "stdafx.h"
#include "CraftWindow.h"

namespace basecross
{
	// �������̏���
	void CraftWindow::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// ���C���[��ݒ�
		SetDrawLayer(5);

		// �`�惂�[�h��ύX
		m_ptrDraw->SetRasterizerState(RasterizerState::DoubleDraw);
	}

	// ���t���[���X�V����
	void CraftWindow::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		CraftUI::OnUpdate();
	}
}