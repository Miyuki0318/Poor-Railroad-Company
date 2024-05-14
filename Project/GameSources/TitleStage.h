/*!
@file TitleStage.h
@brief タイトルステージ
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "StageCSV.h"
#include "ResourcesManager.h"

namespace basecross
{
	enum Switching
	{
		off,
		on
	};

	class TitleStage : public StageCSV, public ResourcesManager
	{
	private:
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		// 地面のスケール
		const Vec3 m_groundScale = Vec3(+50.0f, +2.0f, +50.0f);

		// カメラの位置・視点
		const Vec3 m_cameraEye = Vec3(25.0f, 20.0f, -47.0f);
		const Vec3 m_cameraAt = Vec3(25.0f, 2.0f, -25.0f);

		// フェードの時間
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;
		
		const float m_width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float m_height = static_cast<float>(App::GetApp()->GetGameHeight());

		Vec3 m_diff;
		float m_distance;

		// ボタンが押されたか
		bool m_buttonPush;

		bool m_zooming;

		shared_ptr<Sprite> m_fadeSprite;

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

		/*
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*
		@brief リソースの読込
		*/
		void CreateResourses();

		/*!
		@brief リソースの解放
		*/
		void ReleasedResourses();

		/*
		@brief スプライトの生成
		*/
		void CreateSprite();
		
		/*
		@brief オープニング画面の生成
		*/
		void CreateOpningScreen();

		/*
		@brief 地面の生成
		*/
		void CreateGround();

		/*
		@brief タイトル用のプレイヤー生成
		*/
		void CreatePlayer();

		/*
		@brief 建物の生成
		*/
		void CreateBuilding();

		/*
		@brief カメラのズーム処理
		*/
		void TitleCameraZoom();

		/*
		@brief スプライトのフェード処理
		*/
		void FadeSprite();

		/*
		@brief ボタンを押した時の処理
		*/
		void PushButtonX();

		/*
		@brief オブジェクトとプレイヤーの距離
		*/
		void DistanceToPlayer();

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		void CreateStageCSV(string csvPath = "TestStage") override
		{
		}

	public:
		/*
		@brief コンストラクタ
		*/
		TitleStage() : StageCSV(), ResourcesManager()
		{
			m_buttonPush = false;
			m_zooming = false;

			m_objectGroup = CreateSharedObjectGroup(L"Settings");
		}

		/*
		@brief デストラクタ
		*/
		virtual ~TitleStage() {};

		/*
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 破棄される時に一度だけ呼び出される関数
		*/
		virtual void OnDestroy() override;

		/*
		@brief 毎フレーム呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;

		wstring GetTagName()
		{
			return tagName;
		}
	};
}