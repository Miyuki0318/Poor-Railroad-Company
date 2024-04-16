/*!
@file StageSet.cpp
@brief �X�e�[�W�̔z�u����
*/

#pragma once
#include "stdafx.h"
namespace basecross {
	class StageSet :public Stage
	{
		enum eGameProgress
		{
			Playing,
			GameClear,
			GameOver
		};

		// �Q�[���̏�
		eGameProgress m_gameProgress;
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

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		StageSet() :Stage() {
			m_gameProgress = eGameProgress::Playing;
		}
		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~StageSet() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;



	};

}