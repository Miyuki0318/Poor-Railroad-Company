/*!
@file MovieStage.h
@brief ���[�r�[�X�e�[�W
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "ResourcesManager.h"

namespace basecross
{
	class LeaveMovieStage : public MovieStage, public ResourcesManager
	{
	private:
		const float m_fadeTime;

		// �o�ߎ���
		float m_timer;

		wstring m_moviePath;

		// �{�^���������ꂽ��
		bool m_pushButton;

		/*
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses();

		/*
		@brief �w�i�̃X�v���C�g�̐���
		*/
		void CreateBackSprite();

		/*
		@brief �{�^�������������̏���
		*/
		void ButtonPush();

		/*
		@brief �X�e�[�W�ڍs
		*/
		void ChangeStage();

	public :
		LeaveMovieStage() :
			MovieStage(),
			m_fadeTime(1.0f),
			m_timer(0.0f),
			m_pushButton(false)
		{
		}

		virtual ~LeaveMovieStage()
		{
		};

		/*
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*
		@brief ���t���[���Ăяo�����֐�
		*/
		virtual void OnUpdate() override;
	};
}