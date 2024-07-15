/*!
@file GamePlayer.cpp
@brief �Q�[�����̃v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Input.h"
#include "OzawaUtility.h"
#include "GamePlayer.h"
#include "GamePlayerState.h"
#include "ActionGuide.h"

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

		// �X�e�[�g�}�V���̏�����
		m_playerState.reset(new GamePlayerStateMachine(GetThis<GamePlayer>()));
		m_playerState->SetState(GamePlayerIdleState::Instance());

		// �X�e�[�^�X�̐ݒ�
		const auto& scene = App::GetApp()->GetScene<Scene>();
		m_statusLevel = scene->GetStatusLevel();
		m_limitChoicesLevel = scene->GetLimitChoicesLevel();
		m_startGearLevel = scene->GetStartGearLevel();

		// ������
		m_addItem = make_pair(eItemType::None, 0);

		// �J�n���̏����M�A�ݒ�
		AddItemCount(eItemType::Gear, (int)m_playerData.at(ePST::StartGear).at(m_startGearLevel));
	}

	// ���t���[���X�V����
	void GamePlayer::OnUpdate()
	{
		// �|�[�Y���Ȃ疳��
		const auto& prog = GetTypeStage<GameStage>()->GetGameProgress();
		bool isPause = ORS(prog, eGameProgress::Pause);
		if (isPause) return;

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
		UpdateItemCount();
	}

	// ���Z�b�g����
	void GamePlayer::ResetPlayer(const Vec3& start, const Vec3& goal)
	{
		// ������
		m_acsel = 0.0f;
		m_moveValue = 0.0f;
		m_rotTarget.zero(); 
		m_currentRot.zero();
		m_startPosition = start;
		m_goalStagingPosition = goal;
		SetPosition(m_startPosition);
		ResetCrafting();

		// �ҋ@��ԃX�e�[�g�ɕύX
		m_playerState->SetState(GamePlayerIdleState::Instance());

		// �N���t�g�}�l�[�W���[�Ƀ��Z�b�g�����𑗂�
		m_craft->ResetCraftManager();

		// �X�e�[�^�X�̐ݒ�
		const auto& scene = App::GetApp()->GetScene<Scene>();
		m_statusLevel = scene->GetStatusLevel();
		m_limitChoicesLevel = scene->GetLimitChoicesLevel();
		m_startGearLevel = scene->GetStartGearLevel();

		// �J�n���̏����M�A�ݒ�
		AddItemCount(eItemType::Gear, (int)m_playerData.at(ePST::StartGear).at(m_startGearLevel));
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

		// �A�C�e���G�t�F�N�g�̐���
		m_itemFly = stagePtr->AddGameObject<FlyItemManager>(thisPtr);
		stagePtr->SetSharedGameObject(L"FlyItemManager", m_itemFly.lock());

		// �A�N�V�����K�C�h�̐���
		stagePtr->AddGameObject<ActionGuide>(GetThis<GamePlayer>(), m_indicator.lock());

		// �N���t�g�}�l�[�W���̐���
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(thisPtr, cWindow, cQTE));
		m_craft->OnCreate();
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

	// �N���t�g��Ԃł�X�{�^������
	void GamePlayer::SwitchCraftWindow()
	{
		// ���o������Ȃ����
		bool isStaging = GetTypeStage<BaseStage>()->GetIsStaging();
		if (isStaging) return;

		// �N���t�g������Ȃ����ɁA�N���t�g�s�Ȃ�
		if (!m_status(ePlayerStatus::IsCrafting) && !GetCraftPosshible()) return;

		// �N���t�g��Ԃ�؂�ւ���
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// �N���t�g�}�l�[�W���[�ɃN���t�g��Ԃ𑗂�
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));

		// �X�e�[�W�̏�Ԃ�ύX����
		const auto& stagePtr = GetTypeStage<GameStage>();
		auto prog = stagePtr->GetGameProgress();
		if (OR(prog, eGameProgress::Playing, eGameProgress::CraftPause))
		{
			stagePtr->SetGameProgress(m_status(ePlayerStatus::IsCrafting) ? eGameProgress::CraftPause : eGameProgress::Playing);
		}
	}

	// �N���t�g���Ȃ珉��������
	void GamePlayer::ResetCrafting()
	{
		// �N���t�gQTE���Ȃ����
		if (GetStatus(ePlayerStatus::IsCraftQTE))
		{
			m_craft->DestroyCraftQTE();
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}

		// �N���t�g���Ȃ�E�B���h�E�����
		if (GetStatus(ePlayerStatus::IsCrafting))
		{
			SwitchCraftWindow();
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

	// �A�C�e�����̍X�V
	void GamePlayer::UpdateItemCount()
	{
		// �A�C�e�������z���Ȃ��l�ɏ㏑��
		m_craft->m_itemCount.at(eItemType::Stone) = min(m_craft->m_itemCount.at(eItemType::Stone), (unsigned char)m_playerData.at(ePST::StoneMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Wood) = min(m_craft->m_itemCount.at(eItemType::Wood), (unsigned char)m_playerData.at(ePST::WoodMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Gear) = min(m_craft->m_itemCount.at(eItemType::Gear), (unsigned char)m_playerData.at(ePST::GearMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Rail) = min(m_craft->m_itemCount.at(eItemType::Rail), (unsigned char)m_playerData.at(ePST::RailMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::WoodBridge) = min(m_craft->m_itemCount.at(eItemType::WoodBridge), (unsigned char)m_playerData.at(ePST::BridgeMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Crossing) = min(m_craft->m_itemCount.at(eItemType::Crossing), (unsigned char)m_playerData.at(ePST::CrossingMax).at(m_limitChoicesLevel));
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
			Vec2 LStick = GetLStickValue();
			Vec3 stickValue = Vec3(LStick.x, 0.0f, LStick.y);

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
		result == GameClear ? SetState(GamePlayerSuccesState::Instance()) : SetState(GamePlayerFailedState::Instance());
	}
}