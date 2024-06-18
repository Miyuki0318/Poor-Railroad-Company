/*!
@file TitlePlayerMovingState.cpp
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<TitlePlayerMovingState> TitlePlayerMovingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<TitlePlayerMovingState> instance(new TitlePlayerMovingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring TitlePlayerMovingState::GetStateName() const
	{
		return L"�ړ���ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void TitlePlayerMovingState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}
	}

	// �X�e�[�g�X�V���̏���
	void TitlePlayerMovingState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// �ҋ@��ԂȂ�ړ��X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(TitlePlayerIdleState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsGathering)) player->SetState(TitlePlayerGatheringState::Instance());

		// �ړ��X�V
		UpdateMoving(player);
	}

	// �X�e�[�g�I�����̏���
	void TitlePlayerMovingState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void TitlePlayerMovingState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}

	// �ړ��X�V
	void TitlePlayerMovingState::UpdateMoving(const shared_ptr<TitlePlayer>& player)
	{
		// �ړ��X�V�𑗂�
		player->m_moveValue = 0.0f;
		player->UpdateMove();
		player->UpdateRotation();

		// �A�j���[�V�����̍X�V
		player->m_moveValue = min(floor(Utility::RadToDeg(player->m_moveValue), 1), player->m_maxMove);
		player->UpdateAnimation(player->m_moveValue * player->m_animationMap.at(ePAK::Walking).animeSpeed);
	}
}