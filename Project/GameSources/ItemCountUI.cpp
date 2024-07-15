/*!
@file ItemCountUI.cpp
@brief �A�C�e���̃J�E���^UI
@author ���V���M
*/

#include "stdafx.h"
#include "ItemCountUI.h"
#include "GamePlayer.h"

namespace basecross
{
	// �������̏���
	void ItemCountUI::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage(); // �X�e�[�W�̎擾

		// �X�v���C�g�̐���
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"UI_MENUBACK_TX", m_backScale, m_startPos + m_spriteMargin + BACK_LAYER);
		m_itemSprite = stagePtr->AddGameObject<Sprite>(m_texKey, Vec2(m_spriteScale), Vec3(m_startPos));
		m_slashSprite = stagePtr->AddGameObject<Sprite>(L"SLASH_TX", Vec2(m_limitScale), Vec3(m_startPos + m_limitPos + m_spriteMargin + m_numberMargin + m_numberMargin));
		
		// �����p�X�v���C�g�̐���
		m_itemNums = make_pair(
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + m_numberMargin)),
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin))
		);
		m_limitNums = make_pair(
			stagePtr->AddGameObject<Number>(Vec2(m_limitScale), Vec3(m_startPos + m_limitPos + m_spriteMargin + m_numberMargin + m_limitMargin + m_limitMargin + m_limitMargin)),
			stagePtr->AddGameObject<Number>(Vec2(m_limitScale), Vec3(m_startPos + m_limitPos + m_spriteMargin + m_numberMargin + m_numberMargin + m_limitMargin))
		);

		// �f�B�q���[�Y�F�ŐF��ݒ�
		m_itemNums.first.lock()->SetDiffuseColor(COL_BLACK);
		m_itemNums.second.lock()->SetDiffuseColor(COL_BLACK);
		m_limitNums.first.lock()->SetDiffuseColor(COL_RED);
		m_limitNums.second.lock()->SetDiffuseColor(COL_RED);

		// ���C���[�ݒ�
		m_backSprite.lock()->SetDrawLayer(4);
		m_itemSprite.lock()->SetDrawLayer(4);
		m_slashSprite.lock()->SetDrawLayer(4);
		m_backSprite.lock()->SetDrawLayer(4);
		m_itemNums.first.lock()->SetDrawLayer(4);
		m_itemNums.second.lock()->SetDrawLayer(4);
		m_limitNums.first.lock()->SetDrawLayer(4);
		m_limitNums.second.lock()->SetDrawLayer(4);
	}

	// ���t���[���X�V
	void ItemCountUI::OnUpdate()
	{
		// �����̍X�V���s��
		UpdateNumbers();
	}

	// �����̍X�V����
	void ItemCountUI::UpdateNumbers()
	{
		// �v���C���[����A�C�e�����Ə�������擾
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		int count = player->GetItemCount(m_itemType);
		int limit = player->GetLimitCount(m_itemType);

		// �����X�v���C�g�̕\�����鐔�����X�V
		m_itemNums.first.lock()->SetNumber(ONE_PLACE(count));
		m_itemNums.second.lock()->SetNumber(TENS_PLACE(count));
		m_limitNums.first.lock()->SetNumber(ONE_PLACE(limit));
		m_limitNums.second.lock()->SetNumber(TENS_PLACE(limit));

		// ����l�ɒB���Ă���ԐF�ɂ��A����Ȃ���΍��F�ɂ���
		bool isLimit = count >= limit;
		m_itemNums.first.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
		m_itemNums.second.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
	}
}