/*!
@file PlayerMovingState.cpp
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerMovingState> PlayerMovingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerMovingState> instance(new PlayerMovingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerMovingState::GetStateName() const
	{
		return L"Moving";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerMovingState::Enter(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�X�V���̏���
	void PlayerMovingState::Execute(const shared_ptr<Player>& player)
	{
		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsMining)) player->ChangeState(PlayerMiningState::Instance());

		// �N���t�g��ԂȂ�N���t�g�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->ChangeState(PlayerCraftingState::Instance());

		// A�{�^�����͂�����΍̌@�A�܂��̓��[���̐ݒu���s��
		if (Input::GetPushA()) player->MovingPushA();

		// X�{�^�����͂�����΃N���t�g���s��
		if (Input::GetPushX()) player->CraftingPushX();

		// �ړ��X�V�𑗂�
		player->UpdateMove();
	}

	// �X�e�[�g�I�����̏���
	void PlayerMovingState::Exit(const shared_ptr<Player>& player) 
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}