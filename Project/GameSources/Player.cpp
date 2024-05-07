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

		// �d�͂̒ǉ�
		AddComponent<Gravity>();

		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// �N���t�g�@�\�̐���
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft = make_shared<CraftManager>(cWindow, cQTE);

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	// ���t���[���X�V����
	void Player::OnUpdate()
	{
		// �̌@���Ȃ�
		if (m_status(ePlayerStatus::IsMining))
		{
			// �̌@���̍X�V
			UpdateMining();
		}

		// �̌@������Ȃ����
		if (!m_status(ePlayerStatus::IsMining))
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

			// �N���t�g������Ȃ����
			if (!m_status(ePlayerStatus::IsCrafting))
			{
				// �ړ��X�V
				UpdateMove();
			}
		}

		// �A�C�e����Ԃ̍X�V
		UpdateItemStatus();

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
		Debug::Log(L"�ړ����� : ", m_status(ePlayerStatus::IsMove));
		Debug::Log(L"�̌@���� : ", m_status(ePlayerStatus::IsMining));
		Debug::Log(L"�N���t�g���� : ", m_status(ePlayerStatus::IsCrafting));
		Debug::Log(L"�N���t�gQTE���� : ", m_status(ePlayerStatus::IsCraftQTE));
		Debug::Log(L"�؂̏�����Ԃ�", m_status(ePlayerStatus::IsHaveWood) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Wood), L"��");
		Debug::Log(L"�΂̏�����Ԃ�", m_status(ePlayerStatus::IsHaveStone) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Stone), L"��");
		Debug::Log(L"���[���̏�����Ԃ�", m_status(ePlayerStatus::IsHaveRail) ? L"������ : " : L"������ : ", GetItemCount(eItemType::Rail), L"��");
	}

	// A�{�^�����͎�
	void Player::OnPushA()
	{
		// �f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime();

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

		}

	}

	// �N���t�g�Ăяo��
	void Player::OnCraft()
	{
		//// �A�C�R�����I�����Ă���N���t�g�A�C�e�����擾
		//eCraftItem item = m_craft->GetIconItem();
		eCraftItem item = eCraftItem::Rail;

		// �N���t�g
		if (m_craft->GetShowCraftWindow() && !m_status(ePlayerStatus::IsCraftQTE))
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (m_craft->CraftOrder(item))
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				m_craft->StartQTE();

			}
			return;
		}

		// �N���t�gQTE
		if (m_status(ePlayerStatus::IsCraftQTE))
		{
			// �N���t�g�}�l�[�W����QTE�̃o�[�̒�~�𑗂�
			m_craft->StopQTE(item);
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}
	}

	// �̌@�Ăяo��
	void Player::OnMining(const shared_ptr<TemplateObject>& miningObj)
	{
		// �̌@�\�I�u�W�F�N�g�Ɍ^�L���X�g
		const auto& mining = dynamic_pointer_cast<MiningObject>(miningObj);
		if (!mining) return;
		
		// �̌@�I�u�W�F�N�g�ɍ̌@�����𑗂�
		mining->OnMining();

		// �^�O�ɉ����ăA�C�e����ǉ�
		if (mining->FindTag(L"Tree"))
		{
			AddItemCount(eItemType::Wood, 1);
		}
		if (mining->FindTag(L"Rock"))
		{
			AddItemCount(eItemType::Stone, 1);
		}

		// �̌@��Ԃɂ���
		m_status.Set(ePlayerStatus::IsMining) = true;
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

		//auto ptrXA = App::GetApp()->GetXAudio2Manager();
		//ptrXA->Start(L"Rail_SE", 1, 0.5f);

	}
	
	// �̌@���̍X�V
	void Player::UpdateMining()
	{		//�T�E���h�̍Đ�

		// �̌@���̃A�j���[�V�����X�V
		// UpdateAnimation(ePlayerStatus::IsMining);

		// �̌@���̑ҋ@����
		// �{���Ȃ�A�j���[�V�����I�����Ԃŏ�ԑJ�ڂ����邪
		// ����̓^�C�}�[�őҋ@���Ԃ��Č�����
		if (SetTimer(0.5f))
		{
			m_status.Set(ePlayerStatus::IsMining) = false;
		}
	}

	// �ړ��X�V
	void Player::UpdateMove()
	{

		// LStick�̓��͂�����A�N���t�g������Ȃ����
		bool isMoving = IsInputLStick();
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

	// �A�C�e����Ԃ̍X�V
	void Player::UpdateItemStatus()
	{
		// �A�C�e����Ԃ̍X�V(����A�C�e���̒ǉ�������ΓK�X�ǉ�)
		m_status.Set(ePlayerStatus::IsHaveRail) = GetItemCount(eItemType::Rail);
		m_status.Set(ePlayerStatus::IsHaveWood) = GetItemCount(eItemType::Wood);
		m_status.Set(ePlayerStatus::IsHaveStone) = GetItemCount(eItemType::Stone);
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