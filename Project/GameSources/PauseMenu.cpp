/*!
@file PauseMenu.cpp
@brief �|�[�Y���j���[
@prod ��I�t
*/

#include "stdafx.h"
#include "PauseMenu.h"
#include "MathFunc.h"
#include "Input.h"
#include "GameStage.h"

namespace basecross
{
	using namespace MathF;

	void PauseMenu::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �e��X�v���C�g�̐���
		m_menuSprites.emplace(eMenuTypes::KeyBoard, stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_K_TX", m_CloseMenuScale, m_CloseMenuPos));
		m_menuSprites.emplace(eMenuTypes::XBoxPad, stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_X_TX", m_CloseMenuScale, m_CloseMenuPos));
		m_buttonSprites.emplace(eButtons::Retry, stagePtr->AddGameObject<Sprite>(L"CONTINUE_TX", m_DefaultButtonScale, m_DefaultRetryButtonPos));
		m_buttonSprites.emplace(eButtons::BackTitle, stagePtr->AddGameObject<Sprite>(L"CONTINUE_TITLEBACK_TX", m_DefaultButtonScale, m_DefaultTitleButtonPos));

		// �`�惌�C���[�ݒ�
		for (int i = 0; i < eMenuTypes::MenuNum; i++)
		{
			m_menuSprites.at((eMenuTypes)i).lock()->SetDrawLayer(m_MenuLayerNum);
		}
		for (int i = 0; i < eButtons::ButtonNum; i++)
		{
			m_buttonSprites.at((eButtons)i).lock()->SetDrawLayer(m_ButtonLayerNum);
		}

		SetDrawActiveButtons(false);
	}

	void PauseMenu::OnUpdate()
	{
		MenuTypeDecision(); // ���j���[�^�C�v����
		SetDrawActiveMenu(true, m_currentMenuType); // ���݂̃��j���[�^�C�v�̃X�v���C�g��\��
		StateProcess(m_state); // ��Ԃ��Ƃ̏���
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
		m_menuSprites.at(m_currentMenuType).lock()->SetPosition(Utility::Lerp(m_CloseMenuPos, m_OpenMenuPos, m_lerpRatio));
		m_menuSprites.at(m_currentMenuType).lock()->SetScale(Utility::Lerp(m_CloseMenuScale, m_OpenMenuScale, m_lerpRatio));

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

	void PauseMenu::MenuTypeDecision()
	{
		// �Q�[���p�b�h���ڑ�����Ă�����
		if (Input::GetPadConected())
		{
			m_currentMenuType = XBoxPad; // ���j���[�^�C�v��XBOX��
		}
		else // �����łȂ����
		{
			m_currentMenuType = KeyBoard; // ���j���[�^�C�v���L�[�{�[�h/�}�E�X��
		}
		// �O�t���[���ƌ��t���[���̃��j���[�^�C�v������Ă���΁A�\�����Ă������j���[���\���ɂ���
		if (m_pastMenuType != m_currentMenuType) SetDrawActiveMenu(false, m_pastMenuType);

		m_pastMenuType = m_currentMenuType;
	}

	void PauseMenu::ButtonSelect()
	{
		float stickVal = Input::GetLStickValue().x; // �X�e�B�b�N���͂��擾

		m_scaleRatio += DELTA_TIME * m_ButtonScaleSpeed;
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

	void PauseMenu::SetDrawActiveMenu(bool drawFlag, eMenuTypes mType)
	{
		m_menuSprites.at(mType).lock()->SetDrawActive(drawFlag);
	}
}