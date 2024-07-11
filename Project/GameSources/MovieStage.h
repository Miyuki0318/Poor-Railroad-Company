/*!
@file MovieStage.h
@brief ムービーステージ
@author 新妻葵
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

		float m_timer;

		wstring m_moviePath;

		bool m_isStart;

		bool m_isPushButton;

		/*
		@brief リソースの読込
		*/
		void CreateResourses();

		/*
		@brief 背景のスプライトの生成
		*/
		void CreateBackSprite();

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
			m_isStart(false),
			m_isPushButton(false)
		{
		}

		virtual ~LeaveMovieStage()
		{
		};

		/*
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*
		@brief 毎フレーム呼び出される関数
		*/
		virtual void OnUpdate() override;
	};
}