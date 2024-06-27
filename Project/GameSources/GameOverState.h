/*!
@file GameOverState.h
@brief �Q�[���I�[�o�[���̏����X�e�[�g
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	// ���s���̃X�e�[�genum
	enum class eGameOverState : char
	{
		RailFadeIn,		// ���[���̃t�F�[�h
		SelectFadeIn,	// �I�����̃t�F�[�h�C��
		SelectState,	// �X�e�[�W�I��
		SelectFadeOut,	// �I�����̃t�F�[�h�A�E�g
		StandBy,		// �ҋ@���
	};

	// �I�������^�C�v
	enum class eSelectGameOver : char
	{
		Continue,	// �R���e�B�j���[
		TitleBack,	// �^�C�g���ɖ߂�
	};

	// �Q�[���X�e�[�W�̖��O�錾
	class GameStage;

	/*!
	@brief �Q�[���I�[�o�[���̃X�e�[�g
	*/
	class GameOverState
	{
		// �����X�e�[�W�|�C���^
		weak_ptr<GameStage> m_stage;

		// �N���A���̑I�����X�v���C�g
		weak_ptr<Sprite> m_continueSprite;	// �R���e�B�j���[
		weak_ptr<Sprite> m_titleBackSprite;	// �^�C�g���o�b�N
		weak_ptr<Sprite> m_railLineSprite;	// ���[��

		weak_ptr<SpriteParticle> m_smoke; // ���̃G�t�F�N�g

		const Vec2 m_defScale;	// �X�P�[��
		const Vec3 m_leftPos;	// �I�������̍��W
		const Vec3 m_rightPos;	// �I�����E�̍��W
		const Vec3 m_railPos;	// ���[���̍��W
		const Vec3 m_moveVal;	// ������
		const Vec3 m_smokeDiff;	// ���̏o��ʒu
		const Vec2 m_smokeVelo;	// ���̈ړ���

		const float m_boundScale;		// �o�E���h���̃X�P�[��
		const float m_railFadeTime;		// ���[���̃t�F�[�h����
		const float m_selectFadeTime;	// �I�����̃t�F�[�h����

		float m_totalTime;	// �o�ߎ���
		float m_pastStick;	// �O��̃X�e�B�b�N����

		eSelectGameOver m_currentSelect;	// �I�������X�e�[�W
		eSelectGameOver m_pastSelect;		// �O��I�������X�e�[�W
		eGameOverState m_currentState;		// �X�e�[�g

		// �X�e�[�g�ɉ������֐��Ăяo���}�b�v
		map<eGameOverState, function<void()>> m_stateFunc;

		// �I�����ɉ������X�v���C�g�}�b�v
		map<eSelectGameOver, weak_ptr<Sprite>> m_selectSprite;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GameOverState(const shared_ptr<GameStage>& stagePtr) :
			m_stage(stagePtr),
			m_boundScale(1.25f),
			m_railFadeTime(3.0f),
			m_selectFadeTime(5.0f),
			m_defScale(300.0f, 350.0f),
			m_leftPos(-300.0f, -200.0f, 0.0f),
			m_rightPos(300.0f, -200.0f, 0.0f),
			m_railPos(1920.0f, -250.0f, 0.2f),
			m_moveVal(1920.0f, 0.0f, 0.0f),
			m_smokeDiff(-93.0f, 140.0f, 0.0f),
			m_smokeVelo(3.0f)
		{
			m_totalTime = 0.0f;
			m_pastStick = 0.0f;

			m_currentSelect = eSelectGameOver::Continue;
			m_pastSelect = eSelectGameOver::TitleBack;

			m_currentState = eGameOverState::RailFadeIn;
			m_stateFunc.emplace(eGameOverState::RailFadeIn, bind(&GameOverState::RailSpriteFadeIn, this));
			m_stateFunc.emplace(eGameOverState::SelectFadeIn, bind(&GameOverState::SelectSpriteFadeIn, this));
			m_stateFunc.emplace(eGameOverState::SelectState, bind(&GameOverState::SelectStageState, this));
			m_stateFunc.emplace(eGameOverState::SelectFadeOut, bind(&GameOverState::SelectSpriteFadeOut, this));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		~GameOverState() {}

		/*!
		@brief �X�v���C�g�̐����֐�
		*/
		void CreateState();

		/*!
		@brief �X�e�[�g�̍X�V�����֐�
		*/
		void UpdateState();

		/*!
		@brief �X�e�[�g�̏������֐�
		*/
		void ResetState();

		/*!
		@brief �X�e�[�g�擾�֐�
		@return m_gameClearState
		*/
		eGameOverState GetState() const
		{
			return m_currentState;
		}

		/*!
		@brief �I�������X�e�[�W�擾�֐�
		@return m_selectStage
		*/
		eSelectGameOver GetSelectStage() const
		{
			return m_currentSelect;
		}

	private:

		/*!
		@brief ���[���X�v���C�g�̃t�F�[�h�C������
		*/
		void RailSpriteFadeIn();

		/*!
		@brief �I�����v���C�g�̃t�F�[�h�C������
		*/
		void SelectSpriteFadeIn();

		/*!
		@brief �X�e�[�W�I�𒆂̏���
		*/
		void SelectStageState();

		/*!
		@brief �I�����v���C�g�̃t�F�[�h�C������
		*/
		void SelectSpriteFadeOut();
	};
}