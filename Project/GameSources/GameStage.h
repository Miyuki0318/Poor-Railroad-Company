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
		Playing,
		GameClear,
		GameOver,
		ContinueFade,
	};

	enum class eContinueState
	{
		FadeIn,
		Reset,
		FadeOut,
	};

	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public BaseStage
	{
		// �Q�[���N���A�E�Q�[���I�[�o�[�̃X�v���C�g
		shared_ptr<Sprite> m_gameSprite;

		// �Q�[���̏�
		eGameProgress m_gameProgress;

		// �Q�[���N���A����^�C�g���X�e�[�W�ւ̑J�ڂ܂ł̗P�\
		const float m_defermentTransition;

		// �Q�[���N���A����̌o�ߎ��ԃJ�E���g�p�ϐ�
		float m_countTime;

		eContinueState m_continueState;

		map<eGameProgress, function<void()>> m_progressFunc;
		map<eContinueState, function<void()>> m_continueFunc;

		/*!
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses() override;

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

		/*!
		@brief �^�C�g���X�e�[�W�J�ڗp�̏���
		*/
		void ToTitleStage();

		/*!
		@brief �R���e�B�j���[���̏���
		*/
		void ToContinueStage();

		void ContinueFadeInState();
		void ContinueResetState();
		void ContinueFadeOutState();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage(const string stagePath) :
			BaseStage(stagePath),
			m_defermentTransition(3.0f)
		{
			m_countTime = 0.0f;
			m_gameProgress = eGameProgress::Playing;
			m_continueState = eContinueState::FadeIn;

			m_progressFunc.emplace(eGameProgress::GameClear, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::GameOver, bind(&GameStage::ToContinueStage, this));
			m_progressFunc.emplace(eGameProgress::ContinueFade, bind(&GameStage::ToContinueStage, this));

			m_continueFunc.emplace(eContinueState::FadeIn, bind(&GameStage::ContinueFadeInState, this));
			m_continueFunc.emplace(eContinueState::Reset, bind(&GameStage::ContinueResetState, this));
			m_continueFunc.emplace(eContinueState::FadeOut, bind(&GameStage::ContinueFadeOutState, this));
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