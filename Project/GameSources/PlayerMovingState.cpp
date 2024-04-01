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
		return L"�ړ���ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerMovingState::Enter(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�X�V���̏���
	void PlayerMovingState::Execute(const shared_ptr<Player>& player)
	{
		// �ړ���ԂȂ�ړ��X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(PlayerIdleState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// �N���t�g��ԂȂ�N���t�g�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());

		// �ړ��X�V�𑗂�
		player->UpdateMove();
	}

	// �X�e�[�g�I�����̏���
	void PlayerMovingState::Exit(const shared_ptr<Player>& player) 
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerMovingState::OnPushA(const shared_ptr<Player>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}

	// X�{�^�����͎�
	void PlayerMovingState::OnPushX(const shared_ptr<Player>& player)
	{
		// �N���t�g��ʂ�؂�ւ���
		player->SwitchCraftWindow();
	}
}