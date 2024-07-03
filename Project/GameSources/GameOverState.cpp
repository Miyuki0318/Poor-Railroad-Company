/*!
@file GameOverState.cpp
@brief �Q�[���I�[�o�[���̏����X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void GameOverState::CreateState()
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = m_stage.lock();

		// �Q�[���I�[�o�[���Ɉ����X�v���C�g
		m_continueSprite = stagePtr->AddGameObject<Sprite>(L"OVER_CONTINUE_TX", m_defScale, m_leftPos + m_moveVal);
		m_titleBackSprite = stagePtr->AddGameObject<Sprite>(L"OVER_TITLEBACK_TX", m_defScale, m_rightPos + m_moveVal);
		m_railLineSprite = stagePtr->AddGameObject<Sprite>(L"RAIL_LINE_TX", Vec2(WINDOW_WIDTH, 128.0f), m_railPos);

		// ���̃G�t�F�N�g�X�v���C�g���擾
		m_smoke = stagePtr->GetSharedGameObject<SpriteParticle>(L"SmokeEffect");

		// �I�����}�b�v�ɒǉ�
		m_selectSprite.emplace(eSelectGameOver::Continue, m_continueSprite);
		m_selectSprite.emplace(eSelectGameOver::TitleBack, m_titleBackSprite);
	}

	// �X�V����
	void GameOverState::UpdateState()
	{
		// �X�e�[�g�ɉ������֐��Ăяo��
		m_stateFunc.at(m_currentState)();
	}

	// ����������
	void GameOverState::ResetState()
	{
		m_totalTime = 0.0f;
		m_currentState = eGameOverState::RailFadeIn;

		// �������̍��W�ɖ߂�
		m_continueSprite.lock()->SetPosition(m_leftPos + m_moveVal);
		m_titleBackSprite.lock()->SetPosition(m_rightPos + m_moveVal);
		m_railLineSprite.lock()->SetPosition(m_railPos);
	}

	// ���[���̃t�F�[�h�C��
	void GameOverState::RailSpriteFadeIn()
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_railPos, m_railPos - m_moveVal, m_totalTime / m_railFadeTime);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_totalTime >= m_railFadeTime)
		{
			m_currentState = eGameOverState::SelectFadeIn;
			m_totalTime = 0.0f;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_railLineSprite.lock()->SetPosition(pos);
	}

	// �I�����X�v���C�g�̃t�F�[�h�C��
	void GameOverState::SelectSpriteFadeIn()
	{
		// �C�[�W���O�����ňړ�
		Easing<Vec3> moveEasing;
		Vec3 move = moveEasing.EaseOut(EasingType::Cubic, m_moveVal, Vec3(0.0f), m_totalTime, m_selectFadeTime);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_totalTime >= m_selectFadeTime)
		{
			m_currentState = eGameOverState::SelectState;
			m_totalTime = -XM_PIDIV4;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_continueSprite.lock()->SetPosition(m_leftPos + move);
		m_titleBackSprite.lock()->SetPosition(m_rightPos + move);
	}

	// �I�𒆂̃X�e�[�g
	void GameOverState::SelectStageState()
	{
		// �o�ߎ���(0.0f�`XM_2PI)
		m_totalTime += DELTA_TIME * 2.0f;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		// �R���g���[���[�̐ڑ��ɉ����đI���������s��
		Input::GetPadConected() ? ControllerSelect() : MouseSelect();

		// �X�P�[�����T�C���J�[�u�Ńo�E���h����
		float scale = SinCurve(m_totalTime, 1.0f, m_boundScale);
		m_selectSprite.at(m_pastSelect).lock()->SetScale(m_defScale);
		m_selectSprite.at(m_currentSelect).lock()->SetScale(m_defScale * scale);

		// B�{�^�����͂������
		if (Input::GetPushB())
		{
			// SE��炵�ď��������A�X�e�[�g��؂�ւ���
			m_totalTime = 0.0f;
			m_stage.lock()->CreateSE(L"WHISTLE_SE", 1.0f);
			m_selectSprite.at(m_currentSelect).lock()->SetScale(m_defScale);
			m_currentState = eGameOverState::SelectFadeOut;
		}
	}

	// �R���g���[���[�őI��
	void GameOverState::ControllerSelect()
	{
		// LStick����
		float stickVal = Input::GetLStickValue().x;

		// �O��͖����͂ŁA���݂œ��͂������
		if (stickVal && !m_pastStick)
		{
			// �o�ߎ��Ԃ����Z�b�g���A�I������؂�ւ���
			m_totalTime = -XM_PIDIV4;
			m_pastSelect = m_currentSelect;
			switch (m_currentSelect)
			{
			case eSelectGameOver::Continue:
				m_currentSelect = eSelectGameOver::TitleBack;
				break;

			case eSelectGameOver::TitleBack:
				m_currentSelect = eSelectGameOver::Continue;
				break;

			default:
				break;
			}
		}

		// ���͂̕ۑ�
		m_pastStick = stickVal;
	}

	// �}�E�X�ł̑I��
	void GameOverState::MouseSelect()
	{
		Vec2 mousePos = Input::GetMousePosition();	// �}�E�X�̍��W
		Vec2 helfScale = m_defScale / 2.0f;			// �X�P�[���̔���

		// �I�����̍��W
		Vec2 cntnPos = Vec2(m_continueSprite.lock()->GetPosition());
		Vec2 backPos = Vec2(m_titleBackSprite.lock()->GetPosition());

		// ���̃X�e�[�W�փX�v���C�g�͈͓̔��Ȃ�
		if (GetBetween(mousePos, cntnPos + helfScale, cntnPos - helfScale))
		{
			if (m_currentSelect == eSelectGameOver::Continue) return;

			// �I������ύX
			m_totalTime = -XM_PIDIV4;
			m_currentSelect = eSelectGameOver::Continue;
			m_pastSelect = eSelectGameOver::TitleBack;
		}

		// �^�C�g���փX�v���C�g�͈͓̔��Ȃ�
		if (GetBetween(mousePos, backPos + helfScale, backPos - helfScale))
		{
			if (m_currentSelect == eSelectGameOver::TitleBack) return;

			// �I������ύX
			m_totalTime = -XM_PIDIV4;
			m_currentSelect = eSelectGameOver::TitleBack;
			m_pastSelect = eSelectGameOver::Continue;
		}
	}

	// �I�����X�v���C�g�̃t�F�[�h�A�E�g
	void GameOverState::SelectSpriteFadeOut()
	{
		// �C�[�W���O�����ňړ�
		Easing<Vec3> moveEasing;
		Vec3 move = moveEasing.EaseIn(EasingType::Cubic, Vec3(0.0f), m_moveVal, m_totalTime, m_selectFadeTime);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_totalTime >= m_selectFadeTime)
		{
			m_currentState = eGameOverState::StandBy;
			m_totalTime = 0.0f;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_continueSprite.lock()->SetPosition(m_leftPos - move);
		m_titleBackSprite.lock()->SetPosition(m_rightPos - move);

		// ���̃G�t�F�N�g��ǉ�
		m_smoke.lock()->SetEmitterPosition(m_selectSprite.at(m_currentSelect).lock()->GetPosition() + m_smokeDiff);
		m_smoke.lock()->AddParticle(Vec2(RangeRand(50.0f, 5.0f)), m_smokeVelo, RangeRand(XM_PI, -XM_PI), 0.5f);
	}
}