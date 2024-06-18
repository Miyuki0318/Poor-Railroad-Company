/*!
@file GamePlayerSuccesState.cpp
@brief �v���C���[�̃Q�[���N���A��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GamePlayerSuccesState> GamePlayerSuccesState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GamePlayerSuccesState> instance(new GamePlayerSuccesState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GamePlayerSuccesState::GetStateName() const
	{
		return L"�N���A��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void GamePlayerSuccesState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}

		// �S�[�����̍��W��ݒ�
		player->m_goalPosition = player->GetPosition();
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerSuccesState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �����A�j���[�V�����Ȃ�
		if (player->IsAnimation(ePAK::Walking))
		{
			// �S�[�����̍��W���牉�o�p�̍��W�Ɉړ�
			Vec3 pos = Utility::Lerp(player->m_goalPosition, player->m_goalStagingPosition, m_totalTime);
			m_totalTime += DELTA_TIME;
			m_totalTime = min(m_totalTime, 1.0f);

			// �o�ߎ��Ԃ�1.0�ȏ�Ȃ�N���A���̃A�j���[�V�����ɐ؂�ւ���
			if (m_totalTime >= 1.0f) player->SetAnimationMesh(ePAK::GameSucces);

			// ���W�̍X�V
			player->SetPosition(pos);
		}

		// �A�j���[�V�������X�V
		player->UpdateAnimation();
	}

	// �X�e�[�g�I�����̏���
	void GamePlayerSuccesState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void GamePlayerSuccesState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void GamePlayerSuccesState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// Y�{�^�����͎�
	void GamePlayerSuccesState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void GamePlayerSuccesState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}