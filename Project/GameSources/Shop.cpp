/*!
@file Shop.cpp
@brief �V���b�v�@�\�̎���
@author �����
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Shop::OnCreate()
	{
		// �X�e�[�g�}�V���̏�����
		m_ShopState.reset(new ShopStateMachine(GetThis<Shop>()));
		// �ŏ��̃X�e�[�g��ݒ�
		m_ShopState->SetState(ShopCloseState::Instance());



	}

	void Shop::OnUpdate()
	{
		// �X�e�[�g�}�V���̍X�V�����𑗂�
		m_ShopState->Update();
	}
}