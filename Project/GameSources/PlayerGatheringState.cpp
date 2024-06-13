/*!
@file PlayerGatheringState.cpp
@brief �v���C���[�̍̌@��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerGatheringState> PlayerGatheringState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerGatheringState> instance(new PlayerGatheringState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerGatheringState::GetStateName() const
	{
		return L"�̌@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerGatheringState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		player->SetAnimationMesh(ePAKey::Harvesting);
	}

	// �X�e�[�g�X�V���̏���
	void PlayerGatheringState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����X�V
		float animeSpeed = player->m_playerData.at(ePST::GatherSpeed).at(player->m_playerLevel);
		player->UpdateAnimation(animeSpeed);
		player->UpdateRotation();

		// �̌@���̑ҋ@����
		if (!player->m_ptrDraw->IsTargetAnimeEnd()) return;

		// �̌@�������Ă���X�e�[�g��ҋ@�ɑJ�ڂ�����
		player->m_status.Set(ePlayerStatus::IsGathering) = false;

		// A�{�^�����͂�����Ȃ�
		if (Input::GetButtonB())
		{
			player->IndicatorOrder();
		}

		// �̎�X�e�[�^�X�ɂȂ��Ă���΃A�j���[�V�����̐؂�ւ��A�Ȃ��ĂȂ���΃X�e�[�g�̐؂�ւ�
		player->m_status(ePlayerStatus::IsGathering) ? player->SetAnimationMesh(ePAKey::Harvesting) : player->SetState(PlayerIdleState::Instance());
	}

	// �X�e�[�g�I�����̏���
	void PlayerGatheringState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerGatheringState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void PlayerGatheringState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void PlayerGatheringState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// Y�{�^�����͎�
	void PlayerGatheringState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}