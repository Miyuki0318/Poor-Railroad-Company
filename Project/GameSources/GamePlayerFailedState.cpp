/*!
@file GamePlayerFailedState.cpp
@brief �v���C���[�̃Q�[�����s��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GamePlayerFailedState.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GamePlayerFailedState> GamePlayerFailedState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GamePlayerFailedState> instance(new GamePlayerFailedState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GamePlayerFailedState::GetStateName() const
	{
		return L"���s��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void GamePlayerFailedState::Enter(const shared_ptr<GamePlayer>& player)
	{
		if (!player->IsAnimation(ePAK::GameFailed))
		{
			player->SetAnimationMesh(ePAK::GameFailed);
		}

		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			player->m_craft->DestroyCraftQTE();
			player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}

		if (player->GetStatus(ePlayerStatus::IsCrafting))
		{
			player->SwitchCraftWindow();
		}
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerFailedState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�������X�V
		player->UpdateAnimation(player->m_animationMap.at(ePAK::GameFailed).animeSpeed);
	}

	// �X�e�[�g�I�����̏���
	void GamePlayerFailedState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void GamePlayerFailedState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void GamePlayerFailedState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// Y�{�^�����͎�
	void GamePlayerFailedState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void GamePlayerFailedState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}