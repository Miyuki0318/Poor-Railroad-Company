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
		m_totalTime = 0.0f;
		m_moveTime = (player->m_goalStagingPosition - player->m_goalPosition).length() / 2.5f;

		// ��]����S�[�����o�ʒu�����ɂ���
		float rotY = atan2f(player->m_goalStagingPosition.z - player->m_goalPosition.z, player->m_goalStagingPosition.x - player->m_goalPosition.x);
		player->m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));

		// �J�����̕�����ێ�
		Vec3 cameraPos = player->GetStage()->GetView()->GetTargetCamera()->GetEye();
		rotY = atan2f(cameraPos.z - player->m_goalStagingPosition.z, cameraPos.x - player->m_goalStagingPosition.x);
		m_toCameraRot = Vec3(cos(rotY), 0.0f, sin(-rotY));
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerSuccesState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �����A�j���[�V�����Ȃ�
		if (player->IsAnimation(ePAK::Walking))
		{
			// �S�[�����̍��W���牉�o�p�̍��W�Ɉړ�
			Vec3 pos = Utility::Lerp(player->m_goalPosition, player->m_goalStagingPosition, m_totalTime / m_moveTime);
			m_totalTime += DELTA_TIME;
			m_totalTime = min(m_totalTime, m_moveTime);

			// �o�ߎ��Ԃ�1.0�ȏ�Ȃ�N���A���̃A�j���[�V�����ɐ؂�ւ���
			if (m_totalTime >= m_moveTime)
			{
				// ��]����J�����̕����ɂ���
				if (player->m_rotTarget != m_toCameraRot)
				{
					player->m_rotTarget = m_toCameraRot;
					player->UpdateStatus();
				}

				// ��]���I����Ă�����A�j���[�V�������I��
				if (!player->GetStatus(ePlayerStatus::IsRotate) && !player->IsAnimation(ePAK::GameSucces))
				{
					player->SetAnimationMesh(ePAK::GameSucces);

					const auto& stagePtr = player->GetTypeStage<GameStage>();
					stagePtr->SetGameProgress(eGameProgress::MoneyCalculation);
				}
			}

			// ���W�̍X�V
			player->SetPosition(pos);
		}

		// �A�j���[�V�������X�V
		player->UpdateAnimation();
		player->UpdateRotation();
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