/*!
@file GamePlayerIdleState.cpp
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GamePlayerIdleState> GamePlayerIdleState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GamePlayerIdleState> instance(new GamePlayerIdleState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GamePlayerIdleState::GetStateName() const
	{
		return L"�ҋ@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void GamePlayerIdleState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �ړ���ԂȂ�ړ��X�e�[�g�ɑJ��
		if (Input::IsInputLStick()) player->m_status.Set(ePlayerStatus::IsMove);

		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAK::Waiting))
		{
			player->SetAnimationMesh(ePAK::Waiting);
		}
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerIdleState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// L�X�e�B�b�N���͂�����Έړ��X�e�[�g�ɑJ��
		if (Input::IsInputLStick() || !player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(GamePlayerMovingState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsGathering)) player->SetState(GamePlayerGatheringState::Instance());

		// �N���t�g��ԂȂ�N���t�g�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(GamePlayerCraftingState::Instance());

		player->m_ptrDraw->UpdateAnimation(DELTA_TIME * 0.75f);

		Vec3 pos = player->GetPosition();
		player->GridHitResponse(pos);
		player->SetPosition(pos);
	}

	// �X�e�[�g�I�����̏���
	void GamePlayerIdleState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void GamePlayerIdleState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
	}

	// B�{�^�����͎�
	void GamePlayerIdleState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}

	// X�{�^�����͎�
	void GamePlayerIdleState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g��ʂ�؂�ւ���
		player->SwitchCraftWindow();
	}

	// Y�{�^�����͎�
	void GamePlayerIdleState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
	}
}