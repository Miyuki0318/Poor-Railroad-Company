/*!
@file TitleStage.h
@brief �^�C�g���X�e�[�W
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
	{
	private:
		// �n�ʂ̃X�P�[��
		const Vec3 m_groundScale = Vec3(+50.0f, +2.0f, +50.0f);

		// �J�����̈ʒu�E���_
		const Vec3 m_cameraEye = Vec3(+50.0f, 20.0f, -22.0f);
		const Vec3 m_cameraAt = Vec3(50.0f, 2.0f, 0.0f);

		// �X�^�[�g�{�^���������ꂽ��
		bool m_startPush;

		// �t�F�[�h���I�������
		bool m_fadeStop;

		// �t�F�[�h�̎���
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;

		shared_ptr<Sprite> m_fadeSprite;

		/*
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses();

		/*
		@brief �n�ʂ̐���
		*/
		void CreateGround();

		/*
		@brief �n�ʂ̐���
		*/
		void CreatePlayer();

		/*
		@brief �����̐���
		*/
		void CreateBuilding();

		/*
		@brief �����̓����蔻��̐���
		*/
		void CreateCollision();

	public:
		/*
		@brief �R���X�g���N�^
		*/
		TitleStage() : Stage()
		{
			m_startPush = false;
			m_fadeStop = false;
		}

		/*
		@brief �f�X�g���N�^
		*/
		virtual ~TitleStage() {};

		/*
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;
		
		/*
		@brief ���t���[���Ăяo�����֐�
		*/
		virtual void OnUpdate() override;
		
		/*
		@brief �I�u�W�F�N�g�j�����ɌĂяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;
	};
}