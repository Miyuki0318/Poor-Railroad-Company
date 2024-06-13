/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"

namespace basecross 
{
	/*!
	@brief �Q�[���̐i�s��
	*/
	enum eGameProgress
	{
		FadeIn,
		StartSE,
		Playing,
		GameClear,
		GameOver,
		ContinueFade,
	};

	// �R���e�B�j���[�������̃X�e�[�genum
	enum class eContinueState
	{
		FadeIn,
		SelectFade,
		Selecting,
		SelectState,
		FadeOut,
	};

	// �R���e�B�j���[���̑I����
	enum class eContinueSelect
	{
		Continue,
		TitleBack,
	};

	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public BaseStage
	{
		weak_ptr<SoundItem> m_bgmItem; // BGM�̃T�E���h�A�C�e��

		// �Q�[���N���A�E�Q�[���I�[�o�[�̃X�v���C�g
		shared_ptr<Sprite> m_gameSprite;

		// �R���e�B�j���[���̃X�v���C�g
		shared_ptr<Sprite> m_ctSprite;	// �R���e�B�j���[
		shared_ptr<Sprite> m_tbSprite;	// �^�C�g���o�b�N
		const Vec2 m_defScale;

		// �Q�[���̏�
		eGameProgress m_gameProgress;

		// �R���e�B�j���[�����̃X�e�[�g
		eContinueState m_continueState;

		// �R���e�B�j���[���̑I����
		eContinueSelect m_currentState;
		eContinueSelect m_pastState;

		// �Q�[���N���A����^�C�g���X�e�[�W�ւ̑J�ڂ܂ł̗P�\
		const float m_defermentTransition;

		// �Q�[���N���A����̌o�ߎ��ԃJ�E���g�p�ϐ�
		float m_countTime;
		float m_totalTime;

		// �R���e�B�j���[���̃X�e�B�b�N����
		float m_pastStickVal;

		Vec3 m_startPosition;
		Vec3 m_goalStagingPosition;

		map<eGameProgress, function<void()>> m_progressFunc;
		map<eContinueState, function<void()>> m_continueFunc;
		map<eContinueSelect, weak_ptr<Sprite>> m_selectMap;
		map<eContinueSelect, function<void()>> m_selectFunc;

		/*!
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses() override;

		/*!
		@brief BGM�̍Đ�
		*/
		void CreateStartBGM();

		/*!
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*!
		@brief �X�e�[�W�{�b�N�X�̐���
		*/
		void CreateGroundBox();

		/*!
		@brief �v���C���[����
		*/
		void CreatePlayer();

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		void CreateStageCSV(string csvPath) override;

		/*!
		@brief ���H����
		*/
		void CreateRailManager();

		/*!
		@brief �؂̑��ꐶ��
		*/
		void CreateBridgeManager();

		/*!
		@brief ���؂̐���
		*/
		void CreateCrossingManager();

		/*!
		@brief �̏W�I�u�W�F�N�g�̐���
		*/
		void CreateGatheringManager();

		/*!
		@brief ���ԃI�u�W�F�N�g�̐���
		*/
		void CreateGearManager();

		/*!
		@brief ��Ԑ���
		*/
		void CreateTrain();

		/*!
		@brief �X�v���C�g�̐���
		*/
		void CreateSpriteObject();

		/*!
		@brief UI�̐���
		*/
		void CreateUIObject();

		/*!
		@brief �X�v���C�g�̕\��
		*/
		void LogoActive();

		void ToFadeInState();

		//void ToStartSEState();

		/*!
		@brief �^�C�g���X�e�[�W�J�ڗp�̏���
		*/
		void ToTitleStage();

		/*!
		@brief �R���e�B�j���[���̏���
		*/
		void ToContinueStage();

		void ContinueFadeInState();
		void ContinueSelectFadeState();
		void ContinueSelectingState();
		void ContinueSelectState();
		void ContinueFadeOutState();

		void ResetState();
		void TitleBackState();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage(const string stagePath) :
			BaseStage(stagePath),
			m_defermentTransition(3.0f),
			m_defScale(275.0f, 100.0f)
		{
			m_countTime = 0.0f;
			m_totalTime = 0.0f;
			m_pastStickVal = 0.0f;
			m_gameProgress = eGameProgress::FadeIn;
			m_continueState = eContinueState::FadeIn;
			m_currentState = eContinueSelect::Continue;
			m_pastState = eContinueSelect::TitleBack;

			m_progressFunc.emplace(eGameProgress::FadeIn, bind(&GameStage::ToFadeInState, this));
			//m_progressFunc.emplace(eGameProgress::StartSE, bind(&GameStage::ToStartSEState, this));
			m_progressFunc.emplace(eGameProgress::GameClear, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::GameOver, bind(&GameStage::ToContinueStage, this));
			m_progressFunc.emplace(eGameProgress::ContinueFade, bind(&GameStage::ToContinueStage, this));

			m_continueFunc.emplace(eContinueState::FadeIn, bind(&GameStage::ContinueFadeInState, this));
			m_continueFunc.emplace(eContinueState::SelectFade, bind(&GameStage::ContinueSelectFadeState, this));
			m_continueFunc.emplace(eContinueState::Selecting, bind(&GameStage::ContinueSelectingState, this));
			m_continueFunc.emplace(eContinueState::SelectState, bind(&GameStage::ContinueSelectState, this));
			m_continueFunc.emplace(eContinueState::FadeOut, bind(&GameStage::ContinueFadeOutState, this));

			m_selectFunc.emplace(eContinueSelect::Continue, bind(&GameStage::ResetState, this));
			m_selectFunc.emplace(eContinueSelect::TitleBack, bind(&GameStage::TitleBackState, this));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �j������鎞�Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �Q�[���̐i�s��Ԃ��Q�b�g����֐�
		*/
		eGameProgress GetGameProgress() const
		{
			return m_gameProgress;
		}

		/*!
		@brief �Q�[���̐i�s��Ԃ��Z�b�g����֐�
		*/
		void SetGameProgress(eGameProgress progress)
		{
			m_gameProgress = progress;
		}
	};
}