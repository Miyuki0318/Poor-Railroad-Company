/*!
@file TitleStage.h
@brief タイトルステージ
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"

namespace basecross
{
	enum eTitleProgress
	{
		normal, // 通常
		push,   // ボタン押す
		select, // オブジェクト選択
		start   // ゲームスタート
	};

	class TitleStage : public BaseStage
	{
	private:
		// タグ名取得
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		// 地面のスケール
		const Vec3 m_groundScale = Vec3(+50.0f, +2.0f, +50.0f);

		// カメラの位置・視点
		const Vec3 m_cameraEye = Vec3(25.0f, 20.0f, -47.0f);
		const Vec3 m_cameraAt = Vec3(25.0f, 2.0f, -25.0f);

		// 列車の開始位置
		const Vec3 m_trainPos = Vec3(35.0f,1.0f,-29.0f);

		// フェードの時間
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;
		
		const float m_width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float m_height = static_cast<float>(App::GetApp()->GetGameHeight());

		eTitleProgress titleProgress;

		Vec3 m_diff;
		float m_distance;

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
		void CreateResourses() override;

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
		@brief レール管理クラスの生成
		*/
		void CreateRailManager();

		/*
		@brief 建物の生成
		*/
		void CreateBuilding();

		/*
		@brief 列車の生成
		*/
		void CreateTrain();

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
		@brief 状態を変化させる処理
		*/
		void Progress(shared_ptr<GameObject>& obj);

		/*
		@brief オブジェクトとプレイヤーの距離
		*/
		void DistanceToPlayer();

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		void CreateStageCSV(string csvPath = "Title") override;

	public:
		/*
		@brief コンストラクタ
		*/
		TitleStage() : BaseStage()
		{
			titleProgress = eTitleProgress::normal;

			//m_buttonPush = false;
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

	};
}