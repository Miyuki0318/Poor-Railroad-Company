/*!
@file PauseMenu.cpp
@brief �|�[�Y���j���[
@prod ��I�t
*/

#include "stdafx.h"
#include "PauseMenu.h"
#include "MathFunc.h"
#include "GameStage.h"

namespace basecross
{
	using namespace MathF;

	void PauseMenu::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �e��X�v���C�g�̐���
		m_menuSprite = stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_TX", m_CloseMenuScale, m_CloseMenuPos);
		m_menuSprite.lock()->SetDrawLayer(8);
		m_buttonSprites.emplace(eButtons::Retry, stagePtr->AddGameObject<Sprite>(L"CONTINUE_TX", m_DefaultButtonScale, m_DefaultRetryButtonPos));
		m_buttonSprites.emplace(eButtons::BackTitle, stagePtr->AddGameObject<Sprite>(L"CONTINUE_TITLEBACK_TX", m_DefaultButtonScale, m_DefaultTitleButtonPos));
		m_buttonSprites.at(eButtons::Retry).lock()->SetDrawLayer(9);
		m_buttonSprites.at(eButtons::BackTitle).lock()->SetDrawLayer(9);

		SetDrawActiveButtons(false);
	}

	void PauseMenu::OnUpdate()
	{
		StateProcess(m_state);
	}

	bool PauseMenu::OnOpen()
	{
		if (m_state != State::Closed) return false; // ��Ԃ��N���[�Y��łȂ���Ή������Ȃ�
		m_state = State::Open; // �I�[�v����ԂɈڍs
		GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::Pause); // �Q�[���i�s��Ԃ��|�[�Y��Ԃɂ���
		m_lerpRatio = 0.0f; // ��Ԋ�����0��
		return true;
	}

	bool PauseMenu::OnClose()
	{
		if (m_state != State::Opened) return false; // ��Ԃ��I�[�v����łȂ���Ή������Ȃ�

		SetDrawActiveButtons(false); // �{�^�����\��
		m_state = State::Close;	// �N���[�Y��ԂɈڍs
		GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::Playing); // �Q�[���i�s��Ԃ��v���C��Ԃɂ���
		m_lerpRatio = 1.0f; // ��Ԋ�����1��
		return true;
	}

	void PauseMenu::StateProcess(State state)
	{
		m_lerpRatio = Clamp01(m_lerpRatio); // ��Ԋ�����0~1�͈̔͂ɐ�������

		// ���j���[�X�v���C�g�̃g�����X�t�H�[���ݒ�
		m_menuSprite.lock()->SetPosition(Utility::Lerp(m_CloseMenuPos, m_OpenMenuPos, m_lerpRatio)); 
		m_menuSprite.lock()->SetScale(Utility::Lerp(m_CloseMenuScale, m_OpenMenuScale, m_lerpRatio));

		if (state == State::Open) // �I�[�v����Ԃ̎�
		{
			m_lerpRatio += DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio >= 1.0f ? State::Opened : State::Open;
		}
		if (state == State::Close) // �N���[�Y��Ԃ̎�
		{
			m_lerpRatio -= DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio <= 0.0f ? State::Closed : State::Close;
		}
		if (state == State::Opened) // �I�[�v�����Ԃ̎�
		{
			SetDrawActiveButtons(true); // �{�^����\��
			ButtonSelect(); 
		}
		if (state == State::Selected) // �{�^���Z���N�g���Ԃ̎�
		{
			if (m_currentButton == eButtons::BackTitle) // �^�C�g���ɖ߂�{�^�����I�΂�Ă�����
			{
				TitleButton(); // �^�C�g���{�^���p�������s��
				return;
			}

			if (GetTypeStage<GameStage>()->GetFadeIn() && m_currentButton == eButtons::Retry) // �t�F�[�h�C�����I����������g���C�{�^�����I�΂�Ă�����
			{
				RetryButton(); // ���g���C�{�^���p�������s��
			}
		}
	}

	void PauseMenu::ButtonSelect()
	{
		float stickVal = Input::GetLStickValue().x; // �X�e�B�b�N���͂��擾

		m_scaleRatio += DELTA_TIME * m_ScaleSpeed; // �{�^���̃X�P�[���p
		if (m_scaleRatio >= XM_2PI) m_scaleRatio = 0.0f;

		if (stickVal && !m_pastStickVal) // ���͂����邩���O�ɃX�e�B�b�N���͂�����������
		{
			// �I�𒆂̃{�^����ύX����
			m_scaleRatio = 0.0f;
			m_pastButton = m_currentButton;
			switch (m_currentButton)
			{
			case eButtons::Retry:
				m_currentButton = eButtons::BackTitle;
				break;

			case eButtons::BackTitle:
				m_currentButton = eButtons::Retry;
				break;

			default:
				break;
			}
		}

		m_pastStickVal = stickVal;

		// �I�𒆂̃{�^���̂݃X�P�[�����T�C���J�[�u�ŕύX����
		float scale = Utility::SinCurve(m_scaleRatio, m_MinScaleRatio, m_MaxScaleRatio); 
		m_buttonSprites.at(m_pastButton).lock()->SetScale(m_DefaultButtonScale);
		m_buttonSprites.at(m_currentButton).lock()->SetScale(m_DefaultButtonScale * scale);

		if (Input::GetPushB()) m_state = State::Selected; // B�{�^���������ꂽ��Selected��ԂɈڍs
	}

	void PauseMenu::TitleButton()
	{
		GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::ToTitleClear); // �Q�[���i�s��Ԃ��^�C�g���ɖ߂��ԂɈڍs
	}

	void PauseMenu::RetryButton()
	{
		auto& gameStage = GetTypeStage<GameStage>();
		gameStage->ResetCreateStage();	// �X�e�[�W�����Z�b�g
		gameStage->SetGameProgress(eGameProgress::FadeIn); // �Q�[���i�s��Ԃ��v���C��ԂɈڍs
		SetDrawActiveButtons(false); // �{�^�����\��
		m_state = State::Close; // �N���[�Y��ԂɈڍs
	}

	void PauseMenu::SetDrawActiveButtons(bool drawFlag)
	{
		// �{�^���̐�����SetDrawActive���s��
		for (int i = 0; i < eButtons::ButtonNum; i++)
		{
			m_buttonSprites.at((eButtons)i).lock()->SetDrawActive(drawFlag);
		}
	}
}