/*!
@file Player.cpp
@brief �v���C���[
@author ���V���M
*/

#include "stdafx.h"
#include <cmath>
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// �������̏���
	void Player::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_RED);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();

		// �d�͂̒ǉ�
		AddComponent<Gravity>();

		// �X�e�[�g�}�V���̏�����
		m_playerState.reset(new PlayerStateMachine(GetThis<Player>()));
		m_playerState->SetState(PlayerIdleState::Instance());

		// �X�e�[�W�̎擾(shared_ptr��const�Ŏ擾)
		const auto& stagePtr = GetStage();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// �N���t�g�}�l�[�W���̐���
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(cWindow, cQTE));

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	// ���t���[���X�V����
	void Player::OnUpdate()
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

	// �C���f�B�P�[�^�[�̎擾�ɉ����ď���
	void Player::IndicatorOrder()
	{
		// �G���[�`�F�b�N
		if (!m_indicator.lock()) return;

		// �̌@�\���A�\�Ȃ�̌@�\�I�u�W�F�N�g�̃|�C���^��Ԃ�
		const auto& miningObj = m_indicator.lock()->GetMiningPossible();

		// �̌@�\�I�u�W�F�N�g�̃|�C���^�������
		if (miningObj)
		{
			// �̌@�֐���Ԃ�
			MiningProcces(miningObj);
			return;
		}

		// ���[���ݒu�p�̍��W��ݒ�
		Vec3 railPos = m_indicator.lock()->GetPosition();
		railPos.y = 1.0f;

		// ���[����ݒu�\�����C���f�B�P�[�^�[����擾
		if (m_indicator.lock()->GetRailedPossible(railPos))
		{
			// ���[�����������Ă���ݒu�����𑗂�
			if (GetItemCount(eItemType::Rail))
			{
				AddRailed(railPos);
			}
		}
	}
		
	// �̌@����
	void Player::MiningProcces(const shared_ptr<TemplateObject>& miningObj)
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

	// ���[���̐ݒu
	void Player::AddRailed(const Vec3& railPosition)
	{
		// �����X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// ���[���I�u�W�F�N�g�𐶐����ăO���[�v�ɒǉ�
		const auto& rail = stagePtr->AddGameObject<Rail>(railPosition);
		stagePtr->GetSharedObjectGroup(L"Rails")->IntoGroup(rail);

		// ���[���̏����������炷
		m_craft->UseItem(eItemType::Rail);
	}

	// �N���t�g��Ԃł�X�{�^������
	void Player::SwitchCraftWindow()
	{
		// �N���t�g��Ԃ�؂�ւ���
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// �N���t�g�}�l�[�W���[�ɃN���t�g��Ԃ𑗂�
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
	}

	// �N���t�gQTE���I����Ă��邩�̊m�F
	void Player::CheckedCraftQTE()
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
	void Player::UpdateStatus()
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
	void Player::UpdateMove()
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

	// ��]��ݒ�
	void Player::SetRotateTarget(const Vec3& stickValue)
	{
		// �R���g���[���[�̓��͂����]�����x�N�g����ݒ�
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// ��]�X�V����
	void Player::UpdateRotation()
	{
		// ��]�����x�N�g���ƌ��݂̉�]�x�N�g���̍��������]�ʂ�ݒ�
		Vec3 rot = m_rotTarget - m_currentRot;
		float rotate = rot.length() * (DELTA_TIME / m_rotSpeed);
		m_currentRot += Vec3(rot.normalize() * rotate);

		// ��]�ʂ����Z���ꂽ���݂̉�]�x�N�g���̃��W�A����Y���Ƃ��Đݒ�
		float rotY = atan2f(m_currentRot.z, m_currentRot.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// �R���g���[���[�ɂ��ړ�
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		pos += stickValue * m_moveSpeed * DELTA_TIME;
		
		// ���W�̍X�V
		SetPosition(pos);
	}
}