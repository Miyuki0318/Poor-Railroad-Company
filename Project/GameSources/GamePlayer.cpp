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

		// �J�n���̍��W�Ɉړ�
		SetPosition(m_startPosition);

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

		// �X�e�[�g�}�V����B�{�^�����͎��̏����𑗂�
		if (GetPushB()) m_playerState->PushB();

		// �X�e�[�g�}�V����Y�{�^�����͎��̏����𑗂�
		if (GetPushY()) m_playerState->PushY();

		// �X�e�[�g�}�V����X�{�^�����͎��̏����𑗂�
		if (GetPushX()) m_playerState->PushX();

		// �A�C�e����Ԃ̍X�V
		UpdateStatus();

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
		Debug::Log(L"�v���C���[�̃X�e�[�g : ", m_playerState->GetCurrentState()->GetStateName());
	}

	// ���Z�b�g����
	void GamePlayer::ResetPlayer()
	{
		// ������
		m_acsel = 0.0f;
		m_moveValue = 0.0f;
		m_rotTarget.zero(); 
		m_currentRot.zero();
		SetPosition(m_startPosition);

		// �ҋ@��ԃX�e�[�g�ɕύX
		m_playerState->SetState(PlayerIdleState::Instance());

		// �N���t�g�}�l�[�W���[�Ƀ��Z�b�g�����𑗂�
		m_craft->StopQTE();
		m_craft->CraftingEnabled(false);
		m_craft->ResetCraftManager();
	}

	// �R���|�[�l���g�ݒ�
	void GamePlayer::CreateComponent()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// �e�̐ݒ�
		m_ptrShadow = AddComponent<Shadowmap>();
		m_ptrShadow->SetMeshToTransformMatrix(m_modelMat);

		// ���b�V���ƃA�j���[�V�����̐ݒ�
		SetAnimationMesh(ePAKey::Waiting);

		// �R���W����OBB�̒ǉ�
		AddComponent<CollisionCapsule>();
	}

	// �v���C���[�ɕt������@�\�̐���
	void GamePlayer::CreatePlayerFeatures()
	{
		// �X�e�[�W�̎擾(shared_ptr��const�Ŏ擾)
		const auto& stagePtr = GetStage();

		// this�X�}�[�g�|�C���^
		const auto& thisPtr = GetThis<TemplateObject>();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(thisPtr);

		// �A�N�V�����K�C�h�̐���
		stagePtr->AddGameObject<ActionGuide>(GetThis<GamePlayer>(), m_indicator.lock());

		// �N���t�g�}�l�[�W���̐���
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(thisPtr, cWindow, cQTE));
	}

	// �C���f�B�P�[�^�[�̎擾�ɉ����ď���
	void GamePlayer::IndicatorOrder()
	{
		// �G���[�`�F�b�N
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;
		
		// �C���f�B�P�[�^�[�̕����Ɍ���
		if (GetButtonRB()) SetRotateIndicatorAngle();

		// �̌@���߁A�̌@�ł�����I��
		if (GatheringOrder(indicator)) return;

		// ���[���ݒu���߁A�ݒu�ł�����I��
		if (AddRailOrder(indicator)) return;

		// �؂̑���ݒu���߁A�ݒu�ł�����I��
		if (AddBridgeOrder(indicator)) return;

		// ���؂̐ݒu���߁A�ݒu�ł�����I��
		if (AddCrossingOrder(indicator)) return;
	}

	// �̌@����
	bool GamePlayer::GatheringOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// �̌@���߂𑗂�A�̌@�ł�����^�O�Z�b�g���󂯎��
		int id = indicator->GatheringOrder();

		// �̌@�I�u�W�F�N�g�̃^�O�Z�b�g���󂶂�Ȃ���΍̌@�����𑗂�
		if (id != 0)
		{
			GatheringProcces(id);
			return true;
		}
		
		return false;
	}

	// �̌@����
	void GamePlayer::GatheringProcces(int stageID)
	{
		// �̌@�Ώۃ}�b�v��p���č̌@����ǉ�
		eStageID id = STAGE_ID(stageID);
		
		// �A�C�e���J�E���^�̒ǉ���SE�̍Đ�
		if (m_gCountMap.find(id) != m_gCountMap.end())
		{
			AddItemCount(m_gCountMap.at(id));
		}

		if (m_gSoundMap.find(id) != m_gSoundMap.end())
		{
			StartSE(m_gSoundMap.at(id) + L"_SE", 1.0f);
		}

		// �̌@��Ԃɂ���
		m_status.Set(ePlayerStatus::IsGathering) = true;
	}

	// ���[���ǉ�����
	bool GamePlayer::AddRailOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// ���[�����������Ă���
		if (GetItemCount(eItemType::Rail))
		{
			// �ݒu���߂𑗂�A�ݒu�ł����珊���������炷
			if (indicator->RailedOrder())
			{
				m_craft->UseItem(eItemType::Rail);
				StartSE(L"ADDRAIL_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// �؂̑���ǉ�����
	bool GamePlayer::AddBridgeOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// �؂̑�����������Ă���
		if (GetItemCount(eItemType::WoodBridge))
		{
			if (indicator->BridgeOrder())
			{
				m_craft->UseItem(eItemType::WoodBridge);
				//StartSE(L"ADDBRIDGE_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// ���ؒǉ�����
	bool GamePlayer::AddCrossingOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// ���؂��������Ă���
		if (GetItemCount(eItemType::Crossing))
		{
			if (indicator->CrossingOrder())
			{
				m_craft->UseItem(eItemType::Crossing);
				StartSE(L"ADDRAIL_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// �A�N�V�������ɃC���f�B�P�[�^�[�Ɍ���
	void GamePlayer::SetRotateIndicatorAngle()
	{
		// �G���[�`�F�b�N
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;

		// ���W���m�Ŋp�x�����߂�
		Vec3 indiPos = indicator->GetPosition();
		float rotY = atan2f(indiPos.z - m_position.z, indiPos.x - m_position.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(-rotY));
	}

	// �N���t�g��Ԃł�X�{�^������
	void GamePlayer::SwitchCraftWindow()
	{
		// �N���t�g������Ȃ����ɁA�N���t�g�s�Ȃ�
		if (!m_status(ePlayerStatus::IsCrafting) && !GetCraftPosshible()) return;

		// �N���t�g��Ԃ�؂�ւ���
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// �N���t�g�}�l�[�W���[�ɃN���t�g��Ԃ𑗂�
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
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
		// �p�����̈ړ��X�V����
		Player::UpdateMove();

		// LStick�̓��͂�����Ȃ�
		bool isLStick = IsInputLStick();
		if (isLStick)
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			m_acsel += DELTA_TIME;
			m_acsel = min(m_acsel, m_maxAcsel);
			SetRotateTarget(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}

		// �ړ���Ԃ�ݒ�
		m_status.Set(ePlayerStatus::IsMove) = isLStick;
	}

	// �Q�[�����ʂɉ����ăX�e�[�g��؂�ւ���
	void GamePlayer::SetGameResult(eGameProgress result)
	{
		// �v���C���[���Ȃ疳��
		if (result == eGameProgress::Playing) return;

		// �N���A�Ȃ�T�N�Z�X�X�e�[�g�A���s�Ȃ�t�F�C���h�X�e�[�g�ɐ؂�ւ���
		result == GameClear ? SetState(PlayerSuccesState::Instance()) : SetState(PlayerFailedState::Instance());
	}
}