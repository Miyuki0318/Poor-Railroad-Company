/*!
@file Player.cpp
@brief �v���C���[
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

		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// �N���t�g�@�\�̐���
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		m_craft = make_shared<CraftManager>(cWindow);

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	// ���t���[���X�V����
	void Player::OnUpdate()
	{
		// A�{�^�������͂��ꂽ��
		if (GetPushA())
		{
			// A�{�^�����͎��̏������N���t�g��Ԃŕ��򂳂���
			m_status(ePlayerStatus::IsCrafting) ? OnCraft() : OnPushA();
		}

		// X�{�^�������͂��ꂽ��
		if (GetPushX())
		{
			// �N���t�g��Ԃ�؂�ւ���
			m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

			// �N���t�g�}�l�[�W���[�ɃN���t�g��Ԃ𑗂�
			m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
		}

		// �ړ��X�V
		UpdateMove();

		// �A�C�e����Ԃ̍X�V
		m_status.Set(ePlayerStatus::IsHaveRail) = GetItemCount(eItemType::Rail);
		m_status.Set(ePlayerStatus::IsHaveWood) = GetItemCount(eItemType::Wood);
		m_status.Set(ePlayerStatus::IsHaveStone) = GetItemCount(eItemType::Stone);

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
		Debug::Log(L"�ړ����� : ", m_status(ePlayerStatus::IsMove));
		Debug::Log(L"�N���t�g���� : ", m_status(ePlayerStatus::IsCrafting));
		Debug::Log(L"�؂̏�����Ԃ�", m_status(ePlayerStatus::IsHaveWood) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Wood), L"��");
		Debug::Log(L"�΂̏�����Ԃ�", m_status(ePlayerStatus::IsHaveStone) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Stone), L"��");
		Debug::Log(L"���[���̏�����Ԃ�", m_status(ePlayerStatus::IsHaveRail) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Rail), L"��");
	}

	// A�{�^�����͎�
	void Player::OnPushA()
	{
		// �G���[�`�F�b�N
		if (!m_indicator.lock()) return;

		// �̌@�\���A�\�Ȃ�̌@�\�I�u�W�F�N�g�̃|�C���^��Ԃ�
		const auto& miningObj = m_indicator.lock()->GetMiningPossible();

		// �̌@�\�I�u�W�F�N�g�̃|�C���^�������
		if (miningObj)
		{
			// �̌@�֐���Ԃ�
			OnMining(miningObj);
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
				OnRailed(railPos);
			}
			return;
		}
	}

	// �N���t�g�Ăяo��
	void Player::OnCraft()
	{
		// �N���t�g�̂ݑ����Ă��邪�A���ł�QTE�Ɉڍs������
		m_craft->Crafting(GetThis<Player>());
	}

	// �̌@�Ăяo��
	void Player::OnMining(const shared_ptr<TemplateObject>& miningObj)
	{
		// �̌@�\�I�u�W�F�N�g�Ɍ^�L���X�g����
		// �̌@�Ăяo���֐��𑗂�
		// �^�O�ɉ����ăA�C�e���J�E���^�𑝉�
	}

	// ���[���̐ݒu�Ăяo��
	void Player::OnRailed(const Vec3& railPosition)
	{
		// �����X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// ���[���I�u�W�F�N�g�𐶐����ăO���[�v�ɒǉ�
		const auto& rail = stagePtr->AddGameObject<Rail>(railPosition);
		stagePtr->GetSharedObjectGroup(L"Rails")->IntoGroup(rail);

		// ���[���̏����������炷
		AddItemCount(eItemType::Rail, -1);
	}
	
	// �ړ��X�V
	void Player::UpdateMove()
	{
		// LStick�̓��͂�����A�N���t�g������Ȃ����
		bool isMoving = IsInputLStick() && !m_status(ePlayerStatus::IsCrafting);
		if (isMoving)
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			ControllerRotation(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}

		// �ړ���Ԃ�ݒ�
		m_status.Set(ePlayerStatus::IsMove) = isMoving;
		m_status.Set(ePlayerStatus::IsIdle) = !isMoving;
	}

	// �R���g���[���[�ɂ���]
	void Player::ControllerRotation(const Vec3& stickValue)
	{
		// �X�e�B�b�N�̌X���ɍ��킹�ăI�u�W�F�N�g����]������
		float rotY = atan2f(-stickValue.z, stickValue.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// �R���g���[���[�ɂ��ړ�
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		pos += stickValue * m_speed * DELTA_TIME;
		
		// ���W�̍X�V
		SetPosition(pos);
	}
}