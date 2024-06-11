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
		AddTextureResource(L"EASY_TX", modelPath + L"EasyMapTexture.tga");
		AddTextureResource(L"NORMAL_TX", modelPath + L"NormalMapTexture.tga");
		AddTextureResource(L"HARD_TX", modelPath + L"HardMapTexture.tga");

		// マップのテクスチャ
		AddTextureResource(L"EASYMAP_TX", texturePath + L"MapEasy.png");
		AddTextureResource(L"NORMALMAP_TX", texturePath + L"MapNormal.png");
		AddTextureResource(L"HARDMAP_TX", texturePath + L"MapHard.png");

		// 地面のテクスチャ
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		AddAudioResource(L"TITLE_BGM", soundPath + L"Title");

		AddedTextureResources();
		AddedAudioResources();
	}

	void TitleStage::StartBGM()
	{
		m_soundManager->StartBGM(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.5f, ThisPtr);
	}

	// オープニング画面の生成
	void TitleStage::CreateOpningScreen()
	{
		auto& opning = AddGameObject<TitleLogo>();
	}

	// 地面の生成
	void TitleStage::CreateGround()
	{		
		// 床ボックスオブジェクトの追加
		auto& ground = AddGameObject<GroundBox>(Vec3(m_cameraAt.x, 0.0f, m_cameraAt.z), m_groundScale);
		ground->SetDrawActive(false);
		SetSharedGameObject(L"TitleGround", ground);

		AddGameObject<GroundManager>();
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

	void TitleStage::CreateGatheringManager()
	{
		const auto& gatheringManager = AddGameObject<GatheringManager>();
		SetSharedGameObject(L"GatheringManager", gatheringManager);
	}

	// プレイヤーの生成
	void TitleStage::CreatePlayer()
	{
		Vec3 startPos = Vec3(m_cameraAt.x, 2.0f, m_cameraAt.z);
		auto& player = AddGameObject<GamePlayer>(startPos, Vec3(0.0f));
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

		// 看板の生成
		const auto& board = AddGameObject<SignBoard>();
		SetSharedGameObject(L"Board", board);
		m_objectGroup->IntoGroup(board);
		
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

	// ボタンを押した時の処理
	void TitleStage::PushButtonB()
	{
		if (titleProgress == usually)
		{
			titleProgress = push;
		}

		if (titleProgress == select)
		{
			titleProgress = usually;
		}
	}

	// カメラのズーム処理
	void TitleStage::TitleCameraZoom()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj&& !m_zooming)
		{
			titleCamera->SetTargetObject(player);
			titleCamera->ZoomStart(titleCamera->GetEye());
			m_zooming = true;
		}

		if (titleProgress == usually)
		{
			titleCamera->SetEye(m_cameraEye);
			titleCamera->SetAt(m_cameraAt);
			m_zooming = false;
		}
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
		else if(titleProgress == usually || titleProgress == push)
		{
			m_fadeSprite->SetDiffuseColor(COL_ALPHA);
		}
	}

	void TitleStage::Progress(shared_ptr<GameObject>& obj)
	{
		if (obj)
		{
			if (obj->FindTag(L"GAMESTART"))
			{
				titleProgress = start;
				return;
			}

			titleProgress = zoom;
			return;
		}
	}

	// オブジェクトとプレイヤーの距離
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

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
				}

				Progress(m_selectObj);
			}

			if (!m_selectObj)
			{
				titleProgress = usually;
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
			if (Input::GetPad().wPressedButtons & XINPUT_GAMEPAD_B)
			{
				PushButtonB();
			}

			Debug::Log(L"カメラのAt : ", GetView()->GetTargetCamera()->GetAt());

			Debug::Log(L"列車の位置 : ", GetSharedGameObject<TitleTrain>(L"TitleTrain", true)->GetPosition());

			if (titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(titleProgress == usually)
			{
				if (m_selectObj && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
			}

			TitleCameraZoom();

			FadeSprite();
		}
		catch (...)
		{
			throw;
		}
	}
}