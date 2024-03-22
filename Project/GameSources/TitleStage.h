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
		bool m_startPush;

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
		@brief �X�^�[�g�{�^��������������
		*/
		void PushStartButton();

	public:
		/*
		@brief �R���X�g���N�^
		*/
		TitleStage() : Stage()
		{
			m_startPush = false;
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