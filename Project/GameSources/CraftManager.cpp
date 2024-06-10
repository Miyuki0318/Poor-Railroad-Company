/*!
@file CraftManager.cpp
@brief �N���t�g�Ǘ��p�}�l�[�W��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;

	// ���Z�b�g����
	void CraftManager::ResetCraftManager()
	{
		m_craftItem = eCraftItem::Rail;
		m_craftType = eItemType::Rail;

		// �A�C�e���J�E���^
		m_itemCount.at(eItemType::Wood) = 0;
		m_itemCount.at(eItemType::Stone) = 0;
		m_itemCount.at(eItemType::Gear) = 1;
		m_itemCount.at(eItemType::Rail) = 0;
		m_itemCount.at(eItemType::WoodBridge) = 0;
		m_itemCount.at(eItemType::Crossing) = 0;
	}

	// �A�C�e���N���t�g
	bool CraftManager::CraftOrder(eCraftItem item)
	{
		// �f�ޗv�����̎擾
		const int woodValue = GetRacipeValue(item, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(item, eCraftParam::StoneValue);
		const int gearValue = GetRacipeValue(item, eCraftParam::GearValue);

		// �쐬�\���̐^�U��Ԃ�
		return GetCraftPossible(woodValue, stoneValue, gearValue);
	}

	// �N���t�g�E�B���h�E�̌Ăяo��
	void CraftManager::CraftingEnabled(bool enable)
	{
		// QTE�ƃE�B���h�E�I�u�W�F�N�g�̎擾
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		const auto& player = m_player.lock();
		if (qte && window && player)
		{
			// �X�v���C�g�̒��S�ʒu�ݒ�𑗂�
			Vec3 playerPos = player->GetPosition();
			Vec3 windowPos = Utility::ConvertToWorldPosition(player->GetStage()->GetView(), playerPos);
			eRectType rect = eRectType::DownLeft;
			if (windowPos.x < 0.0f) rect = eRectType::DownRight;
			if (windowPos.y < 0.0f) rect = eRectType::UpLeft;
			if (windowPos.x < 0.0f && windowPos.y < 0.0f) rect = eRectType::UpRight;

			// ���_���W��ύX
			qte->SetVerticesRect(rect);
			window->SetVerticesRect(rect);

			// �`�������ݒ�
			qte->SetRectType(rect);
			window->SetRectType(rect);

			// �`���Ԑݒ�𑗂�
			window->SetDrawEnable(enable);
			qte->SetDrawEnable(enable, window->GetPosition());
		}
	}

	// QTE�̊J�n�Ăяo��
	void CraftManager::StartQTE(eCraftItem item, eItemType type)
	{
		// QTE�ƃE�B���h�E�I�u�W�F�N�g�̎擾
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		if (qte && window)
		{
			// qte�I�u�W�F�N�g��QTE�J�n�Ăяo���𑗂�
			qte->StartQTE(window->GetPosition());
			m_craftItem = item;
			m_craftType = type;
		}
	}

	// QTE�̒�~�Ăяo��
	bool CraftManager::StopQTE()
	{
		bool succes = false;

		// qte�ƃI�u�W�F�N�g���擾
		if (auto qte = m_craftQTE.lock())
		{
			// QTE��~�Ăяo����QTE���ʂ̐^�U���擾
			succes = qte->StopQTE();
			
			// ���؂̍쐬���s�Ȃ�f�ނ�������I��
			if (m_craftItem == eCraftItem::Crossing && !succes) return false;

			// �f�ޏ���
			UseItem(eItemType::Wood, GetRacipeValue(m_craftItem, eCraftParam::WoodValue));
			UseItem(eItemType::Stone, GetRacipeValue(m_craftItem, eCraftParam::StoneValue));
			UseItem(eItemType::Gear, GetRacipeValue(m_craftItem, eCraftParam::GearValue));

			// QTE���ʂɉ����č쐬�ʂ�ݒ�
			AddItemCount(m_craftType, GetRacipeValue(m_craftItem, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));
		}

		return succes;
	}

	// QTE�̔j��
	void CraftManager::DestroyCraftQTE()
	{
		m_craftQTE.lock()->StopQTE();
	}
}