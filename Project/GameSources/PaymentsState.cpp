/*!
@file PaymentsState.cpp
@brief �J���������̎������Z�\���X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GamePlayer.h"
#include "PaymentsState.h"

namespace basecross
{
	// �X�e�[�g�������̏���
	void PaymentsState::CreateState()
	{
		// �X�v���C�g�̐���
		const auto& stagePtr = m_stage.lock();
		m_menuSprite = stagePtr->AddGameObject<Sprite>(L"PAYMENTS_MENU_TX", WINDOW_SIZE, Vec3(0.0f, WINDOW_HEIGHT, 0.0f) + m_menuPosition);
		m_menuSprite.lock()->SetDrawLayer(8);

		// �����X�v���C�g�̐���
		NumberCount reward, rSales, gold, resource, rInstallations, total;
		for (int i = 0; i < 5; i++)
		{
			float diff = float(5 - i);
			reward.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff))));
			rSales.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 1.0f))));
			gold.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 2.0f))));
			resource.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 3.0f))));
			rInstallations.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 4.0f))));
			total.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + m_totalMargin + (m_numberMarginX * diff) + (m_numberMarginY * 5.0f))));
		}

		// �}�b�v�ɐ����\���̂�o�^
		m_numbersMap.emplace(eGamePaymentsState::RewardCount, reward);
		m_numbersMap.emplace(eGamePaymentsState::RailsSales, rSales);
		m_numbersMap.emplace(eGamePaymentsState::GoldBarSales, gold);
		m_numbersMap.emplace(eGamePaymentsState::ResourceSales, resource);
		m_numbersMap.emplace(eGamePaymentsState::RailsInstallations, rInstallations);
		m_numbersMap.emplace(eGamePaymentsState::TotalIncome, total);

		// �S�Ă̐����X�v���C�g�𓧖���
		for (auto& numbers : m_numbersMap)
		{
			for (auto& number : numbers.second.sprite)
			{
				number.lock()->SetDrawLayer(8);
				number.lock()->SetDiffuseColor(COL_ALPHA);
			}
		}
	}

	// �X�e�[�g�̍X�V
	void PaymentsState::UpdateState()
	{
		m_stateFunc.at(m_state)();
	}

	// �X�e�[�g�̏�����
	void PaymentsState::ResetState()
	{
		// �X�e�[�g�����j���[�t�F�[�h�ɂ��A�X�v���C�g���J�n���̍��W��
		m_state = eGamePaymentsState::MenuFadeIn;
		m_menuSprite.lock()->SetStartPosition();

		// �S�Ă̐����X�v���C�g��0�ԁE�����E�J�n���̍��W��
		for (auto& numbers : m_numbersMap)
		{
			for (auto& number : numbers.second.sprite)
			{
				number.lock()->SetNumber(0);
				number.lock()->SetDiffuseColor(COL_ALPHA);
				number.lock()->SetStartPosition();
			}
		}
	}

	// ���j���[�t�F�[�h�C��
	void PaymentsState::MenuFadeInState()
	{
		// �V�F�A�h�|�C���^��
		auto& menu = m_menuSprite.lock();

		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(menu->GetStartPosition(), m_menuPosition, m_fadeTotalTime / m_menuFadeTime);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_fadeTotalTime >= m_menuFadeTime)
		{
			menu->StartSE(L"COIN_SE", 1.0f);
			m_state = eGamePaymentsState::RewardCount;
			m_fadeTotalTime = 0.0f;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_fadeTotalTime += DELTA_TIME;
		menu->SetPosition(pos);
	}

	// �J����V�\��
	void PaymentsState::RewardCountState()
	{
		// �����\���̂��擾���A�\�������𑗂�
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// �������I�������X�e�[�g��؂�ւ���
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::RailsSales;
		}
	}

	// ���[�����p�����\��
	void PaymentsState::RailsSalesState()
	{
		// �����\���̂��擾���A�\�������𑗂�
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// �������I�������X�e�[�g��؂�ւ���
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::GoldBarSales;
		}
	}

	// ���򔄋p�����\��
	void PaymentsState::GoldBarSalesState()
	{
		// �����\���̂��擾���A�\�������𑗂�
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// �������I�������X�e�[�g��؂�ւ���
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::ResourceSales;
		}
	}

	// �������p�����\��
	void PaymentsState::ResourceSalesState()
	{
		// �����\���̂��擾���A�\�������𑗂�
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// �������I�������X�e�[�g��؂�ւ���
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::RailsInstallations;
		}
	}

	// ���[���ݒu��p�\��
	void PaymentsState::RailsInstallationsState()
	{
		// �����\���̂��擾���A�\�������𑗂�
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// �������I�������X�e�[�g��؂�ւ���
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::TotalIncome;
		}
	}

	// ���v�����\��
	void PaymentsState::TotalIncomeState()
	{
		// �����\���̂��擾���A�\�������𑗂�
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// �������I�������X�e�[�g��؂�ւ���
		if (numbers.degit < 0)
		{
			// �R�C��SE���~�߁A�w��SE���Đ�
			m_menuSprite.lock()->StopSE(L"COIN_SE");
			m_menuSprite.lock()->StartSE(L"BUY_SE", 2.0f);
			m_state = eGamePaymentsState::DrawStandBy;
		}
	}

	// �ς邽�߂̕\���ҋ@
	void PaymentsState::DrawStandByState()
	{
		// �ς邽�߂̎��Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_fadeTotalTime >= m_standByTime)
		{
			m_state = eGamePaymentsState::MenuFadeOut;
			m_fadeTotalTime = 0.0f;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_fadeTotalTime += DELTA_TIME;
	}

	// ���j���[�t�F�[�h�A�E�g
	void PaymentsState::MenuFadeOutState()
	{
		// �V�F�A�h�|�C���^��
		auto& menu = m_menuSprite.lock();

		// ���`��Ԃňړ�
		float ratio = m_fadeTotalTime / m_menuFadeTime;
		Vec3 pos = Utility::Lerp(m_menuPosition, menu->GetStartPosition(), ratio);
		Vec3 move = Utility::Lerp(Vec3(0.0f), Vec3(0.0f, WINDOW_HEIGHT, 0.0f), ratio);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_fadeTotalTime >= m_menuFadeTime)
		{
			m_state = eGamePaymentsState::StandBy;
			m_fadeTotalTime = 0.0f;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_fadeTotalTime += DELTA_TIME;
		menu->SetPosition(pos);
		for (auto& numbers : m_numbersMap)
		{
			MoveNumbersPosition(numbers.second, move);
		}
	}

	// �����\���̂̐����X�v���C�g�ړ�����
	void PaymentsState::MoveNumbersPosition(const NumberCount& count, const Vec3& move)
	{
		for (auto& number : count.sprite)
		{
			auto& ptr = number.lock();

			// �J�n���̍��W�Ɉړ��ʂ𑫂��č��W���X�V
			ptr->SetPosition(ptr->GetStartPosition() + move);
		}
	}
}