/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
	{
	private:
		const Vec3 m_cameraEye = Vec3(0.0f, 30.0f, -20.0f);
		const Vec3 m_cameraAt = Vec3(0.0f, 0.0f, 0.0f);

		// �X�^�[�g�{�^���������ꂽ��
		bool m_startPush;

		// �t�F�[�h���I�������
		bool m_fadeStop;

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
		@brief �X�v���C�g�̐���
		*/
		void CreateSpriteObject();

		/*
		@brief �n�ʂ̐���
		*/
		void CreateGround();
		
		/*
		@brief �X�^�[�g�{�^�������������̏���
		*/
		void PushStartButton();

		/*
		@brief �t�F�[�h�C������
		*/
		void FadeInSprite();

		/*
		@brief �t�F�[�h�A�E�g����
		*/
		void FadeOutSprite();

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
	};
}