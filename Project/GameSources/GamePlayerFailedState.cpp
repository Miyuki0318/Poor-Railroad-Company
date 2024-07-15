/*!
@file GamePlayerFailedState.cpp
@brief �v���C���[�̃Q�[�����s��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GamePlayerFailedState.h"
#include "GameTrain.h"

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
		// �����A�j���[�V�����ɂ���
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}

		// �N���t�g���Ȃ珉��������
		player->ResetCrafting();

		const auto& stagePtr = player->GetStage();
		const auto& train = stagePtr->GetSharedGameObject<GameTrain>(L"Train");

		// ��]����S�[�����o�ʒu�����ɂ���
		float rotY = atan2f(train->GetPosition().z - player->m_position.z, train->GetPosition().x - player->m_position.x);
		player->m_rotTarget = Vec3(cos(rotY), 0.0f, -sin(rotY));
		m_rotate = Vec3(cos(player->GetRotation().y), 0.0f, sin(player->GetRotation().y));
		m_totalTime = 0.0f;
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerFailedState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�������X�V
		Vec3 toRot = Utility::Lerp(m_rotate, player->m_rotTarget, m_totalTime);
		m_totalTime += DELTA_TIME / (m_rotate - player->m_rotTarget).length();

		// ���[�e�[�V�����̍X�V
		float rotY = atan2f(toRot.z, toRot.x);
		player->SetRotation(Vec3(0.0f, rotY, 0.0f));

		// �o�ߎ��Ԃ�1.0�ȏ�ɂȂ�����
		if (m_totalTime >= 1.0f)
		{
			m_totalTime = 1.0f;

			// �|���A�j���[�V�����ɕύX
			if (!player->IsAnimation(ePAK::GameFailed))
			{
				player->SetAnimationMesh(ePAK::GameFailed);
			}
			player->UpdateAnimation(player->m_animationMap.at(ePAK::GameFailed).animeSpeed);
			return;
		}

		// �A�j���[�V�����̍X�V
		player->UpdateAnimation();
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