/*!
@file CraftManager.cpp
@brief �N���t�g�Ǘ��p�}�l�[�W��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �A�C�e���N���t�g
	bool CraftManager::CraftOrder()
	{
		// eCraftItem item = m_selectIcon.lock()->GetSelectItem();
		eCraftItem item = eCraftItem::Rail;

		// �f�ޗv�����̎擾
		const int woodValue = GetRacipeValue(item, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(item, eCraftParam::StoneValue);

		// �쐬�\���̐^�U��Ԃ�
		return GetCraftPossible(woodValue, stoneValue);
	}

	// �N���t�g�E�B���h�E�̌Ăяo��
	void CraftManager::CraftingEnabled(bool enable)
	{
		// QTE�ƃE�B���h�E�I�u�W�F�N�g�̎擾
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		if (qte && window)
		{
			// �`���Ԑݒ�𑗂�
			qte->SetDrawEnable(enable, window->GetPosition());
			window->SetDrawEnable(enable);

			// �X�v���C�g�̒��S�ʒu�ݒ�𑗂�(�������ݒ肾�����U�E��Œ�)
			//eVerticesRect rect = GetCraftWindowDrawRect();
			eVerticesRect rect = eVerticesRect::UpperRight;
			qte->SetVerticesRect(rect);
			window->SetVerticesRect(rect);
		}
	}

	// QTE�̊J�n�Ăяo��
	void CraftManager::StartQTE()
	{
		// QTE�ƃE�B���h�E�I�u�W�F�N�g�̎擾
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		if (qte && window)
		{
			// qte�I�u�W�F�N�g��QTE�J�n�Ăяo���𑗂�
			qte->StartQTE(window->GetPosition());
		}
	}

	// QTE�̒�~�Ăяo��
	void CraftManager::StopQTE()
	{
		// qte�ƑI���A�C�R���I�u�W�F�N�g���擾
		const auto& qte = m_craftQTE.lock();
		//const auto& selectIcon = m_selectIcon.lock();
		//if (qte && selectIcon)
		if (qte)
		{
			// eCraftItem item = m_selectIcon.lock()->GetSelectItem();
			eCraftItem item = eCraftItem::Rail;

			// QTE��~�Ăяo����QTE���ʂ̐^�U���擾
			bool succes = qte->StopQTE();

			// �f�ޏ���
			UseItem(eItemType::Wood, GetRacipeValue(item, eCraftParam::WoodValue));
			UseItem(eItemType::Stone, GetRacipeValue(item, eCraftParam::StoneValue));

			// QTE���ʂɉ����č쐬�ʂ�ݒ�
			AddItemCount(eItemType::Rail, GetRacipeValue(item, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));
		}
	}
}