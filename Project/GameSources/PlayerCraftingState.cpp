/*!
@file PlayerCraftingState.cpp
@brief �v���C���[�̃N���t�g��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerCraftingState> PlayerCraftingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerCraftingState> instance(new PlayerCraftingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerCraftingState::GetStateName() const
	{
		return L"�N���t�g��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerCraftingState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
		player->SetAnimationMesh(ePAKey::CraftStart);
	}

	// �X�e�[�g�X�V���̏���
	void PlayerCraftingState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g��Ԃ��������ꂽ�̂ňړ��X�e�[�g�ɑJ��
		if (player->IsAnimation(ePAKey::CraftFinish) && player->m_ptrDraw->IsTargetAnimeEnd())
		{
			player->SetState(PlayerIdleState::Instance());
		}

		// �N���t�gQTE��ԂȂ�QTE���I��������ǂ����̊m�F�����𑗂�
		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			CheckedCraftQTE(player);
		}

		// QTE�̃A�j���[�V�������I�����Ă�����
		if (player->IsAnimeEnd(ePAKey::QTESucces) || player->IsAnimeEnd(ePAKey::QTEFailed))
		{
			player->SetAnimationMesh(ePAKey::CraftStart, player->m_animationMap.at(ePAKey::CraftFinish).flameNum * DELTA_TIME);
		}

		player->UpdateAnimation();

		//// A�{�^�����͂�����΃N���t�g����A�{�^�����͏����𑗂�
		//if (Input::GetPushA()) player->CraftingPushA();

		//// X�{�^�����͂�����΃N���t�g����X�{�^�����͏����𑗂�
		//if (Input::GetPushX()) player->SwitchCraftWindow();
	}

	// �X�e�[�g�I�����̏���
	void PlayerCraftingState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerCraftingState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		if (player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (player->m_craft->CraftOrder(eCraftItem::WoodBridge))
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::WoodBridge, eItemType::WoodBridge);
				player->SetAnimationMesh(ePAKey::Crafting);
			}
			return;
		}

		// �N���t�gQTE
		if (player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// QTE��~���̏����𑗂�
			StoppedCraftQTE(player);
		}
	}

	// B�{�^�����͎�
	void PlayerCraftingState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		if (player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (player->m_craft->CraftOrder(eCraftItem::Rail))
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::Rail, eItemType::Rail);
				player->SetAnimationMesh(ePAKey::Crafting);
			}
			return;
		}

		// �N���t�gQTE
		if (player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// QTE��~���̏����𑗂�
			StoppedCraftQTE(player);
		}
	}

	// Y�{�^�����͎�
	void PlayerCraftingState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		//// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		//if (player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE))
		//{
		//	// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
		//	if (player->m_craft->CraftOrder())
		//	{
		//		// QTE��Ԃ��I���ɂ��AQTE���J�n������
		//		player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
		//		player->m_craft->StartQTE();
		//		player->SetAnimationMesh(ePAKey::Crafting);
		//	}
		//	return;
		//}

		//// �N���t�gQTE
		//if (player->m_status(ePlayerStatus::IsCraftQTE))
		//{
		//	// QTE��~���̏����𑗂�
		//	StoppedCraftQTE(player);
		//}
	}

	// X�{�^�����͎�
	void PlayerCraftingState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// QTE��Ԃ���Ȃ���΃N���t�g��ʂ�؂�ւ���
		if (!player->m_status(ePlayerStatus::IsCraftQTE))
		{
			player->SwitchCraftWindow();
			player->SetAnimationMesh(ePAKey::CraftFinish);
		}
	}

	// �N���t�gQTE���I����Ă��邩�̊m�F
	void PlayerCraftingState::CheckedCraftQTE(const shared_ptr<GamePlayer>& player)
	{
		// QTE���I�������
		if (player->m_craft->GetEndedQTE())
		{
			// QTE��~���̏����𑗂�
			StoppedCraftQTE(player);
		}
	}

	// QTE��~���̏���
	void PlayerCraftingState::StoppedCraftQTE(const shared_ptr<GamePlayer>& player)
	{
		// QtE��Ԃ�����
		player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;

		// QTE�I�����̏����𑗂�A���ʂɉ����ăA�j���[�V������ύX
		bool isSucces = player->m_craft->StopQTE();
		player->SetAnimationMesh(isSucces ? ePAKey::QTESucces : ePAKey::QTEFailed);
		player->StartSE(isSucces ? L"C_SUCCES_SE" : L"C_FAILURE_SE", 1.0f);
	}
}