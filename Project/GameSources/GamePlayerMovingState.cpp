/*!
@file GamePlayerMovingState.cpp
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GamePlayerMovingState> GamePlayerMovingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GamePlayerMovingState> instance(new GamePlayerMovingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GamePlayerMovingState::GetStateName() const
	{
		return L"�ړ���ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void GamePlayerMovingState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerMovingState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �|�[�Y���Ȃ疳��
		bool isPause = player->GetTypeStage<GameStage>()->GetGameProgress() == eGameProgress::Pause;
		if (isPause) return;

		// �ҋ@��ԂȂ�ړ��X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(GamePlayerIdleState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsGathering)) player->SetState(GamePlayerGatheringState::Instance());

		// �N���t�g��ԂȂ�N���t�g�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(GamePlayerCraftingState::Instance());

		// �ړ��X�V
		UpdateMoving(player);
	}

	// �X�e�[�g�I�����̏���
	void GamePlayerMovingState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void GamePlayerMovingState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void GamePlayerMovingState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}

	// X�{�^�����͎�
	void GamePlayerMovingState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g��ʂ�؂�ւ���
		player->SwitchCraftWindow();
	}

	// Y�{�^�����͎�
	void GamePlayerMovingState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �ړ��X�V
	void GamePlayerMovingState::UpdateMoving(const shared_ptr<GamePlayer>& player)
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