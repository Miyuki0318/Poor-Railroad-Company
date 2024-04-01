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
		return L"�N���t�g��ԃX�e�[�g";
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
			player->SetState(PlayerIdleState::Instance());
		}

		// �N���t�gQTE��ԂȂ�QTE���I��������ǂ����̊m�F�����𑗂�
		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			player->CheckedCraftQTE();
		}

		//// A�{�^�����͂�����΃N���t�g����A�{�^�����͏����𑗂�
		//if (Input::GetPushA()) player->CraftingPushA();

		//// X�{�^�����͂�����΃N���t�g����X�{�^�����͏����𑗂�
		//if (Input::GetPushX()) player->SwitchCraftWindow();
	}

	// �X�e�[�g�I�����̏���
	void PlayerCraftingState::Exit(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerCraftingState::OnPushA(const shared_ptr<Player>& player)
	{
		// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		if (player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (player->m_craft->CraftOrder())
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE();
			}
			return;
		}

		// �N���t�gQTE
		if (player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// �N���t�g�}�l�[�W����QTE�̃o�[�̒�~�𑗂�
			player->m_craft->StopQTE();
			player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}
	}

	// X�{�^�����͎�
	void PlayerCraftingState::OnPushX(const shared_ptr<Player>& player)
	{
		// QTE��Ԃ���Ȃ���΃N���t�g��ʂ�؂�ւ���
		if (!player->m_status(ePlayerStatus::IsCraftQTE))
		{
			player->SwitchCraftWindow();
		}
	}
}