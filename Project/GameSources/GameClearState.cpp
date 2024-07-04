/*!
@file GameClearState.cpp
@brief �Q�[���N���A���̏����X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GamePlayer.h"
#include "GameClearState.h"
#include "Input.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void GameClearState::CreateState()
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = m_stage.lock();

		// �Q�[���N���A���Ɉ����X�v���C�g
		m_nextStageSprite = stagePtr->AddGameObject<Sprite>(L"NEXTSTAGE_TX", m_defScale, m_leftPos - m_moveVal);
		m_clearBackSprite = stagePtr->AddGameObject<Sprite>(L"CLEAR_TITLEBACK_TX", m_defScale, m_rightPos - m_moveVal);
		m_railLineSprite = stagePtr->AddGameObject<Sprite>(L"RAIL_LINE_TX", Vec2(WINDOW_WIDTH, 128.0f), m_railPos);

		// ���̃G�t�F�N�g�X�v���C�g���擾
		m_smoke = stagePtr->GetSharedGameObject<SpriteParticle>(L"SmokeEffect");

		// �I�����}�b�v�ɒǉ�
		m_selectSprite.emplace(eSelectGameClear::NextStage, m_nextStageSprite);
		m_selectSprite.emplace(eSelectGameClear::TitleBack, m_clearBackSprite);
	}

	// �X�V����
	void GameClearState::UpdateState()
	{
		// �X�e�[�g�ɉ������֐��Ăяo��
		m_stateFunc.at(m_currentState)();
	}

	// ����������
	void GameClearState::ResetState()
	{
		m_totalTime = 0.0f;
		m_currentState = eGameClearState::RailFadeIn;
		
		// �������̍��W�ɖ߂�
		m_nextStageSprite.lock()->SetPosition(m_leftPos - m_moveVal);
		m_clearBackSprite.lock()->SetPosition(m_rightPos - m_moveVal);
		m_railLineSprite.lock()->SetPosition(m_railPos);
	}

	// ���[���̃t�F�[�h�C��
	void GameClearState::RailSpriteFadeIn()
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_railPos, m_railPos + m_moveVal, m_totalTime / m_railFadeTime);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_totalTime >= m_railFadeTime)
		{
			m_currentState = eGameClearState::SelectFadeIn;
			m_totalTime = 0.0f;
		}

		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_railLineSprite.lock()->SetPosition(pos);
	}

	// �I�����X�v���C�g�̃t�F�[�h�C��
	void GameClearState::SelectSpriteFadeIn()
	{
		// �C�[�W���O�����ňړ�
		Easing<Vec3> moveEasing;
		Vec3 move = moveEasing.EaseOut(EasingType::Cubic, m_moveVal, Vec3(0.0f), m_totalTime, m_selectFadeTime);

		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_totalTime >= m_selectFadeTime)
		{
			m_currentState = eGameClearState::SelectState;
			m_totalTime = -XM_PIDIV4;
		}
		
		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos - move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos - move);
	}

	// �I��
	void GameClearState::SelectStageState()
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
			m_currentState = eGameClearState::SelectFadeOut;
		}
	}

	// �R���g���[���[�őI��
	void GameClearState::ControllerSelect()
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
			case eSelectGameClear::TitleBack:
				m_currentSelect = eSelectGameClear::NextStage;
				break;

			case eSelectGameClear::NextStage:
				m_currentSelect = eSelectGameClear::TitleBack;
				break;

			default:
				break;
			}
		}

		// ���͂̕ۑ�
		m_pastStick = stickVal;
	}

	// �}�E�X�ł̑I��
	void GameClearState::MouseSelect()
	{
		Vec2 mousePos = Input::GetMousePosition();	// �}�E�X�̍��W
		Vec2 helfScale = m_defScale / 2.0f;			// �X�P�[���̔���

		// �I�����̍��W
		Vec2 nextPos = Vec2(m_nextStageSprite.lock()->GetPosition());
		Vec2 backPos = Vec2(m_clearBackSprite.lock()->GetPosition());

		// ���̃X�e�[�W�փX�v���C�g�͈͓̔��Ȃ�
		if (GetBetween(mousePos, nextPos + helfScale, nextPos - helfScale))
		{
			if (m_currentSelect == eSelectGameClear::NextStage) return;

			// �I������ύX
			m_totalTime = -XM_PIDIV4;
			m_currentSelect = eSelectGameClear::NextStage;
			m_pastSelect = eSelectGameClear::TitleBack;
		}

		// �^�C�g���փX�v���C�g�͈͓̔��Ȃ�
		if (GetBetween(mousePos, backPos + helfScale, backPos - helfScale))
		{
			if (m_currentSelect == eSelectGameClear::TitleBack) return;

			// �I������ύX
			m_totalTime = -XM_PIDIV4;
			m_currentSelect = eSelectGameClear::TitleBack;
			m_pastSelect = eSelectGameClear::NextStage;
		}
	}

	// �I�����X�v���C�g�̃t�F�[�h�A�E�g
	void GameClearState::SelectSpriteFadeOut()
	{
		// �C�[�W���O�����ňړ�
		Easing<Vec3> moveEasing;
		Vec3 move = moveEasing.EaseIn(EasingType::Cubic, Vec3(0.0f), m_moveVal, m_totalTime, m_selectFadeTime);
		
		// �t�F�[�h���Ԃ��߂�����X�e�[�g��؂�ւ�
		if (m_totalTime >= m_selectFadeTime)
		{
			m_currentState = eGameClearState::StandBy;
			m_totalTime = 0.0f;
		}
		
		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos + move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos + move);

		// ���̃G�t�F�N�g��ǉ�
		m_smoke.lock()->SetEmitterPosition(m_selectSprite.at(m_currentSelect).lock()->GetPosition() + m_smokeDiff);
		m_smoke.lock()->AddParticle(Vec2(RangeRand(50.0f, 5.0f)), m_smokeVelo, RangeRand(XM_PI, -XM_PI), 0.5f);
	}
}