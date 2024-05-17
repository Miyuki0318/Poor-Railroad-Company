/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "ResourcesManager.h"

//#define STAGE_ID(id) static_cast<eStageID>(id)

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

	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public BaseStage, public ResourcesManager
	{
		// �Q�[���N���A�E�Q�[���I�[�o�[�̃X�v���C�g
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

		// �Q�[���̏�
		eGameProgress m_gameProgress;

		/*!
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses();

		/*!
		@brief ���\�[�X�̉��
		*/
		void ReleasedResourses();

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
		void CreateStageCSV(string csvPath = "Test") override;

		/*!
		@brief �̌@�n�I�u�W�F�N�g�̐�������
		*/
		void CreateStageObject();

		/*!
		@brief ���H����
		*/
		void CreateRailManager();

		/*!
		@brief�S�[���n�_�̐���
		*/
		void CreateGoalRail();

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

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage() :BaseStage(), ResourcesManager() {
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

		/*!
		@brief �j������鎞�Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

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