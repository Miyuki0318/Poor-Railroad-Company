/*!
@file TitlePlayerGatheringState.cpp
@brief �v���C���[�̍̌@��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<TitlePlayerGatheringState> TitlePlayerGatheringState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<TitlePlayerGatheringState> instance(new TitlePlayerGatheringState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring TitlePlayerGatheringState::GetStateName() const
	{
		return L"�̌@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void TitlePlayerGatheringState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		player->SetAnimationMesh(ePAK::Harvesting);
	}

	// �X�e�[�g�X�V���̏���
	void TitlePlayerGatheringState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����X�V
		float animeSpeed = player->m_playerData.at(ePST::GatherSpeed).at(player->m_statusLevel);
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
		player->m_status(ePlayerStatus::IsGathering) ? player->SetAnimationMesh(ePAK::Harvesting) : player->SetState(TitlePlayerIdleState::Instance());
	}

	// �X�e�[�g�I�����̏���
	void TitlePlayerGatheringState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void TitlePlayerGatheringState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}