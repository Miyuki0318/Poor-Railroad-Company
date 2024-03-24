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
	void CraftManager::Crafting(const shared_ptr<TemplateObject>& tempPtr)
	{
		// �N���t�g�A�C�R��������I�������擾����@�\�����łŒǉ�
		// �܂�QTE�������ׁA����͌Œ�łP�N���t�g�ł�����

		// �^�L���X�g
		const auto& playerPtr = dynamic_pointer_cast<Player>(tempPtr);
		if (!playerPtr) return;

		// �ȈՎ���
		const int woodValue = GetRacipeValue(eCraftItem::Rail, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(eCraftItem::Rail, eCraftParam::StoneValue);

		// �f�ޗv�����ɑ���ĂȂ���ΏI��
		if (woodValue > playerPtr->GetItemCount(eItemType::Wood)) return;
		if (stoneValue > playerPtr->GetItemCount(eItemType::Stone)) return;

		// �f�ޏ���ƃN���t�g
		playerPtr->AddItemCount(eItemType::Wood, -woodValue);
		playerPtr->AddItemCount(eItemType::Stone, -stoneValue);
		playerPtr->AddItemCount(eItemType::Rail, GetRacipeValue(eCraftItem::Rail, eCraftParam::SuccesValue));
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
}