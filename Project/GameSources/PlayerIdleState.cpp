/*!
@file PlayerIdleState.cpp
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerIdleState> PlayerIdleState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerIdleState> instance(new PlayerIdleState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerIdleState::GetStateName() const
	{
		return L"�ҋ@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerIdleState::Enter(const shared_ptr<Player>& player)
	{
		// �ړ���ԂȂ�ړ��X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsMove)) player->SetState(PlayerMovingState::Instance());
	}

	// �X�e�[�g�X�V���̏���
	void PlayerIdleState::Execute(const shared_ptr<Player>& player)
	{
		// L�X�e�B�b�N���͂�����Έړ��X�e�[�g�ɑJ��
		if (Input::IsInputLStick()) player->SetState(PlayerMovingState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// �N���t�g��ԂȂ�N���t�g�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());
	}

	// �X�e�[�g�I�����̏���
	void PlayerIdleState::Exit(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerIdleState::OnPushA(const shared_ptr<Player>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}

	// X�{�^�����͎�
	void PlayerIdleState::OnPushX(const shared_ptr<Player>& player)
	{
		// �N���t�g��ʂ�؂�ւ���
		player->SwitchCraftWindow();
	}
}