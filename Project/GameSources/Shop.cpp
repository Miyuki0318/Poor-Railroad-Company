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
		for (int i = 0; i < eEnhancements::EnhancementsSize; i++) {
			m_enhancementsCostNumSprite.at(i).resize(m_maxDigit);
		}
		m_enhancementsNextCostSprite.resize(m_maxDigit);

		// ������p�̕\�����W���ő包�������炷�ݒ�
		for (int i = 0; i < m_enhancementsCostSpritePos.size(); i++) {
			m_enhancementsCostSpritePos.at(i).x += m_maxDigit * m_numberScale.x;
		}
		m_nextCostSpritePos.x += m_maxDigit * m_numberScale.x;

		// �X�e�[�W���擾
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �w����ʗp�X�v���C�g�̐ݒ�
		m_purchaseScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_PURCHASESCREEN_TX", WINDOW_SIZE, m_screenSpritePos);
		m_purchaseScreenSprite.lock()->SetDrawLayer(5);
		m_purchaseScreenSprite.lock()->SetDrawActive(false);
		// �I���ꏊ�\���p�X�v���C�g�̐ݒ�
		m_selectPointSprite = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", m_selectPointScale, m_selectPointSpritePos.at(m_currentEnhancements));
		m_selectPointSprite.lock()->SetDrawLayer(7);
		m_selectPointSprite.lock()->SetDrawActive(false);
		// �����s�I�����O���[�A�E�g�p�X�v���C�g�̐ݒ�
		for (int i = 0; i < m_notSelectableSprite.size(); i++) {
			m_notSelectableSprite.at(i) = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", m_selectPointScale, m_selectPointSpritePos.at(i));
			m_notSelectableSprite.at(i).lock()->SetDrawLayer(8);
			m_notSelectableSprite.at(i).lock()->SetDiffuseColor(Col4(0.5f, 0.5f, 0.5f, 0.5f));
			m_notSelectableSprite.at(i).lock()->SetDrawActive(false);
		}
		// �A�C�R���p�X�v���C�g�̐ݒ�
		m_iconSprite.at(eOperationType::Controller) = stagePtr->AddGameObject<Sprite>(L"ABUTTON_TX", m_iconScale, m_iconPos);
		m_iconSprite.at(eOperationType::Controller).lock()->SetDrawLayer(8);
		m_iconSprite.at(eOperationType::Controller).lock()->SetDrawActive(false);
		m_iconSprite.at(eOperationType::Keyboard) = stagePtr->AddGameObject<Sprite>(L"RCLICK_TX", m_iconScale, m_iconPos);
		m_iconSprite.at(eOperationType::Keyboard).lock()->SetDrawLayer(8);
		m_iconSprite.at(eOperationType::Keyboard).lock()->SetDrawActive(false);
		// �w�����e�m�F��ʗp�X�v���C�g�̐ݒ�
		m_confirmationScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CONFIRMATIONSCREEN_TX", WINDOW_SIZE, m_screenSpritePos);
		m_confirmationScreenSprite.lock()->SetDrawLayer(9);
		m_confirmationScreenSprite.lock()->SetDrawActive(false);
		// �������e�u���ޏ�������㏸�v�p�X�v���C�g�̐ݒ�
		m_LimitChoicesSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_LIMITCHOICES_TX", m_enhancementsContentsScale, m_enhancementsContentsPos);
		m_LimitChoicesSprite.lock()->SetDrawLayer(10);
		m_LimitChoicesSprite.lock()->SetDrawActive(false);
		// �������e�u�X�e�[�^�X�v�p�X�v���C�g�̐ݒ�
		m_statusSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_SPEEDCHOICES_TX", m_enhancementsContentsScale, m_enhancementsContentsPos);
		m_statusSprite.lock()->SetDrawLayer(10);
		m_statusSprite.lock()->SetDrawActive(false);
		// �������e�u���ؑf�ޏ����������v�p�X�v���C�g�̐ݒ�
		m_gearSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CROSSINGCHOICES_TX",m_enhancementsContentsScale, m_enhancementsContentsPos);
		m_gearSprite.lock()->SetDrawLayer(10);
		m_gearSprite.lock()->SetDrawActive(false);
		// �I�����X�v���C�g�̐ݒ�
		m_confirmSprite.emplace(eConfirmState::Yes, stagePtr->AddGameObject<Sprite>(L"SHOP_YES_TX", m_confirmSpriteScale, m_confirmSpritePos.at(eConfirmState::Yes)));
		m_confirmSprite.at(eConfirmState::Yes).lock()->SetDrawLayer(10);
		m_confirmSprite.at(eConfirmState::Yes).lock()->SetDrawActive(false);
		m_confirmSprite.emplace(eConfirmState::No, stagePtr->AddGameObject<Sprite>(L"SHOP_NO_TX", m_confirmSpriteScale, m_confirmSpritePos.at(eConfirmState::No)));
		m_confirmSprite.at(eConfirmState::No).lock()->SetDrawLayer(10);
		m_confirmSprite.at(eConfirmState::No).lock()->SetDrawActive(false);
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
		while (maxCost > 0) {
			maxDigit++;
			maxCost /= 10;
		}

		// �ő包����Ԃ�
		return maxDigit;
	}


}