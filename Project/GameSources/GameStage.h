/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "GameClearState.h"
#include "GameOverState.h"
#include "PaymentsState.h"

namespace basecross 
{
	/*!
	@brief �Q�[���̐i�s��
	*/
	enum eGameProgress
	{
		FadeIn,				// �J�n���̃t�F�[�h�C��
		Opening,
		Playing,			// �v���C��
		CraftPause,			// �N���t�g���̃|�[�Y
		Pause,				// �|�[�Y
		GoalConect,			// �S�[���Ɛڑ�����
		GameClear,			// �Q�[���N���A
		MoneyCalculation,	// ���Z����
		MoneyCountDown,		// ���Z���ʕ\��
		ClearSelect,		// �N���A��̑I��
		ToNext,				// ���̃X�e�[�W��
		ToTitleClear,		// �N���A���̃^�C�g���X�e�[�W��
		ToTitleOver,		// ���s���̃^�C�g���X�e�[�W��
		DeRailed,			// �E����
		GameOver,			// �Q�[���I�[�o�[
		ContinueFadeIn,		// �R���e�B�j���[���̃t�F�[�h�C��
		ContinueFadeOut,	// �R���e�B�j���[���̃t�F�[�h�A�E�g
	};

	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public BaseStage
	{
		weak_ptr<SoundItem> m_bgmItem; // BGM�̃T�E���h�A�C�e��

		// �Q�[���N���A�E�Q�[���I�[�o�[�̃X�v���C�g
		shared_ptr<Sprite> m_gameSprite;

		// �N���A���̏���
		unique_ptr<GameClearState> m_gameClearState;
		unique_ptr<GameOverState> m_gameOverState;
		unique_ptr<PaymentsState> m_paymentsState;

		// �Q�[���̏�
		eGameProgress m_gameProgress;

		// �Q�[���N���A����^�C�g���X�e�[�W�ւ̑J�ڂ܂ł̗P�\
		const float m_defermentTransition;

		// �Q�[���N���A����̌o�ߎ��ԃJ�E���g�p�ϐ�
		float m_countTime;
		float m_totalTime;

		// �R���e�B�j���[���̃X�e�B�b�N����
		float m_pastStickVal;

		// �J�n���̈ʒu�ƃS�[����̉��o�ʒu
		Vec3 m_startPosition;
		Vec3 m_goalStagingPosition;

		// �X�e�[�g����
		map<eGameProgress, function<void()>> m_progressFunc;

		// BGMkey
		map<wstring, string> m_bgmKeyMap;

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
		@brief �n�ʂ�csv�ōĐ���
		*/
		void ResetGroundStage();

		/*!
		@brief �J�����̍Đ���
		*/
		void ResetCameraObject();

		/*!
		@brief �X�v���C�g�̕\��
		*/
		void LogoActive();

		/*!
		@brief �J�n���̃t�F�[�h�C���̏���
		*/
		void ToFadeInState();

		/*!
		@brief �J�n���̃t�F�[�h�C���̏���
		*/
		void ToOpeningState();

		/*!
		@brief �Q�[�����̏���
		*/
		void ToPlayingState();

		/*!
		@brief ���x�v�Z�̏���
		*/
		void ToMoneyCalculationState();

		/*!
		@brief ���x���ʕ\���̏���
		*/
		void ToMoneyCountDownState();

		/*!
		@brief �Q�[���N���A���̑I������
		*/
		void ToClearSelectStage();

		/*!
		@brief ���̃X�e�[�W�֏���
		*/
		void ToNextStage();

		/*!
		@brief �^�C�g���X�e�[�W�J�ڗp�̏���
		*/
		void ToTitleStage();

		void PushButtonStart();

		/*!
		@brief �Q�[���I�[�o�[���̏���
		*/
		void ToGameOverStage();

		/*!
		@brief �R���e�B�j���[���t�F�[�h�C������
		*/
		void ToContinueFadeIn();

		/*!
		@brief �R���e�B�j���[���t�F�[�h�A�E�g����
		*/
		void ToContinueFadeOut();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage(const string stagePath) :
			BaseStage(stagePath),
			m_defermentTransition(3.0f)
		{
			m_countTime = 0.0f;
			m_totalTime = 0.0f;
			m_pastStickVal = 0.0f;

			m_bgmKeyMap.emplace(L"FIRST_BGM", "First");
			m_bgmKeyMap.emplace(L"FIRST_BGM", "Second");
			m_bgmKeyMap.emplace(L"FIRST_BGM", "Third");
			m_bgmKeyMap.emplace(L"FIRST_BGM", "Fourth");
			m_bgmKeyMap.emplace(L"SECOND_BGM", "Lv5");
			m_bgmKeyMap.emplace(L"SECOND_BGM", "Lv6");
			m_bgmKeyMap.emplace(L"THIRD_BGM", "Lv7");
			m_bgmKeyMap.emplace(L"THIRD_BGM", "Lv8");
			m_bgmKeyMap.emplace(L"FOURTH_BGM", "Lv9");
			m_bgmKeyMap.emplace(L"FOURTH_BGM", "Lv10");

			m_gameProgress = eGameProgress::FadeIn;

			m_progressFunc.emplace(eGameProgress::FadeIn, bind(&GameStage::ToFadeInState, this));
			m_progressFunc.emplace(eGameProgress::Opening, bind(&GameStage::ToOpeningState, this));
			m_progressFunc.emplace(eGameProgress::Playing, bind(&GameStage::ToPlayingState, this));
			m_progressFunc.emplace(eGameProgress::MoneyCalculation, bind(&GameStage::ToMoneyCalculationState, this));
			m_progressFunc.emplace(eGameProgress::MoneyCountDown, bind(&GameStage::ToMoneyCountDownState, this));
			m_progressFunc.emplace(eGameProgress::ClearSelect, bind(&GameStage::ToClearSelectStage, this));
			m_progressFunc.emplace(eGameProgress::ToNext, bind(&GameStage::ToNextStage, this));
			m_progressFunc.emplace(eGameProgress::ToTitleClear, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::ToTitleOver, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::GameOver, bind(&GameStage::ToGameOverStage, this));
			m_progressFunc.emplace(eGameProgress::ContinueFadeIn, bind(&GameStage::ToContinueFadeIn, this));
			m_progressFunc.emplace(eGameProgress::ContinueFadeOut, bind(&GameStage::ToContinueFadeOut, this));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameStage() 
		{
			m_gameClearState.reset();
			m_gameOverState.reset();
			m_paymentsState.reset();
		}

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
		@brief �X�e�[�W��csv�ōĐ���
		*/
		void ResetCreateStage();

		/*!
		@brief �t�F�[�h�C���𑗂�A�I���������Ԃ��֐�
		*/
		bool GetFadeIn() const
		{
			return m_fadeSprite->FadeInColor(2.0f);
		}

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