/*!
@file TitleStage.cpp
@brief タイトルステージの実体
@author 新妻葵
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Input;

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
		wstring modelPath = mediaPath + L"Models/" + L"Bilding/";
		
		// タイトルロゴ
		AddTextureResource(L"TITLE_LOGO", texturePath + L"TitleLogo.png");

		// 開発開始のテクスチャ
		AddTextureResource(L"START_TX", texturePath + L"PleaseButton.png");

		// ボードのテクスチャ
		AddTextureResource(L"BOARD_TX", modelPath + L"RouteMapTexture.tga");

		// マップ選択モデルのテクスチャ
		AddTextureResource(L"FIRST_TX", modelPath + L"FirstMapTexture.tga");
		AddTextureResource(L"SECOND_TX", modelPath + L"SecondMapTexture.tga");
		AddTextureResource(L"THIRD_TX", modelPath + L"ThirdMapTexture.tga");
		AddTextureResource(L"FOURTH_TX", modelPath + L"FourthMapTexture.tga");
		AddTextureResource(L"FIFTH_TX", modelPath + L"FifthMapTexture.tga");

		// タイトルBGM
		AddAudioResource(L"TITLE_BGM", soundPath + L"Title");

		AddedTextureResources();
		AddedAudioResources();
	}

	// BGM開始関数
	void TitleStage::StartBGM()
	{
		m_bgmItem = m_soundManager->StartBGM(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.5f, ThisPtr);
	}

	// タイトルロゴの生成
	void TitleStage::CreateOpningScreen()
	{
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
		Vec3 startPos = Vec3(m_cameraAt.x, 2.0f, m_cameraAt.z);
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(startPos);
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
		// 会社の生成
		const auto& company = AddGameObject<Company>();
		SetSharedGameObject(L"Company", company);
		m_objectGroup->IntoGroup(company);
		
		// 路線図の生成
		const auto& routeMap = AddGameObject<RouteMap>();
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

	// 看板の生成
	void TitleStage::CreateSignBoard()
	{
		for (int i = 0; i < m_boardQuantity; i++)
		{
			AddGameObject<SignBoard>(m_textureKeys[i], m_boardPositions[i]);
		}
	}

	// 所持金UIの生成
	void TitleStage::CreateUISprite()
	{
		const float scale = 75.0f;
		const Vec3 position = Vec3(650.0f, 460.0f, 0.2f);
		AddGameObject<MoneyCountUI>(scale, position);
	}

	// ボタンを押した時の処理
	void TitleStage::PushButtonB()
	{
		if (titleProgress == normal)
		{
			titleProgress = push;
		}

		if (titleProgress == select || titleProgress == zoom)
		{
			titleProgress = normal;
		}
	}

	// カメラのズーム処理
	void TitleStage::TitleCameraZoom()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj && !m_zooming)
		{
			m_zooming = true;
			
			Vec3 objPos = m_selectObj->GetComponent<Transform>()->GetPosition();
			titleCamera->SetTargetObject(m_selectObj);

			bool isTrain = bool(dynamic_pointer_cast<TitleTrain>(m_selectObj));
			objPos += isTrain ? m_trainDiffEye : m_objDiffEye;

			titleCamera->ZoomStart(objPos);
		}

		if (titleProgress == normal)
		{
			titleCamera->ZoomEnd();
			//CameraReset();
		}
	}

	void TitleStage::CameraReset()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		titleCamera->SetEye(m_cameraEye);
		titleCamera->SetAt(m_cameraAt);
		titleCamera->SetTargetObject(nullptr);
		m_zooming = false;
	}

	// スプライトのフェード処理
	void TitleStage::FadeSprite()
	{
		if (!m_fadeSprite) return;

		if (titleProgress == zoom)
		{
			if (m_fadeSprite->FadeInColor(2.0f))
			{
				titleProgress = select;
			}
		}
		else if (titleProgress == start)
		{
			m_fadeSprite->FadeInColor(2.0f);
		}
		else if(titleProgress == normal || titleProgress == push)
		{
			m_fadeSprite->SetDiffuseColor(COL_ALPHA);
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

				if (!m_selectObj->FindTag(tagName))
				{
					m_selectObj->AddTag(tagName);
					titleProgress = zoom;
				}

				player->SetState(TitlePlayerPauseState::Instance());
			}


			if (!m_selectObj)
			{
				titleProgress = normal;
			}
		}
	}

	// 実行時、一度だけ処理される関数
	void TitleStage::OnCreate()
	{
		try
		{
			BaseStage::OnCreate();

			CreateViewLight();

			CreateOpningScreen();

			CreatePlayer();

			CreateStageCSV(m_stagePath);

			CreateRailManager();

			CreateGatheringManager();

			CreateGround();

			CreateBuilding();

			CreateTrain();

			CreateSignBoard();
			
			CreateUISprite();
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
			if (Input::GetPushB())
			{
				PushButtonB();
			}
			
			auto& camera = GetView()->GetTargetCamera();
			auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);
			
			if (titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(titleProgress == normal || titleProgress == move)
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

			m_fadeSprite->SetDrawActive(false);

			// 通常時以外は演出中のフラグを立てる
			m_isStaging = titleProgress != normal;

			Debug::Log(m_boardQuantity);

		}
		catch (...)
		{
			throw;
		}
	}
}