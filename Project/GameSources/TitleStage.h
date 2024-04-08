/*!
@file TitleStage.h
@brief タイトルステージ
@author 新妻葵
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
	{
	private:
		// 地面のスケール
		const Vec3 m_groundScale = Vec3(+50.0f, +2.0f, +50.0f);

		// カメラの位置・視点
		const Vec3 m_cameraEye = Vec3(m_groundScale.x, 30.0f, -15.0f);
		const Vec3 m_cameraAt = Vec3(m_groundScale.x, 0.0f, 0.0f);

		// スタートボタンが押されたか
		bool m_startPush;

		// フェードが終わったか
		bool m_fadeStop;

		// フェードの時間
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;

		shared_ptr<Sprite> m_fadeSprite;

		/*
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*
		@brief リソースの読込
		*/
		void CreateResourses();

		/*
		@brief スプライトの生成
		*/
		void CreateSpriteObject();

		/*
		@brief 地面の生成
		*/
		void CreateGround();

		/*
		@brief 建物の生成
		*/
		void CreateBuilding();

	public:
		/*
		@brief コンストラクタ
		*/
		TitleStage() : Stage()
		{
			m_startPush = false;
			m_fadeStop = false;
		}

		/*
		@brief デストラクタ
		*/
		virtual ~TitleStage() {};

		/*
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;
		
		/*
		@brief 毎フレーム呼び出される関数
		*/
		virtual void OnUpdate() override;
		
		/*
		@brief オブジェクト破棄時に呼び出される関数
		*/
		virtual void OnDestroy() override;
	};
}