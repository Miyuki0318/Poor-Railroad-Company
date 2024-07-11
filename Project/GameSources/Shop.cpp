/*!
@file Shop.cpp
@brief �V���b�v�@�\�̎���
@author �����
*/

#pragma once
#include "stdafx.h"
#include "Shop.h"
#include "ShopCloseState.h"

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
		for (int i = 0; i < eEnhancements::size; i++) {
			m_enhancementsCostNumSprite.at(i).resize(m_maxDigit);
		}
		m_enhancementsNextCostSprite.resize(m_maxDigit);

		// �I�����̕\���ʒu��ݒ�
		m_selectPointSpritePos = vector<Vec3>{
			Vec3(-179.0f, 181.0f, 0.0f),
			Vec3(-179.0f, -66.0f, 0.0f),
			Vec3(-179.0f, -320.0f, 0.0f)
		};
		 
		// ����Lv�̕\���ʒu��ݒ�
		m_playerLvSpritePos = vector<Vec3>{
			Vec3(-370.0f, 117.0f, 0.0f),
			Vec3(-370.0f, -128.0f, 0.0f),
			Vec3(-370.0f, -383.0f, 0.0f)
		};

		// ������p�̕\���ʒu��ݒ�
		m_enhancementsCostSpritePos = vector<Vec3>{
			Vec3(400.0f, 117.0f, 0.0f),
			Vec3(400.0f, -128.0f, 0.0f),
			Vec3(400.0f, -383.0f, 0.0f)
		};

		// �I�����u�͂��v�u�������v�\���ʒu��ݒ�
		m_confirmSpritePos = vector<Vec3>{
			Vec3(-550.0f, -300.0f, 0.0f), // �I�����u�͂��v
			Vec3(-250.0f, -300.0f, 0.0f) // �I�����u�������v
		};

		// �I�����u�͂��v�u�������v�����X�P�[����ݒ�
		m_confirmSpritedefScale = Vec2(360.0f, 90.0f);

		// ������p�̕\�����W��ݒ�
		m_nextCostSpritePos = Vec3(120.0f + m_maxDigit * m_numberScale.x, -90.0f, 0.0f);

		// ���̃��x���̕\�����W��ݒ�
		m_nextLevelSpritePos = Vec3(-350.0f, -90.0f, 0.0f);

		// �X�e�[�W���擾
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �w����ʗp�X�v���C�g�̐ݒ�
		m_purchaseScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_PURCHASESCREEN_TX", Vec2(1920.0f, 1080.0f), Vec3(-400.0f, 0.0f, 0.0f));
		m_purchaseScreenSprite.lock()->SetDrawLayer(10);
		m_purchaseScreenSprite.lock()->SetDrawActive(false);
		// �I���ꏊ�\���p�X�v���C�g�̐ݒ�
		m_selectPointSprite = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", Vec2(1170.0f, 238.0), m_selectPointSpritePos.at(m_currentEnhancements));
		m_selectPointSprite.lock()->SetDrawLayer(12);
		m_selectPointSprite.lock()->SetDrawActive(false);
		// �I���ꏊ�\���p�X�v���C�g�̐ݒ�
		for (int i = 0; i < m_notSelectableSprite.size(); i++) {
			m_notSelectableSprite[i] = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", Vec2(1170.0f, 238.0), m_selectPointSpritePos.at(i));
			m_notSelectableSprite[i].lock()->SetDrawLayer(13);
			m_notSelectableSprite[i].lock()->SetDiffuseColor(Col4(0.5f, 0.5f, 0.5f, 0.5f));
			m_notSelectableSprite[i].lock()->SetDrawActive(false);
		}
		// �w�����e�m�F��ʗp�X�v���C�g�̐ݒ�
		m_confirmationScreenSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CONFIRMATIONSCREEN_TX", Vec2(1920.0f, 1080.0f), Vec3(-400.0f, 0.0f, 0.0f));
		m_confirmationScreenSprite.lock()->SetDrawLayer(14);
		m_confirmationScreenSprite.lock()->SetDrawActive(false);
		// �������e�u���ޏ�������㏸�v�p�X�v���C�g�̐ݒ�
		m_LimitChoicesSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_LIMITCHOICES_TX", Vec2(875.0f, 125.0f), Vec3(-170.0f, 100.0f, 0.0f));
		m_LimitChoicesSprite.lock()->SetDrawLayer(15);
		m_LimitChoicesSprite.lock()->SetDrawActive(false);
		// �������e�u�̌@/�ړ����x�����v�p�X�v���C�g�̐ݒ�
		m_statusSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_SPEEDCHOICES_TX", Vec2(875.0f, 125.0f), Vec3(-170.0f, 100.0f, 0.0f));
		m_statusSprite.lock()->SetDrawLayer(15);
		m_statusSprite.lock()->SetDrawActive(false);
		// �������e�u���ؑf�ޏ����������v�p�X�v���C�g�̐ݒ�
		m_gearSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_CROSSINGCHOICES_TX", Vec2(875.0f, 125.0f), Vec3(-170.0f, 100.0f, 0.0f));
		m_gearSprite.lock()->SetDrawLayer(15);
		m_gearSprite.lock()->SetDrawActive(false);
		// �I�����u�͂��v�p�X�v���C�g�̐ݒ�
		m_yesSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_YES_TX", m_confirmSpritedefScale, Vec3(-400.0f, -270.0f, 0.0f));
		m_yesSprite.lock()->SetDrawLayer(15);
		m_yesSprite.lock()->SetDrawActive(false);
		//�I�����u�������v�p�X�v���C�g�̐ݒ�
		m_noSprite = stagePtr->AddGameObject<Sprite>(L"SHOP_NO_TX", m_confirmSpritedefScale, Vec3(80.0f, -270.0f, 0.0f));
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
		while (maxCost > 0) {
			maxDigit++;
			maxCost /= 10;
		}

		// �ő包����Ԃ�
		return maxDigit;
	}


}