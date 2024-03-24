/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public Stage
	{
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

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

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage() :Stage() {}

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
		@brief �^�C�}�[�N���X�擾�֐�
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}
	};


}
//end basecross

