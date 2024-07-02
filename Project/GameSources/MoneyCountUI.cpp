/*!
@file MoneyCountUI.cpp
@brief �������̃J�E���^UI
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void MoneyCountUI::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage(); // �X�e�[�W�̎擾

		// �X�v���C�g�̐���
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"UI_MENUBACK_TX", m_backScale, m_startPos + m_backMargin + BACK_VEC);
		m_itemSprite = stagePtr->AddGameObject<Sprite>(L"UI_COIN_TX", Vec2(m_spriteScale), Vec3(m_startPos));

		// �����X�v���C�g�̐���
		for (int i = 0; i < 5; i++)
		{
			auto& number = stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + (m_numberMargin * float(i))));
			number->SetDiffuseColor(COL_BLACK);
			m_numbers.push_back(number);
		}
	}

	// ���t���[���X�V
	void MoneyCountUI::OnUpdate()
	{
		// �����̍X�V����
		UpdateNumbers();
	}

	// �����̕\���X�V
	void MoneyCountUI::UpdateNumbers()
	{
		// �V�[�����珊�������擾
		const auto& scene = App::GetApp()->GetScene<Scene>();
		int money = scene->GetMoney();

		// ����ɂȂ��ĂȂ����̐^�U
		bool isLimit = (money >= MONEY_LIMIT);

		// �����̐ݒ菈��
		int place = 10000;
		for (auto& number : m_numbers)
		{
			number.lock()->SetNumber(money / place % 10);
			number.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
			place = place / 10;
		}
	}
}