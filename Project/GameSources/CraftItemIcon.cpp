/*!
@file CraftItemIcon.cpp
@brief �N���t�g����A�C�e���A�C�R��
@author ���V���M
*/

#include "stdafx.h"
#include "CraftItemIcon.h"

namespace basecross
{
	// �������̏���
	void CraftItemIcon::OnCreate()
	{
		// �p�����̐������̏���
		CraftUI::OnCreate();

		// ���C���[�ݒ�
		SetDrawLayer(7);
	}

	// �X�V����
	void CraftItemIcon::OnUpdate()
	{
		// �p�����̍X�V����
		CraftUI::OnUpdate();

		// �F���N���t�g�ł��邩�Őݒ�
		SetDiffuseColor(m_craftPosshible ? COL_WHITE : COL_GRAY);
	}
}