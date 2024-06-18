/*!
@file GamePlayerGatheringState.cpp
@brief �v���C���[�̍̌@��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GamePlayerGatheringState> GamePlayerGatheringState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GamePlayerGatheringState> instance(new GamePlayerGatheringState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GamePlayerGatheringState::GetStateName() const
	{
		return L"�̌@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void GamePlayerGatheringState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		player->SetAnimationMesh(ePAK::Harvesting);

		// �t���O��������
		m_isFlyEffect = false;

		// �A�j���[�V�����̔����̎��Ԃ����߂�(�t���[���������x���ɂ�鑬�x��FPS��2.0f)
		m_animeHelfTime = player->m_animationMap.at(ePAK::Harvesting).flameNum
			/ player->m_playerData.at(ePST::GatherSpeed).at(player->m_playerLevel)
			/ ANIME_FPS / 2.0f;
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerGatheringState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����X�V
		float animeSpeed = player->m_playerData.at(ePST::GatherSpeed).at(player->m_playerLevel);
		player->UpdateAnimation(animeSpeed);
		player->UpdateRotation();

		// �A�C�e���G�t�F�N�g���o��
		float animeTime = player->m_ptrDraw->GetCurrentAnimationTime();
		if (!m_isFlyEffect && m_animeHelfTime <= animeTime)
		{
			m_isFlyEffect = true;
			player->m_itemFly.lock()->StartFly(player->m_addItem);
		}

		// �̌@���̑ҋ@����
		if (!player->m_ptrDraw->IsTargetAnimeEnd()) return;

		// �̌@�������Ă���X�e�[�g��ҋ@�ɑJ�ڂ�����
		player->m_status.Set(ePlayerStatus::IsGathering) = false;

		// A�{�^�����͂�����Ȃ�
		if (Input::GetButtonB())
		{
			player->IndicatorOrder();
			m_isFlyEffect = false;
		}

		// �̎�X�e�[�^�X�ɂȂ��Ă���΃A�j���[�V�����̐؂�ւ��A�Ȃ��ĂȂ���΃X�e�[�g�̐؂�ւ�
		player->m_status(ePlayerStatus::IsGathering) ? player->SetAnimationMesh(ePAK::Harvesting) : player->SetState(GamePlayerIdleState::Instance());
	}

	// �X�e�[�g�I�����̏���
	void GamePlayerGatheringState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void GamePlayerGatheringState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void GamePlayerGatheringState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void GamePlayerGatheringState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// Y�{�^�����͎�
	void GamePlayerGatheringState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}