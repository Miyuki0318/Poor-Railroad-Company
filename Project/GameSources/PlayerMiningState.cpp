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
		return L"Mining";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerMiningState::Enter(const shared_ptr<Player>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�X�V���̏���
	void PlayerMiningState::Execute(const shared_ptr<Player>& player)
	{
		// �̌@��Ԃ��������ꂽ�̂ňړ��X�e�[�g�ɑJ��
		if (!player->GetStatus(ePlayerStatus::IsMining)) 
		{
			player->SetState(PlayerMovingState::Instance());
		}

		// �̌@��Ԃ̑ҋ@���Ԃ𑗂�
		player->MiningWaiting();
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