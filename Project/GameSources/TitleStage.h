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
		opening,// オープニング
		normal, // 通常
		push,   // ボタン押す
		zoom, // ズームイン
		select, // オブジェクト選択
		shop, // ショップ画面中
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
		const Vec3 m_cameraAt = Vec3(18.0f, 2.0f, -17.0f);

		// ズーム位置に加算する差分
		const Vec3 m_shopDiffEye;
		const Vec3 m_routeMapDiffEye;
		const Vec3 m_trainDiffEye;

		// 列車の開始位置
		const Vec3 m_trainPos = Vec3(23.0f,1.0f,-20.0f);

		// プレイヤーの開始座標
		Vec3 m_startPosition;
		
		// 検索範囲
		const float m_searchArea = 3.0f;

		eTitleProgress m_titleProgress;

		// 放置された時間
		float m_leaveTime;

		bool m_zooming;

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

		weak_ptr<SoundItem> m_bgmItem;

		weak_ptr<Sprite> m_pushAButton;
		weak_ptr<Sprite> m_pushBButton;

		Vec3 m_oldPlayerPos;
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
		@brief 所持金スプライトの生成
		*/
		void CreateUISprite();

		/*
		@brief 矢印UIの生成
		*/
		void CreateArrowSprite();

		/*
		@brief ボタンUIの生成
		*/
		void CreateButtonUI();

		/*
		@brief カメラのズーム処理
		*/
		void TitleCameraZoom();

		/*
		@brief スプライトのフェード処理
		*/
		void FadeSprite();

		/*
		@brief ボタンUIの表示処理
		*/
		void ButtonUIActive();

		/*
		@brief 一定時間放置したらムービーステージに移行する処理
		*/
		void ChengeMovieTime(Vec3 pos);

		/*
		@brief Aボタンを押した時の処理
		*/
		void PushButtonA();

		/*
		@brief Bボタンを押した時の処理
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
		TitleStage(const string& stagePath, eTitleProgress prog) :
			BaseStage(stagePath),
			m_titleProgress(prog),
			m_shopDiffEye(0.0f, 3.0f, -0.5f),
			m_routeMapDiffEye(0.0f, 3.0f, +11.0f),
			m_trainDiffEye(0.0f, 8.0f, 12.0f),
			m_oldPlayerPos(0.0f),
			m_zooming(false)
		{
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
			return m_titleProgress == eTitleProgress::select;
		}

		/*
		@brief ゲームスタート時にtrueを返す関数
		*/
		bool GetStartFlag() const
		{
			return m_titleProgress == eTitleProgress::start;
		}

		/*
		@brief タイトルステージの状態を返す関数
		*/
		eTitleProgress& GetTitleProgress()
		{
			return m_titleProgress;
		}

		/*
		@brief 選択されているオブジェクトを返す関数
		*/
		shared_ptr<GameObject> GetSelectObject() const
		{
			return m_selectObj;
		}

		/*
		@brief 指定したオブジェクトが選択されているか判定する関数
		*/
		bool MatchSelectObject(const shared_ptr<GameObject>& obj)
		{
			return m_selectObj == obj;
		}
	};
}