/*!
@file TitleStage.cpp
@brief タイトルステージの実体
@author 新妻葵
*/

#include "stdafx.h"
#include "Input.h"
#include "TitleStage.h"
#include "MainCamera.h"
#include "SkyBox.h"
#include "Opening.h"
#include "GroundManager.h"
#include "RailManager.h"
#include "GatheringObject.h"
#include "TitlePlayer.h"
#include "Company.h"
#include "RouteMap.h"
#include "TitleTrain.h"
#include "TitlePlayerState.h"
#include "UnBreakRock.h"
#include "MoneyCountUI.h"
#include "TitleGuide.h"
#include "Arrow.h"

namespace basecross
{
	// ビューとライトの作成
	void TitleStage::CreateViewLight()
	{
		auto ptrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto& camera = ObjectFactory::Create<MainCamera>(MainCamera::State::Fixed, m_cameraEye, m_cameraAt);
		ptrView->SetCamera(camera);
		camera->SetAt(m_cameraAt);
		camera->SetEye(m_cameraEye);

		// スカイボックスの作成
		auto& skyBox = AddGameObject<SkyBox>(camera);

		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	// リソースの読込
	void TitleStage::CreateResourses()
	{
		BaseStage::CreateResourses();

		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";
		wstring routePath = mediaPath + L"Models/" + L"RouteMap/" ;
		wstring modelPath = mediaPath + L"Models/" + L"Bilding/";
		
		// タイトルロゴ
		AddTextureResource(L"TITLE_LOGO", texturePath + L"TitleLogo.png");

		// 開発開始のテクスチャ
		AddTextureResource(L"START_TX", texturePath + L"PleaseButton.png");

		// 矢印のテクスチャ
		AddTextureResource(L"RIGHTARROW_TX", texturePath + L"RightArrow.png");
		AddTextureResource(L"LEFTARROW_TX", texturePath + L"LeftArrow.png");
		AddTextureResource(L"D_RIGHTARROW_TX", texturePath + L"D_RightArrow.png");
		AddTextureResource(L"A_LEFTARROW_TX", texturePath + L"A_LeftArrow.png");

		
		// ボードのテクスチャ
		AddTextureResource(L"BOARD_TX", routePath + L"MapTexture.png");

		// マップ選択モデルのテクスチャ
		AddTextureResource(L"FIRST_TX", routePath + L"FirstMapTexture.png");
		AddTextureResource(L"SECOND_TX", routePath + L"SecondMapTexture.png");
		AddTextureResource(L"THIRD_TX", routePath + L"ThirdMapTexture.png");
		AddTextureResource(L"FOURTH_TX", routePath + L"FourthMapTexture.png");
		AddTextureResource(L"FIFTH_TX", routePath + L"FifthMapTexture.png");

		// ガイドのテクスチャ
		AddTextureResource(L"ROUTEMAP_TX", texturePath + L"StageSelectGuide.png");
		AddTextureResource(L"START_TRAIN_TX", texturePath + L"GameStartGuide.png");
		AddTextureResource(L"COMPANIY_TX", texturePath + L"ShopGuide.png");

		// ボタンUI
		AddTextureResource(L"PLEASEBUTTON_TX", texturePath + L"PleaseButton.png");
		AddTextureResource(L"TITLEBACK_TX", texturePath + L"TitleBack.png");
		AddTextureResource(L"CLICK_M1_TX", texturePath + L"ClickM1.png");
		AddTextureResource(L"CLICK_M2_TX", texturePath + L"ClickM2.png");

		// タイトルBGM
		AddAudioResource(L"TITLE_BGM", soundPath + L"Title");

		AddedTextureResources();
		AddedAudioResources();
	}

	// BGM開始関数
	void TitleStage::StartBGM()
	{
		if (m_bgmItem.lock()) return;
		m_bgmItem = m_soundManager->StartBGM(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.0f, ThisPtr);
	}

	// タイトルロゴの生成
	void TitleStage::CreateOpningScreen()
	{
		if (m_titleProgress != opening) return;

		auto& opning = AddGameObject<TitleLogo>();
		SetSharedGameObject(L"TitleLogo", opning);
	}

	// 地面の生成
	void TitleStage::CreateGround()
	{		
		const float scale = 5.5f;
		AddGameObject<GroundManager>(scale);
		AddGameObject<UnBreakRock>();	// 壊せない岩の生成
	}

	// csvでのステージ生成
	void TitleStage::CreateStageCSV(string csvPath)
	{
		// CSVデータ(int型の二次元配列)
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// 二重ループ
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// IDを取得
				int& id = m_stageMap.at(i).at(j);
				eStageID num = STAGE_ID(id);

				// 座標(左限+列番号×スケール,下限+行数-行番号×スケール)
				tempVec.push_back(ROWCOL2POS(i, j));

				// 石のIDなら
				if (num == eStageID::Stone1)
				{
					int random = Utility::RangeRand(3, 0);
					id = id + random;
				}
				
				// 木のIDなら
				if (num == eStageID::Tree1)
				{
					int random = Utility::RangeRand(2, 0);
					id = id + random;
				}

				// プレイヤーの開始位置とクリア演出時の移動先を保持
				if (num == eStageID::PlayerStart)
				{
					m_startPosition = Vec3(float(j), 2.0f, -float(i));
					id = 0;
				}
			}
			m_positionMap.push_back(tempVec);
		}
	}

	// 採集オブジェクトの生成
	void TitleStage::CreateGatheringManager()
	{
		const auto& gatheringManager = AddGameObject<GatheringManager>();
		SetSharedGameObject(L"GatheringManager", gatheringManager);
	}

	// プレイヤーの生成
	void TitleStage::CreatePlayer()
	{
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(m_startPosition);
		SetSharedGameObject(L"Player", player);
	}

	// レール管理クラスの生成
	void TitleStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);
	}

	// 建物の生成
	void TitleStage::CreateBuilding()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();

		// 会社の生成
		const auto& company = AddGameObject<Company>();
		SetSharedGameObject(L"Company", company);
		m_objectGroup->IntoGroup(company);

		// 路線図の生成
		const auto& routeMap = AddGameObject<RouteMap>(scene->GetMapLevel());
		SetSharedGameObject(L"RouteMap", routeMap);
		m_objectGroup->IntoGroup(routeMap);
	}

	// 列車の生成
	void TitleStage::CreateTrain()
	{
		const auto& train = AddGameObject<TitleTrain>(m_trainPos);
		SetSharedGameObject(L"TitleTrain", train);
		m_objectGroup->IntoGroup(train);
	}

	// 所持金UIの生成
	void TitleStage::CreateUISprite()
	{
		const float scale = 75.0f;
		const Vec3 position = Vec3(650.0f, 460.0f, 0.2f);
		auto& moneyUI = AddGameObject<MoneyCountUI>(scale, position);
		SetSharedGameObject(L"MoneyUI", moneyUI);

		AddGameObject<TitleGuide>();
	}

	// 矢印の生成
	void TitleStage::CreateArrowSprite()
	{
		const auto& arrow = AddGameObject<SelectArrow>();
		SetSharedGameObject(L"SelectArrow", arrow);
	}

	// ボタンUIの生成
	void TitleStage::CreateButtonUI()
	{
		// ボタン生成のパラメータ
		const Vec2 scale = Vec2(350.0f, 100.0f);
		const float posX = 1920.0f / 4.0f;
		const float posY = (1080.0f / 2.0f) - scale.y;

		const Vec2 buttonScale = Vec2(70.0f);
		const Vec3 pushAPos = Vec3(+260.0f, -posY, 0.0f);
		const Vec3 pushBPos = Vec3(-700.0f, -posY, 0.0f);


		m_backButton = AddGameObject<Sprite>(L"TITLEBACK_TX", scale, Vec3(posX, -posY, 0.0f));
		m_pleaseButton = AddGameObject<Sprite>(L"PLEASEBUTTON_TX", scale, Vec3(-posX, -posY, 0.0f));
		if (Input::GetPadConected())
		{
			// ゲームパッド用
			m_pushAButton = AddGameObject<Sprite>(L"BUTTON_A_TX", buttonScale, pushAPos);
			m_pushBButton = AddGameObject<Sprite>(L"BUTTON_B_TX", buttonScale, pushBPos);
		}
		else
		{
			// キーボード用
			m_pushAButton = AddGameObject<Sprite>(L"CLICK_M2_TX", buttonScale, pushAPos);
			m_pushBButton = AddGameObject<Sprite>(L"CLICK_M1_TX", buttonScale, pushBPos);
		}

		ButtonUIActive(false);
	}

	// Aボタンを押した時の処理
	void TitleStage::PushButtonA()
	{
		if (m_titleProgress == select)
		{
			m_titleProgress = normal;
		}
	}

	// Bボタンを押した時の処理
	void TitleStage::PushButtonB()
	{
		if (m_titleProgress == opening) return;

		const auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (titleCamera->m_cameraState == MainCamera::Fixed)
		{
			m_titleProgress = push;
		}
	}

	// カメラのズーム処理
	void TitleStage::TitleCameraZoom()
	{
		const auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj && !m_zooming)
		{
			m_zooming = true;
			
			Vec3 objPos = m_selectObj->GetComponent<Transform>()->GetPosition();
			titleCamera->SetTargetObject(m_selectObj);

			// 列車選択中？
			bool isTrain = bool(dynamic_pointer_cast<TitleTrain>(m_selectObj));
			// 路線図選択中？
			bool isRouteMap = bool(dynamic_pointer_cast<RouteMap>(m_selectObj));
			objPos += isTrain ? m_trainDiffEye : isRouteMap ? m_routeMapDiffEye : m_shopDiffEye;

			titleCamera->ZoomStart(objPos, 2.75f);
		}

		if (Utility::OR(m_titleProgress, normal, opening))
		{
			titleCamera->ZoomEnd();
		}

		if (m_titleProgress == zoom)
		{
			if (titleCamera->m_cameraState == MainCamera::ZoomedIn)
			{
				m_titleProgress = select;
			}
		}
	}

	// ボタンUIの表示処理
	void TitleStage::ButtonUIActive(bool active)
	{
		m_backButton.lock()->SetDrawActive(active);
		m_pleaseButton.lock()->SetDrawActive(active);
		m_pushAButton.lock()->SetDrawActive(active);
		m_pushBButton.lock()->SetDrawActive(active);
	}

	// スプライトのフェード処理
	void TitleStage::FadeSprite()
	{
		const auto& train = GetSharedGameObject<TitleTrain>(L"TitleTrain");

		if (m_titleProgress == opening)
		{
			m_fadeSprite->FadeOutColor(1.0f);
		}

		if (Utility::OR(m_titleProgress, normal, push))
		{
			if (m_fadeSprite->GetDiffuseColor() != Col4(COL_ALPHA))
			{
				m_fadeSprite->FadeOutColor(1.0f);
			}
		}

		if (MatchSelectObject(train))
		{
			if (m_fadeSprite->GetDiffuseColor() == COL_WHITE)
			{
				m_titleProgress = select;
			}
			m_fadeSprite->FadeInColor(1.0f);
		}

		if (m_titleProgress == start)
		{
			m_fadeSprite->FadeInColor(2.0f);
		}
	}

	// オブジェクトとプレイヤーの距離
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<TitlePlayer>(L"Player");
		Vec3 playerPos = player->GetPosition();

		// 範囲for文でグループに所属しているオブジェクト数ループさせる
		for (auto& v : m_objectGroup->GetGroupVector())
		{
			// オブジェクトをロック
			auto target = v.lock();
			auto targetPos = target->GetComponent<Transform>()->GetPosition();

			// オブジェクトとプレイヤーの距離を求める
			m_diff = targetPos - playerPos;
			m_distance = m_diff.length();

			if (m_distance < m_searchArea)
			{
				m_selectObj = target;

				// オブジェクトに特定のタグが無かったら…
				if (!m_selectObj->FindTag(tagName))
				{
					m_selectObj->AddTag(tagName);
					m_titleProgress = zoom;
				}

				player->SetState(TitlePlayerPauseState::Instance());
			}

			if (!m_selectObj)
			{
				m_titleProgress = normal;
			}
		}
	}

	// 実行時、一度だけ処理される関数
	void TitleStage::OnCreate()
	{
		try
		{
			BaseStage::OnCreate();

			m_fadeSprite->SetDiffuseColor(COL_WHITE);

			CreateViewLight();

			CreateOpningScreen();

			CreateStageCSV(m_stagePath);

			CreatePlayer();

			CreateRailManager();

			CreateGatheringManager();

			CreateGround();

			CreateBuilding();

			CreateTrain();

			CreateUISprite();

			CreateArrowSprite();

			CreateButtonUI();
		}
		catch (...)
		{
			throw;
		}
	}

	// 破棄される時の処理
	void TitleStage::OnDestroy()
	{
		try
		{
			BaseStage::OnDestroy();
			ReleasedResourses();
		}
		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム実行される関数
	void TitleStage::OnUpdate()
	{
		try 
		{
			Debug::Log(L"所持金 : ", GetMoney());
			const auto& camera = GetView()->GetTargetCamera();
			auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

			if (m_bgmItem.lock() && Utility::OR(m_titleProgress, opening, normal))
			{
				auto& item = m_bgmItem.lock()->m_SourceVoice;
				
				float volume = 0.0f;
				item->GetVolume(&volume);

				if (volume <= m_bgmVolume)
				{
					item->SetVolume(volume + DELTA_TIME);
				}
			}

			if (Input::GetPushB())
			{
				PushButtonB();
			}

			if (Input::GetPushA())
			{
				PushButtonA();
			}
			
			if (m_titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(Utility::OR(m_titleProgress,normal,move))
			{
				if (m_selectObj && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
				m_zooming = false;
			}

			TitleCameraZoom();

			FadeSprite();

			bool isMatch = MatchSelectObject(GetSharedGameObject<RouteMap>(L"RouteMap"));

			if (titleCamera->m_cameraState == MainCamera::ZoomedIn && isMatch)
			{
				ButtonUIActive(true);
			}
			else
			{
				ButtonUIActive(false);
			}

			// 通常時以外は演出中のフラグを立てる
			m_isStaging = m_titleProgress != normal;

			m_oldProgress = m_titleProgress;
		}
		catch (...)
		{
			throw;
		}
	}
}