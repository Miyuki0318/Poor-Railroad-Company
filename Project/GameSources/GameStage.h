/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	/*!
	@brief �Q�[���̐i�s��
	*/
	enum eGameProgress
	{
		Playing,
		GameClear,
		GameOver
	};

	// �X�e�[�W�I�u�W�F�N�gID
	enum class eStageID
	{
		Rail = 1,	// ���[��
		DeRail,		// �E������
		GoalRail,	// �S�[�����[��
		Rock = 10,	// ��
		Tree,		// ��
	};

	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public Stage
	{
		// �Q�[���N���A�E�Q�[���I�[�o�[�̃X�v���C�g
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

		// �Q�[���̏�
		eGameProgress m_gameProgress;

		// �^�C�}�[�I�u�W�F�N�g
		weak_ptr<Timer> m_timer;

		// �X�e�[�W�}�b�v
		vector<vector<int>> m_stageMap;

		/*!
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses();

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
		void CreateStageCSV(string csvPath = "TestStage");

		/*!
		@brief �̌@�n�I�u�W�F�N�g�̐�������
		*/
		void CreateStageObject();

		/*!
		@brief ���H����
		*/
		void CreateRail();
		void CreateRails();

		/*!
		@brief�S�[���n�_�̐���
		*/
		void CreateGoalRail();

		/*!
		@brief ��Ԑ���
		*/
		void CreateTrain();

		/*!
		@brief ���H�̏I������
		*/
		void CreateTarminal();

		/*!
		@brief �X�v���C�g�̐���
		*/
		void CreateSpriteObject();

		/*!
		@brief �X�v���C�g�̕\��
		*/
		void LogoActive();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage() :Stage() {
			m_gameProgress = eGameProgress::Playing;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		///*!
		//@brief �j������鎞�Ɉ�x�����Ăяo�����֐�
		//*/
		//virtual void OnDestroy() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

		/*!
		@brief �^�C�}�[�N���X�擾�֐�
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}

		/*!
		@brief �Q�[���̐i�s��Ԃ��Q�b�g����֐�
		*/
		eGameProgress GetGameProgress()
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