/*!
@file TitlePlayerStartState.cpp
@brief �v���C���[�̒�~��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<TitlePlayerStartState> TitlePlayerStartState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<TitlePlayerStartState> instance(new TitlePlayerStartState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring TitlePlayerStartState::GetStateName() const
	{
		return L"�J�n��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void TitlePlayerStartState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		player->SetAnimationMesh(ePAK::Walking);

		// ��Ԃ̍��W�����ɁA���`��ԂɎg�����W��ێ�
		const auto& train = player->GetStage()->GetSharedGameObject<TitleTrain>(L"TitleTrain");
		m_startPosition = player->GetPosition();
		m_endPosition = (train->GetPosition() + BACK_VEC);
		m_endPosition.y = m_startPosition.y;

		// ��]���ݒ�
		float rotY = atan2f(m_endPosition.z - m_startPosition.z, m_endPosition.x - m_startPosition.x);
		player->m_rotTarget = Vec3(cos(rotY), 0.0f, sin(-rotY));

		m_totalTime = 0.0f;
	}

	// �X�e�[�g�X�V���̏���
	void TitlePlayerStartState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����Ɖ�]�̍X�V
		player->UpdateAnimation();
		player->UpdateRotation();

		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_startPosition, m_endPosition, m_totalTime / m_moveTime);
		m_totalTime += DELTA_TIME;
		m_totalTime = min(m_totalTime, m_moveTime);

		// �ړ����ԂɒB�����Ȃ�
		if (m_totalTime >= m_moveTime)
		{
			// ��~��ԃX�e�[�g�ɐ؂�ւ�
			player->SetState(TitlePlayerPauseState::Instance());

			// �J�����̃Y�[�����I��
			const auto& titleStage = player->GetTypeStage<TitleStage>();
			auto& zoomCamera = dynamic_pointer_cast<MainCamera>(titleStage->GetView()->GetTargetCamera());
			zoomCamera->ZoomEnd();
		}

		// ���W���X�V
		player->SetPosition(pos);
	}

	// �X�e�[�g�I�����̏���
	void TitlePlayerStartState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void TitlePlayerStartState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}