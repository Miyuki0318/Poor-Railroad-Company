/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "PauseMenu.h"

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
		AddTextureResource(L"CONTINUE_TX", texturePath + L"Continue.png");
		AddTextureResource(L"STAGESELECT_TX", texturePath + L"StageSelect.png");
		AddTextureResource(L"TITLEBACK_TX", texturePath + L"TitleBack.png");

		// 地面の仮テクスチャ
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		// ゲーム中のBGM
		AddAudioResource(L"FIRST_BGM", soundPath + L"FirstBGM");
		AddAudioResource(L"SECOND_BGM", soundPath + L"SecondBGM");
		AddAudioResource(L"THIRD_BGM", soundPath + L"ThirdBGM");
		AddAudioResource(L"FOURTH_BGM", soundPath + L"FourthBGM");
		AddAudioResource(L"FIFTH_BGM", soundPath + L"FifthBGM");

		// 追加したリソースをメモリに追加
		AddedTextureResources();
		AddedAudioResources();
	}

	// BGMの再生
	void GameStage::CreateStartBGM()
	{
		// BGMのキーをステージパスから設定
		wstring bgmKey;
		Util::ConvertUtf8toWstring(m_stagePath, bgmKey);
			
		// BGMの再生
		m_bgmItem = m_soundManager->StartBGM(Utility::ToUpperString(bgmKey) + L"_BGM", XAUDIO2_LOOP_INFINITE, 0.0f, ThisPtr);
	}

	//ビューとライトの生成
	void GameStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		Vec3 defEye = Vec3(3.0f + m_stageDistanceX, 20.0f, -23.5f);
		Vec3 defAt = Vec3(3.0f, 1.0f, -8.5f);

		auto PtrCamera = ObjectFactory::Create<MainCamera>(MainCamera::State::Follow, defEye, defAt);
		PtrView->SetCamera(PtrCamera);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	// 床ボックスの生成
	void GameStage::CreateGroundBox()
	{
		AddGameObject<GroundManager>();	// 地面の描画生成
		AddGameObject<UnBreakRock>();	// 壊せない岩の生成
	}

	// プレイヤーの生成
	void GameStage::CreatePlayer()
	{
		// プレイヤーオブジェクトの追加
		const auto& player = AddGameObject<GamePlayer>(m_startPosition, m_goalStagingPosition);

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

				// プレイヤーの開始位置とクリア演出時の移動先を保持
				if (num == eStageID::PlayerStart)
				{
					m_startPosition = Vec3(float(j), 2.0f, -float(i));
					id = 0;
				}
				if (num == eStageID::PlayerGoal) 
				{
					m_goalStagingPosition = Vec3(float(j), 2.0f, -float(i));
					id = 0;
				}

				// 先端レールなら配列の行番号をディスタンスにする
				if (num == eStageID::DeRail)
				{
					m_stageDistanceX = float(j);
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
		Vec3 startPos = GetSharedGameObject<RailManager>(L"RailManager")->GetStartRailPos();
		const auto& train = AddGameObject<GameTrain>(startPos);

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Train", train);
	}

	// スプライトの生成
	void GameStage::CreateSpriteObject()
	{
		m_fadeSprite->SetPosition(m_fadeSprite->GetPosition() + Vec3(0.0f, 0.0f, 0.1f));
		m_fadeSprite->SetDiffuseColor(COL_WHITE);
		m_gameSprite = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(500.0f), Vec3(0.0f, 200.0f, 0.0f));

		m_ctSprite = AddGameObject<Sprite>(L"CONTINUE_TX",	m_defScale, Vec3(-300.0f, -200.0f, 0.0f));
		m_tbSprite = AddGameObject<Sprite>(L"TITLEBACK_TX", m_defScale, Vec3(300.0f, -200.0f, 0.0f));

		m_selectMap.emplace(eContinueSelect::Continue, m_ctSprite);
		m_selectMap.emplace(eContinueSelect::TitleBack, m_tbSprite);

		m_ctSprite->SetDiffuseColor(COL_ALPHA);
		m_tbSprite->SetDiffuseColor(COL_ALPHA);
	}

	// UIの生成
	void GameStage::CreateUIObject()
	{
		// パラメータ
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-910.0f, 500.0f, 0.2f);
		const Vec3 distance = Vec3(0.0f, -scale * 1.75f, 0.0f);

		// アイテム数UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_GEAR_TX", eItemType::Gear);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 4.0), L"UI_RAIL_TX", eItemType::Rail);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 5.0), L"UI_BRIDGE_TX", eItemType::WoodBridge);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 6.0), L"UI_CROSSING_TX", eItemType::Crossing);

		// アイテム取得時の跳んでくエフェクトの設定
		const auto& itemFly = GetSharedGameObject<FlyItemManager>(L"FlyItemManager");
		itemFly->SetTargetUIData(eItemType::Wood, L"UI_WOOD_TX", startPos);
		itemFly->SetTargetUIData(eItemType::Stone, L"UI_STONE_TX", startPos + distance);
		itemFly->SetTargetUIData(eItemType::Gear, L"UI_GEAR_TX", startPos + (distance * 2.0));
		itemFly->SetTargetUIData(eItemType::Rail, L"UI_RAIL_TX", startPos + (distance * 4.0));
		itemFly->SetTargetUIData(eItemType::WoodBridge, L"UI_BRIDGE_TX", startPos + (distance * 5.0));
		itemFly->SetTargetUIData(eItemType::Crossing, L"UI_CROSSING_TX", startPos + (distance * 6.0));

		// ポーズメニューの作成
		auto& pauseMenu = AddGameObject<PauseMenu>();
		SetSharedGameObject(L"PAUSE", pauseMenu);
	}

	// スプライトの表示
	void GameStage::LogoActive()
	{
		switch (m_gameProgress)
		{
		case FadeIn:
		case Playing:
		case ContinueFade:
			m_gameSprite->SetDrawActive(false);
			break;

		case GameClear:
			m_gameSprite->SetTexture(L"GAMECLEAR_TX");
			m_gameSprite->SetDrawActive(false);
			break;

		case GameOver:
			m_gameSprite->SetTexture(L"GAMEOVER_TX");
			m_gameSprite->SetDrawActive(true);
			break;

		default:
			break;
		}
	}

	void GameStage::ToFadeInState()
	{
		if (m_fadeSprite->FadeOutColor(2.0f))
		{
			m_gameProgress = eGameProgress::Playing;
		}

		float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
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

			float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
			m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
		}

		m_countTime += DELTA_TIME;
	}

	void GameStage::PushButtonStart()
	{
		if (m_gameProgress == Pause)
		{
			auto& menu = GetSharedGameObject<PauseMenu>(L"PAUSE");
			menu->OnClose();
			m_gameProgress = Playing;
		}
		if (m_gameProgress == Playing)
		{
			auto& menu = GetSharedGameObject<PauseMenu>(L"PAUSE");
			menu->OnOpen();
			m_gameProgress = Pause;
		}
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
		else
		{
			m_countTime += DELTA_TIME;
		}
	}

	// コンティニュー時のフェードイン処理
	void GameStage::ContinueFadeInState()
	{
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			m_continueState = eContinueState::SelectFade;
		}
	}

	void GameStage::ContinueSelectFadeState()
	{
		m_tbSprite->FadeInColor(0.5f);
		if (m_ctSprite->FadeInColor(0.5f))
		{
			m_continueState = eContinueState::Selecting;
		}
	}

	void GameStage::ContinueSelectingState()
	{
		float stickVal = Input::GetLStickValue().x;
		
		m_totalTime += DELTA_TIME * 2.0f;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		if (stickVal && !m_pastStickVal)
		{
			m_totalTime = 0.0f;
			m_pastState = m_currentState;
			switch (m_currentState)
			{
			case eContinueSelect::Continue:
				m_currentState = eContinueSelect::TitleBack;
				break;

			case eContinueSelect::TitleBack:
				m_currentState = eContinueSelect::Continue;
				break;

			default:
				break;
			}
		}

		m_pastStickVal = stickVal;
		float scale = Utility::SinCurve(m_totalTime, 1.0f, 1.25f);
		m_selectMap.at(m_pastState).lock()->SetScale(m_defScale);
		m_selectMap.at(m_currentState).lock()->SetScale(m_defScale * scale);

		if (Input::GetPushB()) m_continueState = eContinueState::SelectState;
	}

	// コンティニュー時の選択肢を呼び出す
	void GameStage::ContinueSelectState()
	{
		if (m_selectFunc.find(m_currentState) == m_selectFunc.end()) return;

		m_selectFunc.at(m_currentState)();
	}

	// コンティニュー時のリセット処理
	void GameStage::ResetState()
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

	// タイトルに戻る
	void GameStage::TitleBackState()
	{
		// スプライトをフェードアウト
		m_tbSprite->FadeOutColor(1.0f);
		m_ctSprite->FadeOutColor(1.0f);
		if (m_gameSprite->FadeOutColor(2.5f))
		{
			// タイトルステージに遷移
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
		}

		float volume = Utility::Lerp(0.0f, 0.5f, m_gameSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	// コンティニュー時のフェードアウト
	void GameStage::ContinueFadeOutState()
	{
		m_tbSprite->FadeOutColor(0.5f);
		m_ctSprite->FadeOutColor(0.5f);

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

			// BGMの再生
			CreateStartBGM();

			// CSVでステージを生成
			CreateStageCSV(m_stagePath);

			// プレイヤーの生成
			CreatePlayer();

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

			//ビューとライトの作成
			CreateViewLight();

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

			if (Input::GetPad().wPressedButtons & XINPUT_GAMEPAD_START)
			{
				PushButtonStart();
			}

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