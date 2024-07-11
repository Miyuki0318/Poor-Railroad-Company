/*!
@file Shop.cpp
@brief �V���b�v�@�\�̎���
@author �����
*/

#pragma once
#include "stdafx.h"
#include "Shop.h"
#include "ShopCloseState.h"
#include "OzawaUtility.h"

namespace basecross {
	void Shop::OnCreate()
	{
		// �X�e�[�g�}�V���̏�����
		m_ShopState.reset(new ShopStateMachine(GetThis<Shop>()));
		// �ŏ��̃X�e�[�g��ݒ�
		m_ShopState->SetState(ShopCloseState::Instance());

		// ������p�̍ő包�����擾
		m_maxDigit = DemandDigit();
		// �ő包�����̃T�C�Y�Ɋg������
		for (int i = 0; i < m_maxDigit; i++) {
			m_enhancementsCostNumSprite.at(i).resize(m_maxDigit);
		}
		m_enhancementsNextCostSprite.resize(m_maxDigit);
		
		// �X�e�[�W���擾
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �w����ʗp�X�v���C�g�̐ݒ�
		m_purchaseScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_PURCHASESCREEN_TX", WINDOW_SIZE, m_screenDefPosition);
		m_purchaseScreenSprite.lock()->SetDrawLayer(10);
		m_purchaseScreenSprite.lock()->SetDrawActive(false);
		// �I���ꏊ�\���p�X�v���C�g�̐ݒ�
		m_selectPointSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_SELECTPOINT_TX", m_selectPointDefScale, m_selectPointSpritePos.at(m_currentEnhancements));
		m_selectPointSprite.lock()->SetDrawLayer(12);
		m_selectPointSprite.lock()->SetDrawActive(false);
		// �I���ꏊ�\���p�X�v���C�g�̐ݒ�
		for (int i = 0; i < m_notSelectableSprite.size(); i++) {
			m_notSelectableSprite[i] = stagePtr->AddGameObject<Sprite>(L"SHOP_SELECTPOINT_TX", m_selectPointDefScale, m_selectPointSpritePos.at(i));
			m_notSelectableSprite[i].lock()->SetDrawLayer(13);
			m_notSelectableSprite[i].lock()->SetDiffuseColor(Col4(0.5f, 0.5f, 0.5f, 0.5f));
			m_notSelectableSprite[i].lock()->SetDrawActive(false);
		}
		// �w�����e�m�F��ʗp�X�v���C�g�̐ݒ�
		m_confirmationScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CONFIRMATIONSCREEN_TX", WINDOW_SIZE, m_screenDefPosition);
		m_confirmationScreenSprite.lock()->SetDrawLayer(14);
		m_confirmationScreenSprite.lock()->SetDrawActive(false);
		// �������e�u�o�b�N�p�b�N�v�p�X�v���C�g�̐ݒ�
		m_backpackSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_BACKPACK_TX", m_enhancementsContentsScale, m_enhancementsContentsPosition);
		m_backpackSprite.lock()->SetDrawLayer(15);
		m_backpackSprite.lock()->SetDrawActive(false);
		// �������e�u�X�e�[�^�X�v�p�X�v���C�g�̐ݒ�
		m_statusSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_STATUS_TX", m_enhancementsContentsScale, m_enhancementsContentsPosition);
		m_statusSprite.lock()->SetDrawLayer(15);
		m_statusSprite.lock()->SetDrawActive(false);
		// �������e�u�M�A�v�p�X�v���C�g�̐ݒ�
		m_gearSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_GEAR_TX",m_enhancementsContentsScale, m_enhancementsContentsPosition);
		m_gearSprite.lock()->SetDrawLayer(15);
		m_gearSprite.lock()->SetDrawActive(false);
		// �I�����u�͂��v�p�X�v���C�g�̐ݒ�
		m_yesSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_YES_TX", m_confirmSpritedefScale, m_yesSpritePosition);
		m_yesSprite.lock()->SetDrawLayer(15);
		m_yesSprite.lock()->SetDrawActive(false);
		//�I�����u�������v�p�X�v���C�g�̐ݒ�
		m_noSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_NO_TX", m_confirmSpritedefScale, m_noSpritePosition);
		m_noSprite.lock()->SetDrawLayer(15);
		m_noSprite.lock()->SetDrawActive(false);
	}

	void Shop::OnUpdate()
	{
		// �X�e�[�g�}�V���̍X�V�����𑗂�
		m_ShopState->Update();
	}

	int Shop::DemandDigit() {
		// �ő��p
		int maxCost = 0;
		// �ő包��
		int maxDigit = 0;

		// �ő��p���v�Z
		for (int i = 0; i < m_enhancementsCost.size(); i++) {
			for (int j = 0; j < m_enhancementsCost.at(i).size(); j++) {
				// ���݂̍ő��p�Ɣ�r
				if (maxCost < m_enhancementsCost.at(i).at(j)) {
					// �ő��p���X�V
					maxCost = m_enhancementsCost.at(i).at(j);
				}
			}
		}

		// �ő包�����v�Z
		while (maxCost > 1) {
			maxCost /= 10;
			maxDigit++;
		}

		// �ő包����Ԃ�
		return maxDigit;
	}


}