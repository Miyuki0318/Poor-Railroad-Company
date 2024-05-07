#pragma once
#include "stdafx.h"

namespace basecross
{
	enum eGameMenber
	{
		Play,
		Clear,
		Over
	};
	class StageSet : public Stage
	{
		shared_ptr<SoundItem> m_BGM;

		// �Q�[���N���A�E�Q�[���I�[�o�[�̃X�v���C�g
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

		// �Q�[���̏�
		eGameMenber m_gameProgress;

		// �^�C�}�[�I�u�W�F�N�g
		weak_ptr<Timer> m_timer;
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
		@brief �̌@�n�I�u�W�F�N�g�̐�������
		*/
		void CreateStageObject();

		/*!
		@brief ���H����
		*/
		void CreateRail();
		void CreateRails();

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
		@brief �X�v���C�g�̐���
		*/
		void LogoActive();

		//BGM�̐���
		void CreateBGM();

	public:
		StageSet() :Stage() {
			m_gameProgress = eGameMenber::Play;
		}
			

		virtual ~StageSet() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		virtual void OnDestroy()override;
		//�^�C�}�[�N���X�擾�֐�
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}
		//�Q�[���̐i�s��Ԃ��Q�b�g����֐�
		eGameMenber GetGameProgress()
		{
			return m_gameProgress;
		}
		//�Q�[���̐i�s��Ԃ��Z�b�g����֐�
		void SetGameProgress(eGameMenber progress)
		{
			m_gameProgress = progress;
		}
	};

}
