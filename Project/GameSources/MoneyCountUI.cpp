/*!
@file MoneyCountUI.cpp
@brief �������̃J�E���^UI
@author ���V���M
*/

#include "stdafx.h"
#include "MoneyCountUI.h"
#include "Scene.h"
#include "BaseStage.h"

namespace basecross
{
	// �������̏���
	void MoneyCountUI::OnCreate()
	{
		const auto& stagePtr = GetStage(); // �X�e�[�W�̎擾

		// �X�v���C�g�̐���
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"UI_MENUBACK_TX", m_backScale, m_startPos + m_backMargin + BACK_LAYER);
		m_itemSprite = stagePtr->AddGameObject<Sprite>(L"UI_COIN_TX", Vec2(m_spriteScale), Vec3(m_startPos));

		// �����X�v���C�g�̐���
		Vec3 margin = Vec3(m_numberMargin * 4);
		for (int i = 0; i < 5; i++)
		{
			auto& number = stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + margin - (m_numberMargin * float(i))));
			number->SetDiffuseColor(COL_ALPHA);
			m_numbers.sprite.push_back(number);
		}
		
		// �J�n���̕\�����s��
		StartSetNumbers();
	}

	// ���t���[���X�V
	void MoneyCountUI::OnUpdate()
	{
		// �����̍X�V����
		if (!m_isGoal) UpdateNumbers();

		// �F�̍X�V����
		UpdateColor();
	}

	void MoneyCountUI::StartSetNumbers()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		int money = scene->GetMoney();
		m_numbers.goal = money;
		m_numbers.degit = max(Utility::GetUpperDigit(money) - 1, 0);
		m_numbers.index = m_numbers.degit;

		while (m_numbers.degit >= 0)
		{
			// �J�E���g�_�E���\��
			NumberCount::ConsecutiveNumberDraw(m_numbers, m_totalTime);
		}
	}

	// �����̕\���X�V
	void MoneyCountUI::UpdateNumbers()
	{
		// �J�E���g�_�E���\��
		NumberCount::ConsecutiveNumberDraw(m_numbers, m_totalTime);

		// �\�����I�������
		if (m_numbers.degit < 0)
		{
			// SE���~�߂�
			m_isGoal = true;
		}
	}

	// �F�̊m�F
	void MoneyCountUI::UpdateColor()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		bool isLimit = scene->GetMoney() >= MONEY_LIMIT;

		for (auto& number : m_numbers.sprite)
		{
			number.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
		}
	}
}