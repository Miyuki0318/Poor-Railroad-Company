/*!
@file PlayerMiningState.cpp
@brief �v���C���[�̍̌@��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerMiningState> PlayerMiningState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerMiningState> instance(new PlayerMiningState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerMiningState::GetStateName() const
	{
		return L"�̌@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerMiningState::Enter(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�X�V���̏���
	void PlayerMiningState::Execute(const shared_ptr<Player>& player)
	{
		// �̌@���̃A�j���[�V�����X�V
		// UpdateAnimation(ePlayerStatus::IsMining);

		// �̌@���̑ҋ@����
		// �{���Ȃ�A�j���[�V�����I�����Ԃŏ�ԑJ�ڂ����邪
		// ����̓^�C�}�[�őҋ@���Ԃ��Č�����
		if (player->SetTimer(0.1f))
		{
			player->m_status.Set(ePlayerStatus::IsMining) = false;

			player->SetState(PlayerIdleState::Instance());
		}
	}

	// �X�e�[�g�I�����̏���
	void PlayerMiningState::Exit(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerMiningState::OnPushA(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void PlayerMiningState::OnPushX(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}