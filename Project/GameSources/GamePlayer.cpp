/*!
@file GamePlayer.cpp
@brief �Q�[�����̃v���C���[�I�u�W�F�N�g
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
	void GamePlayer::OnCreate()
	{
		// �p�����̐������̏��������s
		Player::OnCreate();

		// �v���C���[�ɕt������@�\�̐���
		CreatePlayerFeatures();

		// �X�e�[�g�}�V���̏�����
		m_playerState.reset(new PlayerStateMachine(GetThis<GamePlayer>()));
		m_playerState->SetState(PlayerIdleState::Instance());
	}

	// ���t���[���X�V����
	void GamePlayer::OnUpdate()
	{
		// �X�e�[�g�}�V���̍X�V�����𑗂�
		m_playerState->Update();

		// �X�e�[�g�}�V����A�{�^�����͎��̏����𑗂�
		if (GetPushA()) m_playerState->PushA();

		// �X�e�[�g�}�V����X�{�^�����͎��̏����𑗂�
		if (GetPushX()) m_playerState->PushX();

		// �A�C�e����Ԃ̍X�V
		UpdateStatus();

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̃X�e�[�g : ", m_playerState->GetCurrentState()->GetStateName());
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
		Debug::Log(L"�ړ����� : ", m_status(ePlayerStatus::IsMove));
		Debug::Log(L"�ҋ@���� : ", m_status(ePlayerStatus::IsIdle));
		Debug::Log(L"���[�e�[�V�������� : ", m_status(ePlayerStatus::IsRotate));
		Debug::Log(L"�N���t�gQTE���� : ", m_status(ePlayerStatus::IsCraftQTE));
		Debug::Log(L"�؂̏�����Ԃ�", m_status(ePlayerStatus::IsHaveWood) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Wood), L"��");
		Debug::Log(L"�΂̏�����Ԃ�", m_status(ePlayerStatus::IsHaveStone) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Stone), L"��");
		Debug::Log(L"���[���̏�����Ԃ�", m_status(ePlayerStatus::IsHaveRail) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Rail), L"��");
	}

	// �R���|�[�l���g�ݒ�
	void GamePlayer::CreateComponent()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetMultiMeshResource(L"PLAYER");
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// �A�j���[�V�����̐ݒ�
		m_ptrDraw->AddAnimation(L"WALK", 0, 60, true);
		m_ptrDraw->AddAnimation(L"MINING", 180, 60, false);

		auto shadowMap = AddComponent<Shadowmap>();
		shadowMap->SetMultiMeshResource(L"PLAYER");
		shadowMap->SetMeshToTransformMatrix(m_modelMat);

		// �R���W����OBB�̒ǉ�
		AddComponent<CollisionCapsule>();

		// �d�͂̒ǉ�
		AddComponent<Gravity>();
	}

	// �v���C���[�ɕt������@�\�̐���
	void GamePlayer::CreatePlayerFeatures()
	{
		// �X�e�[�W�̎擾(shared_ptr��const�Ŏ擾)
		const auto& stagePtr = GetStage();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// �N���t�g�}�l�[�W���̐���
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(cWindow, cQTE));
	}

	// �C���f�B�P�[�^�[�̎擾�ɉ����ď���
	void GamePlayer::IndicatorOrder()
	{
		// �G���[�`�F�b�N
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;

		// �̌@�\���A�\�Ȃ�̌@�\�I�u�W�F�N�g�̃|�C���^��Ԃ�
		const auto& miningObj = indicator->GetMiningPossible();

		// �̌@�\�I�u�W�F�N�g�̃|�C���^�������
		if (miningObj)
		{
			// �̌@�֐���Ԃ�
			MiningProcces(miningObj);
			return;
		}

		// ���[�����������Ă���ݒu�����𑗂�
		if (GetItemCount(eItemType::Rail))
		{
			// ���[����ݒu�\�����C���f�B�P�[�^�[����擾
			if (indicator->GetRailedPossible())
			{
				// ���[���̏����������炷
				m_craft->UseItem(eItemType::Rail);
			}
		}
	}

	// �̌@����
	void GamePlayer::MiningProcces(const shared_ptr<TemplateObject>& miningObj)
	{
		// �̌@�\�I�u�W�F�N�g�Ɍ^�L���X�g
		const auto& mining = dynamic_pointer_cast<MiningObject>(miningObj);
		if (!mining) return;

		// �̌@�I�u�W�F�N�g�ɍ̌@�����𑗂�
		mining->OnMining();

		// �c�[���̍̌@�͂ɉ������擾����ݒ�
		//int addNum = GetToolsMiningValue();
		int addNum = 1; // �c�[�����x���T�O�������׈�U1�ŌŒ�

		// �̌@�Ώۃ}�b�v��p���č̌@����ǉ�
		for (const auto& miningMap : m_miningMap)
		{
			if (mining->FindTag(miningMap.first))
			{
				AddItemCount(miningMap.second, addNum);
			}
		}

		// �̌@��Ԃɂ���
		m_status.Set(ePlayerStatus::IsMining) = true;
	}

	// �N���t�g��Ԃł�X�{�^������
	void GamePlayer::SwitchCraftWindow()
	{
		// �N���t�g��Ԃ�؂�ւ���
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// �N���t�g�}�l�[�W���[�ɃN���t�g��Ԃ𑗂�
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
	}

	// �N���t�gQTE���I����Ă��邩�̊m�F
	void GamePlayer::CheckedCraftQTE()
	{
		// QTE���I�������
		if (m_craft->GetEndedQTE())
		{
			// QTE�I�����̏����𑗂�AQtE��Ԃ�����
			m_craft->StopQTE();
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}
	}

	// �A�C�e����Ԃ̍X�V
	void GamePlayer::UpdateStatus()
	{
		// �ړ���Ԃ̍X�V
		m_status.Set(ePlayerStatus::IsRotate) = ((m_currentRot - m_rotTarget).length() > XM_1DIV2PI);
		m_status.Set(ePlayerStatus::IsIdle) = !m_status(ePlayerStatus::IsMove) && !m_status(ePlayerStatus::IsRotate);

		// �A�C�e����Ԃ̍X�V(����A�C�e���̒ǉ�������ΓK�X�ǉ�)
		m_status.Set(ePlayerStatus::IsHaveRail) = GetItemCount(eItemType::Rail);
		m_status.Set(ePlayerStatus::IsHaveWood) = GetItemCount(eItemType::Wood);
		m_status.Set(ePlayerStatus::IsHaveStone) = GetItemCount(eItemType::Stone);
	}

	// �ړ��X�V
	void GamePlayer::UpdateMove()
	{
		// LStick�̓��͂�����Ȃ�
		bool isLStick = IsInputLStick();
		if (isLStick)
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			SetRotateTarget(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}

		// �ړ���Ԃ�ݒ�
		m_status.Set(ePlayerStatus::IsMove) = isLStick;
	}
}