/*!
@file PlayerCraftingState.cpp
@brief �v���C���[�̃N���t�g��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerCraftingState> PlayerCraftingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerCraftingState> instance(new PlayerCraftingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}


	// �X�e�[�g���擾
	wstring PlayerCraftingState::GetStateName() const
	{
		return L"Crafting";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerCraftingState::Enter(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�X�V���̏���
	void PlayerCraftingState::Execute(const shared_ptr<Player>& player)
	{
		// �N���t�g��Ԃ��������ꂽ�̂ňړ��X�e�[�g�ɑJ��
		if (!player->GetStatus(ePlayerStatus::IsCrafting))
		{
			player->ChangeState(PlayerMovingState::Instance());
		}

		// �N���t�gQTE��ԂȂ�QTE���I��������ǂ����̊m�F�����𑗂�
		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			player->CheckedCraftQTE();
		}

		// A�{�^�����͂�����΃N���t�g����A�{�^�����͏����𑗂�
		if (Input::GetPushA()) player->CraftingPushA();

		// X�{�^�����͂�����΃N���t�g����X�{�^�����͏����𑗂�
		if (Input::GetPushX()) player->CraftingPushX();
	}

	// �X�e�[�g�I�����̏���
	void PlayerCraftingState::Exit(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}