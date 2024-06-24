/*!
@file GameClearState.cpp
@brief �Q�[���N���A���̏����X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void GameClearState::CreateState()
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = m_stage.lock();

		// �Q�[���N���A���Ɉ����X�v���C�g
		m_nextStageSprite = stagePtr->AddGameObject<Sprite>(L"NEXTSTAGE_TX", m_defScale, m_leftPos - m_moveVal);
		m_clearBackSprite = stagePtr->AddGameObject<Sprite>(L"CLEAR_TITLEBACK_TX", m_defScale, m_rightPos - m_moveVal);
		m_railLineSprite = stagePtr->AddGameObject<Sprite>(L"RAIL_LINE_TX", Vec2(WINDOW_WIDTH, 128.0f), m_railPos);

		// �I�����}�b�v�ɒǉ�
		m_selectSprite.emplace(eSelectStage::NextStage, m_nextStageSprite);
		m_selectSprite.emplace(eSelectStage::TitleBack, m_clearBackSprite);
	}

	// �X�V����
	void GameClearState::UpdateState()
	{
		// �X�e�[�g�ɉ������֐��Ăяo��
		m_stateFunc.at(m_clearState)();
	}

	// ����������
	void GameClearState::ResetState()
	{
		m_totalTime = 0.0f;
		m_clearState = eGameClearState::RailFadeIn;
		
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
			m_clearState = eGameClearState::SelectFadeIn;
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
			m_clearState = eGameClearState::SelectStage;
			m_totalTime = -XM_PIDIV4;
		}
		
		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos - move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos - move);
	}

	void GameClearState::SelectStageState()
	{
		// LStick����
		float stickVal = Input::GetLStickValue().x;

		// �o�ߎ���(0.0f�`XM_2PI)
		m_totalTime += DELTA_TIME * 2.0f;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		// �O��͖����͂ŁA���݂œ��͂������
		if (stickVal && !m_pastStick)
		{
			// �o�ߎ��Ԃ����Z�b�g���A�I������؂�ւ���
			m_totalTime = -XM_PIDIV4;
			m_pastSelect = m_selectStage;
			switch (m_selectStage)
			{
			case eSelectStage::TitleBack:
				m_selectStage = eSelectStage::NextStage;
				break;

			case eSelectStage::NextStage:
				m_selectStage = eSelectStage::TitleBack;
				break;

			default:
				break;
			}
		}

		// ���͂̕ۑ�
		m_pastStick = stickVal;

		// �X�P�[�����T�C���J�[�u�Ńo�E���h����
		float scale = Utility::SinCurve(m_totalTime, 1.0f, m_boundScale);
		m_selectSprite.at(m_pastSelect).lock()->SetScale(m_defScale);
		m_selectSprite.at(m_selectStage).lock()->SetScale(m_defScale * scale);

		// B�{�^�����͂������
		if (Input::GetPushB())
		{
			// SE��炵�ď��������A�X�e�[�g��؂�ւ���
			m_totalTime = 0.0f;
			m_stage.lock()->CreateSE(L"WHISTLE_SE", 1.0f);
			m_selectSprite.at(m_selectStage).lock()->SetScale(m_defScale);
			m_clearState = eGameClearState::SelectFadeOut;
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
			m_clearState = eGameClearState::StandBy;
			m_totalTime = 0.0f;
		}
		
		// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z���A���W���X�V
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos + move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos + move);
	}
}