/*!
@file ShopCloseState.cpp
@brief �V���b�v�̖��I����ԃX�e�[�g
@author �����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �X�e�[�g�J�n���̏���
	void ShopCloseState::Enter(const shared_ptr<Shop>& shop)
	{


	}

	// �X�e�[�g�X�V���̏���
	void ShopCloseState::Execute(const shared_ptr<Shop>& shop)
	{


	}

	// �X�e�[�g�I�����̏���
	void ShopCloseState::Exit(const shared_ptr<Shop>& shop)
	{


	}

	// �C���X�^���X����
	shared_ptr<ShopCloseState> ShopCloseState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<ShopCloseState> instance(new ShopCloseState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}
}