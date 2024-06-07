/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// リソースの読み込み
	void GameStage::CreateResourses()
	{
		BaseStage::CreateResourses();

		// アプリケーションオブジェクトの取得
		const auto& app = App::GetApp();

		// ディレクトリパスの定義
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";

		// ゲーム結果テクスチャ
		AddTextureResource(L"GAMECLEAR_TX", texturePath + L"Win.png");
		AddTextureResource(L"GAMEOVER_TX", texturePath + L"Lose.png");

		// 地面の仮テクスチャ
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		// ゲーム中のBGM
		AddAudioResource(L"GAME_BGM", soundPath + L"GameBGM");

		// 追加したリソースをメモリに追加
		AddedTextureResources();
		AddedAudioResources();
	}

	//ビューとライトの生成
	void GameStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<MainCamera>(MainCamera::State::Follow);
		PtrView->SetCamera(PtrCamera);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	// 床ボックスの生成
	void GameStage::CreateGroundBox()
	{
		// 床ボックスオブジェクトの追加
		auto g = AddGameObject<GroundBox>(Vec3(49.5f, 0.0f, -7.0f) ,Vec3(100.0f, 2.0f, 15.0f));
		g->SetDrawActive(false);

		AddGameObject<GroundManager>();	// 地面の描画生成
		AddGameObject<UnBreakRock>();	// 壊せない岩の生成
	}

	// プレイヤーの生成
	void GameStage::CreatePlayer()
	{
		// プレイヤーオブジェクトの追加
		const auto& player = AddGameObject<GamePlayer>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Player", player);
	}

	// csvでのステージ生成
	void GameStage::CreateStageCSV(string csvPath)
	{
		// CSVデータ(int型の二次元配列)
		m_positionMap.clear();
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

	void GameStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);

		AddGameObject<RailGuide>();
	}

	void GameStage::CreateBridgeManager()
	{
		const auto& bridgeManager = AddGameObject<BridgeManager>();
		SetSharedGameObject(L"BridgeManager", bridgeManager);
	}

	void GameStage::CreateCrossingManager()
	{
		const auto& crossingManager = AddGameObject<CrossingManager>();
		SetSharedGameObject(L"CrossingManager", crossingManager);
	}

	void GameStage::CreateGatheringManager()
	{
		const auto& gatheringManager = AddGameObject<GatheringManager>();
		SetSharedGameObject(L"GatheringManager", gatheringManager);
	}

	void GameStage::CreateGearManager()
	{
		const auto& gearManager = AddGameObject<GearManager>();
		SetSharedGameObject(L"GearManager", gearManager);
	}

	// 列車の生成
	void GameStage::CreateTrain()
	{
		// 列車オブジェクトの追加
		const auto& train = AddGameObject<GameTrain>(Vec3(0.0f, 1.0f, -7.0f));

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Train", train);
	}

	// スプライトの生成
	void GameStage::CreateSpriteObject()
	{
		m_gameSprite = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(500.0f), Vec3(0.0f));
	}

	// UIの生成
	void GameStage::CreateUIObject()
	{
		// パラメータ
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-910.0f, 500.0f, 0.1f);
		const Vec3 distance = Vec3(0.0f, -scale * 1.75f, 0.0f);

		// アイテム数UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_GEAR_TX", eItemType::Gear);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 4.0), L"UI_RAIL_TX", eItemType::Rail);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 5.0), L"UI_BRIDGE_TX", eItemType::WoodBridge);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 6.0), L"UI_CROSSING_TX", eItemType::Crossing);
	}

	// スプライトの表示
	void GameStage::LogoActive()
	{
		switch (m_gameProgress)
		{
		case Playing :
		case ContinueFade:
			m_gameSprite->SetDrawActive(false);
			break;

		case GameClear:
			m_gameSprite->SetTexture(L"GAMECLEAR_TX");
			m_gameSprite->SetDrawActive(true);
			break;

		case GameOver:
			m_gameSprite->SetTexture(L"GAMEOVER_TX");
			m_gameSprite->SetDrawActive(true);
			break;
		}
	}

	void GameStage::ToTitleStage()
	{
		// フェードイン開始の条件を満たしていた場合の処理
		if (m_countTime >= m_defermentTransition) {
			// フェード用スプライトのエラーチェック
			if (!m_fadeSprite) return;

			// スプライトのフェードイン処理が終了していた場合の処理
			if (m_fadeSprite->FadeInColor(2.0f))
			{
				// タイトルステージへ遷移
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
				m_countTime = 0.0f;
			}
		}

		m_countTime += DELTA_TIME;
	}

	// コンティニュー処理
	void GameStage::ToContinueStage()
	{
		// フェードイン開始の条件を満たしていた場合の処理
		if (m_countTime >= m_defermentTransition) 
		{
			// フェード用スプライトのエラーチェック
			if (!m_fadeSprite) return;

			// コンティニュー処理を送る
			if (m_continueFunc.find(m_continueState) == m_continueFunc.end()) return;
			m_continueFunc.at(m_continueState)();
		}

		m_countTime += DELTA_TIME;
	}

	// コンティニュー時のフェードイン処理
	void GameStage::ContinueFadeInState()
	{
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			m_continueState = eContinueState::Reset;
		}
	}

	// コンティニュー時のリセット処理
	void GameStage::ContinueResetState()
	{
		// CSVでステージを生成
		CreateStageCSV(m_stagePath);

		// 各種リセット処理を送る
		const auto& railManager = GetSharedGameObject<RailManager>(L"RailManager");
		railManager->ResetInstanceRail();

		const auto& gameTrain = GetSharedGameObject<GameTrain>(L"Train");
		gameTrain->ResetTrain();

		const auto& bridgeManager = GetSharedGameObject<BridgeManager>(L"BridgeManager");
		bridgeManager->ResetBridge();

		const auto& gatheringManager = GetSharedGameObject<GatheringManager>(L"GatheringManager");
		gatheringManager->ResetGathering();

		const auto& crossingManager = GetSharedGameObject<CrossingManager>(L"CrossingManager");
		crossingManager->ResetCrossing();

		const auto& gearManager = GetSharedGameObject<GearManager>(L"GearManager");
		gearManager->ResetGears();

		const auto& player = GetSharedGameObject<GamePlayer>(L"Player");
		player->ResetPlayer();

		m_continueState = eContinueState::FadeOut;
		m_gameProgress = eGameProgress::ContinueFade;
	}

	// コンティニュー時のフェードアウト
	void GameStage::ContinueFadeOutState()
	{
		if (m_fadeSprite->FadeOutColor(2.0f))
		{
			m_gameProgress = eGameProgress::Playing;
			m_continueState = eContinueState::FadeIn;
		}
	}

	// 生成時の処理
	void GameStage::OnCreate() 
	{
		try 
		{
			// 継承元の生成時の処理
			BaseStage::OnCreate();

			//ビューとライトの作成
			CreateViewLight();

			// プレイヤーの生成
			CreatePlayer();

			// CSVでステージを生成
			CreateStageCSV(m_stagePath);

			// 線路の生成
			CreateRailManager();

			// 木の足場の生成
			CreateBridgeManager();

			// 床ボックスの生成
			CreateGroundBox();

			// 列車の生成
			CreateTrain();

			// 踏切の生成
			CreateCrossingManager();

			// 採集オブジェクトの生成
			CreateGatheringManager();

			// 歯車オブジェクトの生成
			CreateGearManager();

			// スプライトの生成
			CreateSpriteObject();

			// UIの生成
			CreateUIObject();

			// BGMの再生
			m_soundManager->StartBGM(L"GAME_BGM", XAUDIO2_LOOP_INFINITE, 0.5f, ThisPtr);

			// スカイボックスの生成
			auto& camera = GetView()->GetTargetCamera();
			auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
			AddGameObject<SkyBox>(mainCamera);

			// 列車とカメラを取得
			auto train = GetSharedGameObject<GameTrain>(L"Train");
			// メインカメラのターゲットに列車をセットする
			mainCamera->SetTargetObject(train);
			mainCamera->SetAt(train->GetDefaultPosition());
		}
		catch (...) 
		{
			throw;
		}
	}

	// 破棄される時の処理
	void GameStage::OnDestroy()
	{
		try
		{
			// 継承元の破棄時の処理
			BaseStage::OnDestroy();
			ReleasedResourses();
		}
		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム更新処理
	void GameStage::OnUpdate()
	{
		try
		{
			// 継承元の更新処理
			BaseStage::OnUpdate();

			// スプライトの表示
			LogoActive();

			// ゲームの結果に応じて処理を実行
			if (m_progressFunc.find(m_gameProgress) == m_progressFunc.end()) return;
			m_progressFunc.at(m_gameProgress)();
		}
		catch (...)
		{
			throw;
		}
	}
}