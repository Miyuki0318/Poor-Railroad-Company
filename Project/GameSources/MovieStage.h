/*!
@file MovieStage.h
@brief ���[�r�[�X�e�[�W
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class LeaveMovieStage : public MovieStage
	{
	private:
		const float m_fadeTime;

		float m_timer;

		bool m_isPushButton;

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
			m_isPushButton(false)
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}