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
	bool CraftManager::Crafting(eCraftItem item)
	{
		// �f�ޗv�����̎擾
		const int woodValue = GetRacipeValue(item, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(item, eCraftParam::StoneValue);

		// �쐬�\���̐^�U��Ԃ�
		return GetCraftPossible(woodValue, stoneValue);
	}

	// �N���t�g�E�B���h�E�̌Ăяo��
	void CraftManager::CraftingEnabled(bool enable)
	{
		// �E�B���h�E�I�u�W�F�N�g�ɕ`��Ăяo���𑗂�
		const auto& window = m_window.lock();
		if (window)
		{
			// �E�B���h�E�̕����ƕ`���Ԑݒ�𑗂�(�������ݒ肾�����U�E��Œ�)
			window->SetWindowRect(eWindowRect::UpperRight);
			window->SetDrawEnable(enable);
		}
	}

	// QTE�̊J�n�Ăяo��
	void CraftManager::StartQTE()
	{
		// qte�I�u�W�F�N�g��QTE�J�n�Ăяo���𑗂�
		const auto& qte = m_craftQTE.lock();
		if (qte)
		{
			qte->StartQTE();
		}
	}

	// QTE�̒�~�Ăяo��
	void CraftManager::StopQTE(eCraftItem item)
	{
		// qte�I�u�W�F�N�g���擾
		const auto& qte = m_craftQTE.lock();
		if (qte)
		{
			// QTE��~�Ăяo����QTE���ʂ̐^�U���擾
			bool succes = qte->StopQTE();

			// �f�ޏ���
			AddItemCount(eItemType::Wood, -GetRacipeValue(item, eCraftParam::WoodValue));
			AddItemCount(eItemType::Stone, -GetRacipeValue(item, eCraftParam::StoneValue));

			// QTE���ʂɉ����č쐬�ʂ�ݒ�
			AddItemCount(eItemType::Rail, GetRacipeValue(item, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));
		}
	}
}