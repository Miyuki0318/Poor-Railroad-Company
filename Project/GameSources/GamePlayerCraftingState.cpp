/*!
@file GamePlayerCraftingState.cpp
@brief �v���C���[�̃N���t�g��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GamePlayerCraftingState.h"
#include "GamePlayerIdleState.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GamePlayerCraftingState> GamePlayerCraftingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GamePlayerCraftingState> instance(new GamePlayerCraftingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GamePlayerCraftingState::GetStateName() const
	{
		return L"�N���t�g��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void GamePlayerCraftingState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
		player->SetAnimationMesh(ePAK::CraftStart);
	}

	// �X�e�[�g�X�V���̏���
	void GamePlayerCraftingState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g��Ԃ��������ꂽ�̂ňړ��X�e�[�g�ɑJ��
		if (player->IsAnimation(ePAK::CraftFinish) && player->m_ptrDraw->IsTargetAnimeEnd())
		{
			player->SetState(GamePlayerIdleState::Instance());
		}

		// �N���t�gQTE��ԂȂ�QTE���I��������ǂ����̊m�F�����𑗂�
		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			CheckedCraftQTE(player);
		}

		// QTE�̃A�j���[�V�������I�����Ă�����
		if (player->IsAnimeEnd(ePAK::QTESucces) || player->IsAnimeEnd(ePAK::QTEFailed))
		{
			player->SetAnimationMesh(ePAK::CraftStart, player->m_animationMap.at(ePAK::CraftFinish).flameNum * DELTA_TIME);
		}

		// �A�j���[�V�����̍X�V
		player->UpdateAnimation();

		// �N���t�g�J�n�ł��邩�̐^�U
		m_isStartCraft = player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE);
	}

	// �X�e�[�g�I�����̏���
	void GamePlayerCraftingState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void GamePlayerCraftingState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		if (m_isStartCraft)
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (player->m_craft->CraftOrder(eCraftItem::WoodBridge))
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				m_pastInput = eCurrentCraftInput::PushStartA;
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::WoodBridge, eItemType::WoodBridge);
				player->SetAnimationMesh(ePAK::Crafting);
			}
			return;
		}

		// QTE��~����
		m_currentInput = eCurrentCraftInput::PushStartA;
		PushedQTE(player);
	}

	// B�{�^�����͎�
	void GamePlayerCraftingState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		if (m_isStartCraft)
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (player->m_craft->CraftOrder(eCraftItem::Rail))
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				m_pastInput = eCurrentCraftInput::PushStartB;
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::Rail, eItemType::Rail);
				player->SetAnimationMesh(ePAK::Crafting);
			}
			return;
		}

		// QTE��~����
		m_currentInput = eCurrentCraftInput::PushStartB;
		PushedQTE(player);
	}

	// Y�{�^�����͎�
	void GamePlayerCraftingState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// �N���t�g�E�B���h�E���\���ς݂ŁAQTE������Ȃ����
		if (m_isStartCraft)
		{
			// �N���t�g���߂𑗂�A�N���t�g�\�ł����true
			if (player->m_craft->CraftOrder(eCraftItem::Crossing))
			{
				// QTE��Ԃ��I���ɂ��AQTE���J�n������
				m_pastInput = eCurrentCraftInput::PushStartY;
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::Crossing, eItemType::Crossing);
				player->SetAnimationMesh(ePAK::Crafting);
			}
			return;
		}

		// QTE��~����
		m_currentInput = eCurrentCraftInput::PushStartY;
		PushedQTE(player);
	}

	// X�{�^�����͎�
	void GamePlayerCraftingState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// QTE��Ԃ���Ȃ���΃N���t�g��ʂ�؂�ւ���
		if (!player->m_status(ePlayerStatus::IsCraftQTE))
		{
			player->SwitchCraftWindow();
			player->SetAnimationMesh(ePAK::CraftFinish);
		}
	}

	// QTE�̒�~
	void GamePlayerCraftingState::PushedQTE(const shared_ptr<GamePlayer>& player)
	{
		// ���͂��s��v�Ȃ疳��
		if (m_currentInput != m_pastInput) return;

		// �N���t�gQTE
		if (player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// QTE��~���̏����𑗂�
			StoppedCraftQTE(player);
		}
	}

	// �N���t�gQTE���I����Ă��邩�̊m�F
	void GamePlayerCraftingState::CheckedCraftQTE(const shared_ptr<GamePlayer>& player)
	{
		// QTE���I�������
		if (player->m_craft->GetEndedQTE())
		{
			// QTE��~���̏����𑗂�
			StoppedCraftQTE(player);
		}
	}

	// QTE��~���̏���
	void GamePlayerCraftingState::StoppedCraftQTE(const shared_ptr<GamePlayer>& player)
	{
		// ���͕ێ���������
		m_pastInput = eCurrentCraftInput::None;
		m_currentInput = eCurrentCraftInput::None;

		// QtE��Ԃ�����
		player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;

		// QTE�I�����̏����𑗂�A���ʂɉ����ăA�j���[�V������ύX
		bool isSucces = player->m_craft->StopQTE();
		player->SetAnimationMesh(isSucces ? ePAK::QTESucces : ePAK::QTEFailed);
		player->StartSE(isSucces ? L"C_SUCCES_SE" : L"C_FAILURE_SE", 1.0f);

		// �쐬�����A�C�e���G�t�F�N�g���΂�
		eItemType item = player->m_craft->GetCraftingItemType();
		if (!isSucces && item == eItemType::Crossing) return;
		player->m_itemFly.lock()->StartFly(item);
	}
}