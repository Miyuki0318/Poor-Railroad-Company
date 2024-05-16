/*!
@file PlayerMovingState.cpp
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

const float ANIME_SPEED = 0.75f; // �A�j���[�V�����̑��x

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
	void PlayerMovingState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAKey::Walk))
		{
			player->SetAnimationMesh(ePAKey::Walk);
		}
	}

	// �X�e�[�g�X�V���̏���
	void PlayerMovingState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �ҋ@��ԂȂ�ړ��X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(PlayerIdleState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// �N���t�g��ԂȂ�N���t�g�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());

		// �ړ��X�V�𑗂�
		player->m_moveValue = 0.0f;
		player->UpdateMove();
		player->UpdateRotation();

		// �A�j���[�V�����̍X�V
		player->m_moveValue = min(floor(Utility::RadToDeg(player->m_moveValue), 1), player->m_maxMove);
		player->m_ptrDraw->UpdateAnimation(DELTA_TIME * player->m_moveValue * ANIME_SPEED);
	}

	// �X�e�[�g�I�����̏���
	void PlayerMovingState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerMovingState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}

	// X�{�^�����͎�
	void PlayerMovingState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g��ʂ�؂�ւ���
		player->SwitchCraftWindow();
	}
}