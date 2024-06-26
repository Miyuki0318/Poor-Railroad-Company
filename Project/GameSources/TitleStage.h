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
		zoom,   // ズーム
		select, // オブジェクト選択
		move,	// 列車に移動中
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
		const Vec3 m_cameraEye = Vec3(18.0f, 12.5f, -30.0f);
		const Vec3 m_cameraAt = Vec3(18.0f, 2.0f, -18.0f);

		// ズーム位置に加算する差分
		const Vec3 m_objDiffEye;
		const Vec3 m_trainDiffEye;

		// 列車の開始位置
		const Vec3 m_trainPos = Vec3(23.0f,1.0f,-20.0f);

		// フェードの時間
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;
		
		const float m_searchArea = 3.0f;

		wstring m_textureKeys[3] = {
			L"SIGNBOARD_OFFICIAL_TX",
			L"SIGNBOARD_SELECT_TX",
			L"SIGNBOARD_START_TX"
		};

		Vec3 m_boardPositions[3] = {
			Vec3( 9.5f,1.0f,-10.0f),
			Vec3(18.5f,1.0f,-9.0f),
			Vec3(23.0f,1.0f,-14.5f),
		};

		const int m_boardQuantity = sizeof(m_textureKeys) / sizeof(m_textureKeys[0]);

		eTitleProgress titleProgress;

		Vec3 m_diff;
		float m_distance;

		bool m_zooming;

		bool m_zoomEnd;

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

		weak_ptr<SoundItem> m_bgmItem;

		/*
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*
		@brief リソースの読込
		*/
		void CreateResourses() override;
		
		/*
		@brief タイトルロゴの生成
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

		/*!
		@brief 採集オブジェクトの生成
		*/
		void CreateGatheringManager();

		/*
		@brief 列車の生成
		*/
		void CreateTrain();

		/*
		@brief 看板の生成
		*/
		void CreateSignBoard();
				
		/*
		@brief 所持金スプライトの生成
		*/
		void CreateUISprite();

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
		void PushButtonB();

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		void CreateStageCSV(string csvPath = "Title") override;

	public:
		/*
		@brief コンストラクタ
		*/
		TitleStage(const string& stagePath) : 
			BaseStage(stagePath),
			m_objDiffEye(0.0f, 3.0f, -0.5f),
			m_trainDiffEye(0.0f, 8.0f, 12.0f)
		{
			titleProgress = eTitleProgress::normal;

			m_zooming = false;

			m_zoomEnd = false;

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

		/*
		@brief BGM開始関数
		*/
		void StartBGM();

		/*
		@brief オブジェクトとプレイヤーの距離
		*/
		void DistanceToPlayer();

		/*
		@brief カメラの初期化
		*/
		void CameraReset();

		/*
		@brief bgmのサウンドアイテムを取得する関数
		@return m_bgmItem
		*/
		weak_ptr<SoundItem>& GetBGMItem()
		{
			return m_bgmItem;
		}

		/*
		@brief オブジェクト選択時にtrueを返す関数
		*/
		bool GetPlayerStop() const
		{
			return titleProgress == eTitleProgress::select;
		}

		/*
		@brief ゲームスタート時にtrueを返す関数
		*/
		bool GetStartFlag() const
		{
			return titleProgress == eTitleProgress::start;
		}

		/*
		@brief タイトルステージの状態を返す関数
		*/
		eTitleProgress& GetTitleProgress()
		{
			return titleProgress;
		}

		shared_ptr<GameObject> GetSelectObject() const
		{
			return m_selectObj;
		}
	};
}