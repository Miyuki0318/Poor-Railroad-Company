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

		// フェード用のテクスチャ
		AddTextureResource(L"FADE_TX", texturePath + L"Blue.png");

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

		AddedTextureResources();
		AddedAudioResources();
	}

	// スプライトの生成
	void TitleStage::CreateSprite()
	{
		auto& sprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(m_width, m_height));
		sprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", sprite);
	}

	// オープニング画面の生成
	void TitleStage::CreateOpningScreen()
	{
		//auto& opning = AddGameObject<TitleLogo>();
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

		// オブジェクトグループ
		const auto& miningGroup = GetSharedObjectGroup(L"MiningObject");

		// 二重ループ
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// 仮オブジェクト
				shared_ptr<MiningObject> mining = nullptr;

				// 座標(左限+列番号×スケール,下限+行数-行番号×スケール)
				Vec3 position = LINE2POS(i, j);

				// 数値の別名
				eStageID num = STAGE_ID(m_stageMap.at(i).at(j));
				switch (num)
				{
				case eStageID::Stone: // 岩なら
					mining = AddGameObject<Rock>(position, 2);
					break;

				case eStageID::Tree: // 木なら
					mining = AddGameObject<Tree>(position, 2);
					break;

				default:
					break;
				}

				// オブジェクトグループへの追加
				if (mining)
				{
					miningGroup->IntoGroup(mining);
				}

				tempVec.push_back(position);
			}
			m_positionMap.push_back(tempVec);
		}
	}

	// プレイヤーの生成
	void TitleStage::CreatePlayer()
	{
		auto& player = AddGameObject<GamePlayer>();
		player->SetPosition(Vec3(m_cameraAt.x, 5.0f, m_cameraAt.z));
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

		// 工事現場の生成
		const auto& construction = AddGameObject<Construction>();
		SetSharedGameObject(L"Construction", construction);
		m_objectGroup->IntoGroup(construction);
		
		// 看板の生成
		const auto& board = AddGameObject<SignBoard>();
		SetSharedGameObject(L"Board", board);
		m_objectGroup->IntoGroup(board);
		
		// 路線図の生成
		const auto& routeMap = AddGameObject<RouteMap>();
		SetSharedGameObject(L"RouteMap", routeMap);
		m_objectGroup->IntoGroup(routeMap);
	}

	void TitleStage::CreateTrain()
	{
		const auto& train = AddGameObject<TitleTrain>(m_trainPos);
		SetSharedGameObject(L"TitleTrain", train);
		//m_objectGroup->IntoGroup(train);
	}

	// ボタンを押した時の処理
	void TitleStage::PushButtonX()
	{
		if (!m_buttonPush)
		{
			m_buttonPush = true;
		}
		else if (m_buttonPush)
		{
			m_buttonPush = false;
		}
	}

	// カメラのズーム処理
	void TitleStage::TitleCameraZoom()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj != NULL && !m_zooming)
		{
			//titleCamera->SetTargetObject(player);
			//titleCamera->ZoomStart(titleCamera->GetEye());
			m_zooming = true;
		}

		if (!m_buttonPush)
		{
			//titleCamera->SetEye(m_cameraEye);
			//titleCamera->SetAt(m_cameraAt);
			m_zooming = false;
		}
	}

	// スプライトのフェード処理
	void TitleStage::FadeSprite()
	{
		auto sprite = GetSharedGameObject<Sprite>(L"FadeSprite", true);
		
		if (m_zooming)
		{
			sprite->FadeInColor(2.0f);
		}
		else
		{
			sprite->SetDiffuseColor(COL_ALPHA);
		}
	}

	// オブジェクトとプレイヤーの距離
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		// 範囲for文でグループに所属しているオブジェクト数ループさせる
		for (auto v : m_objectGroup->GetGroupVector())
		{
			// オブジェクトをロック
			auto target = v.lock();
			auto targetPos = target->GetComponent<Transform>()->GetPosition();

			// オブジェクトとプレイヤーの距離を求める
			m_diff = targetPos - playerPos;
			m_distance = m_diff.length();

			if (m_distance < 2.5f)
			{
				m_selectObj = target;
				if (!m_selectObj->FindTag(tagName))
				{
					m_selectObj->AddTag(tagName);
				}
			}
		}

		if (m_selectObj == NULL)
		{
			m_buttonPush = false;
		}
	}

	// 実行時、一度だけ処理される関数
	void TitleStage::OnCreate()
	{
		try
		{
			BaseStage::OnCreate();

			CreateViewLight();

			CreateResourses();

			CreateSprite();

			CreateOpningScreen();

			CreatePlayer();

			CreateStageCSV("Title");

			CreateRailManager();

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
				PushButtonX();
			}

			Debug::Log(L"カメラのAt : ", GetView()->GetTargetCamera()->GetAt());

			Debug::Log(L"列車の位置 : ", GetSharedGameObject<TitleTrain>(L"TitleTrain", true)->GetPosition());

			if (m_buttonPush)
			{
				DistanceToPlayer();
			}
			else
			{
				if (m_selectObj != NULL && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
			}

			TitleCameraZoom();

			FadeSprite();

			Debug::Log(m_buttonPush);
		}
		catch (...)
		{
			throw;
		}
	}

	// 描画処理
	void TitleStage::OnDraw()
	{
		// アプリケーションオブジェクトの取得
		const auto& app = App::GetApp();

		// デバッグ文字列を強制的に空にする
		app->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}