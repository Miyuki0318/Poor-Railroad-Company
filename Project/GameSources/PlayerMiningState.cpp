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
	void PlayerMiningState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		player->SetAnimationMesh(ePAKey::Harvesting);
	}

	// �X�e�[�g�X�V���̏���
	void PlayerMiningState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����X�V
		player->m_ptrDraw->UpdateAnimation(DELTA_TIME);

		// �̌@���̑ҋ@����
		if (!player->m_ptrDraw->IsTargetAnimeEnd()) return;

		// �̌@�������Ă���X�e�[�g��ҋ@�ɑJ�ڂ�����
		player->m_status.Set(ePlayerStatus::IsMining) = false;
		auto nState = PlayerIdleState::Instance();
		player->SetState(nState);

		// A�{�^�����͂�����Ȃ�
		if (Input::GetButtonA())
		{
			nState->OnPushA(player);
		}
	}

	// �X�e�[�g�I�����̏���
	void PlayerMiningState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerMiningState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void PlayerMiningState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}