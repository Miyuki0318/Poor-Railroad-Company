/*!
@file MovieStage.h
@brief ムービーステージ
@author 新妻葵
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

		wstring moviePath;

		bool m_isPushButton;

		/*
		@brief リソースの読込
		*/
		void CreateResourses();

		/*
		@brief ボタンを押した時の処理
		*/
		void ButtonPush();

		/*
		@brief ステージ移行
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

		virtual ~LeaveMovieStage()
		{
		};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}