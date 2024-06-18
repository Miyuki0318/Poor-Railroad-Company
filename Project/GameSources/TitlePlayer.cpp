/*!
@file TitlePlayer.cpp
@brief �^�C�g���p�v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// �������̏���
	void TitlePlayer::OnCreate()
	{
		// �p�����̐������̏��������s
		Player::OnCreate();

		// �X�e�[�g�}�V���̏�����
		m_playerState.reset(new TitlePlayerStateMachine(GetThis<TitlePlayer>()));
		m_playerState->SetState(TitlePlayerPauseState::Instance());
	}

	// ���t���[���X�V����
	void TitlePlayer::OnUpdate()
	{
		// �X�e�[�g�}�V���̍X�V�����𑗂�
		m_playerState->Update();

		// �X�e�[�g�}�V����B�{�^�����͎��̏����𑗂�
		if (GetPushB()) m_playerState->PushB();

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
		Debug::Log(L"�v���C���[�̃X�e�[�g : ", m_playerState->GetCurrentState()->GetStateName());
	}

	// �v���C���[�ɕt������@�\�̐���
	void TitlePlayer::CreatePlayerFeatures()
	{
		// �X�e�[�W�̎擾(shared_ptr��const�Ŏ擾)
		const auto& stagePtr = GetStage();

		// this�X�}�[�g�|�C���^
		const auto& thisPtr = GetThis<TemplateObject>();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(thisPtr);

		// �A�N�V�����K�C�h�̐���
		stagePtr->AddGameObject<ActionGuide>(GetThis<TitlePlayer>(), m_indicator.lock());
	}

	// �C���f�B�P�[�^�[�̎擾�ɉ����ď���
	void TitlePlayer::IndicatorOrder()
	{
		// �G���[�`�F�b�N
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;

		// �C���f�B�P�[�^�[�̕����Ɍ���
		if (GetButtonRB()) SetRotateIndicatorAngle();

		// �̌@����
		GatheringOrder(indicator);
	}

	// �ړ��X�V
	void TitlePlayer::UpdateMove()
	{
		// LStick�̓��͂�����Ȃ�
		if (IsInputLStick())
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			m_acsel += DELTA_TIME;
			m_acsel = min(m_acsel, m_maxAcsel);
			SetRotateTarget(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}
	}
}