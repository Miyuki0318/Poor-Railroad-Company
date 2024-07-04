/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "BridgeManager.h"
#include "Crossing.h"
#include "GameTrain.h"
#include "GameStage.h"
#include "GamePlayer.h"
#include "GatheringObject.h"
#include "GatherTreasure.h"
#include "Gear.h"
#include "GoalGuide.h"
#include "GroundManager.h"
#include "ItemCountUI.h"
#include "MainCamera.h"
#include "PauseMenu.h"
#include "RailGuide.h"
#include "RailManager.h"
#include "SkyBox.h"
#include "UnBreakRock.h"

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
		AddTextureResource(L"CONTINUE_TITLEBACK_TX", texturePath + L"TitleBack.png");
		AddTextureResource(L"NEXTSTAGE_TX", texturePath + L"NextStage.png");
		AddTextureResource(L"CLEAR_TITLEBACK_TX", texturePath + L"ClearTitleBack.png");
		AddTextureResource(L"OVER_CONTINUE_TX", texturePath + L"ContinueTrain.png");
		AddTextureResource(L"OVER_TITLEBACK_TX", texturePath + L"TitleBackTrain.png");
		AddTextureResource(L"RAIL_LINE_TX", texturePath + L"RailLine.tga");
		AddTextureResource(L"PAYMENTS_MENU_TX", texturePath + L"PaymentsMenu.tga");

		// ゴールガイドテクスチャ
		AddTextureResource(L"GOAL_GUIDE_TX", texturePath + L"GoalGuide.png");
		AddTextureResource(L"GOAL_ARROW_TX", texturePath + L"GoalGuideArrow.png");

		// ゲーム中のBGM
		AddAudioResource(L"FIRST_BGM", soundPath + L"FirstBGM");
		AddAudioResource(L"SECOND_BGM", soundPath + L"SecondBGM");
		AddAudioResource(L"THIRD_BGM", soundPath + L"ThirdBGM");
		AddAudioResource(L"FOURTH_BGM", soundPath + L"FourthBGM");
		AddAudioResource(L"FIFTH_BGM", soundPath + L"FifthBGM");

		// ゲーム中のSE
		AddAudioResource(L"PAUSE_OPEN_SE", soundPath + L"PauseOpen");
		AddAudioResource(L"PAUSE_CLOSE_SE", soundPath + L"PauseClose");
		AddAudioResource(L"TRAIN_DERAIL_SE", soundPath + L"DeRailed");

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
		const float scale = 1.0f;
		const auto& ground = AddGameObject<GroundManager>(scale);	// 地面の描画生成
		SetSharedGameObject(L"GroundManager", ground);
		
		const auto& treasure = AddGameObject<GatherTreasure>();
		SetSharedGameObject(L"GatherTreasure", treasure);

		const auto& unbreak = AddGameObject<UnBreakRock>();	// 壊せない岩の生成
		SetSharedGameObject(L"UnBreakRock", unbreak);
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
		m_fadeSprite->SetDiffuseColor(COL_WHITE);
		m_gameSprite = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(500.0f, 300.0f), Vec3(0.0f, 375.0f, 0.3f));

		// コンティニュー時に扱うスプライト
		m_continueSprite = AddGameObject<Sprite>(L"CONTINUE_TX", m_defScale, m_leftPos);
		m_titleBackSprite = AddGameObject<Sprite>(L"CONTINUE_TITLEBACK_TX", m_defScale, m_rightPos);
		m_continueSprite->SetDiffuseColor(COL_ALPHA);
		m_titleBackSprite->SetDiffuseColor(COL_ALPHA);

		// 煙のエフェクトオブジェクトの生成
		auto& smoke = AddGameObject<SpriteParticle>(L"SMOKE_TX");
		SetSharedGameObject(L"SmokeEffect", smoke);

		auto& star = AddGameObject<SpriteParticle>(L"STAR_TX");
		SetSharedGameObject(L"StarEffect", star);

		m_gameClearState.reset(new GameClearState(GetThis<GameStage>()));
		m_gameClearState->CreateState();

		m_gameOverState.reset(new GameOverState(GetThis<GameStage>()));
		m_gameOverState->CreateState();

		m_paymentsState.reset(new PaymentsState(GetThis<GameStage>()));
		m_paymentsState->CreateState();
	}

	// UIの生成
	void GameStage::CreateUIObject()
	{
		// パラメータ
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-900.0f, 500.0f, 0.4f);
		const Vec3 distance = Vec3(0.0f, -scale * 1.75f, 0.0f);

		// アイテム数UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_GEAR_TX", eItemType::Gear);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 4.0), L"UI_RAIL_TX", eItemType::Rail);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 5.0), L"UI_BRIDGE_TX", eItemType::WoodBridge);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 6.0), L"UI_CROSSING_TX", eItemType::Crossing);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 7.0), L"UI_GOLDBAR_TX", eItemType::GoldBar);

		// アイテム取得時の跳んでくエフェクトの設定
		const auto& itemFly = GetSharedGameObject<FlyItemManager>(L"FlyItemManager");
		itemFly->SetTargetUIData(eItemType::Wood, L"UI_WOOD_TX", startPos);
		itemFly->SetTargetUIData(eItemType::Stone, L"UI_STONE_TX", startPos + distance);
		itemFly->SetTargetUIData(eItemType::Gear, L"UI_GEAR_TX", startPos + (distance * 2.0));
		itemFly->SetTargetUIData(eItemType::Rail, L"UI_RAIL_TX", startPos + (distance * 4.0));
		itemFly->SetTargetUIData(eItemType::WoodBridge, L"UI_BRIDGE_TX", startPos + (distance * 5.0));
		itemFly->SetTargetUIData(eItemType::Crossing, L"UI_CROSSING_TX", startPos + (distance * 6.0));
		itemFly->SetTargetUIData(eItemType::GoldBar, L"UI_GOLDBAR_TX", startPos + (distance * 7.0));

		// ポーズメニューの作成
		auto& pauseMenu = AddGameObject<PauseMenu>();
		SetSharedGameObject(L"PAUSE", pauseMenu);

		// ゴールガイドの生成
		AddGameObject<GoalGuide>();
	}

	// リセット処理
	void GameStage::ResetCreateStage()
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
		player->ResetPlayer(m_startPosition, m_goalStagingPosition);
	}

	// 地面の再生成処理
	void GameStage::ResetGroundStage()
	{
		const auto& groundManager = GetSharedGameObject<GroundManager>(L"GroundManager");
		groundManager->ClearInstanceGround();
		groundManager->CreateInstanceGround();

		const auto& unbreakRock = GetSharedGameObject<UnBreakRock>(L"UnBreakRock");
		unbreakRock->CreateUnBreakRock();
	}

	void GameStage::ResetCameraObject()
	{
		auto train = GetSharedGameObject<GameTrain>(L"Train");

		Vec3 defEye = Vec3(3.0f + m_stageDistanceX, 20.0f, -23.5f);
		Vec3 defAt = Vec3(3.0f, 1.0f, -8.5f);

		auto& camera = GetView()->GetTargetCamera();
		auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
		mainCamera->ResetCamera(defEye, defAt);
	}

	// スプライトの表示
	void GameStage::LogoActive()
	{
		m_gameSprite->SetDrawActive(false);

		switch (m_gameProgress)
		{
		case GameClear:
		case ClearSelect:
		case ToNext:
		case ToTitleClear:
		case MoneyCalculation:
		case MoneyCountDown:
			m_gameSprite->SetTexture(L"GAMECLEAR_TX");
			m_gameSprite->SetDrawActive(true);
			break;

		case GameOver:
		case ToTitleOver:
		case ContinueFadeIn:
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

		float volume = 0.0f;
		m_bgmItem.lock()->m_SourceVoice->GetVolume(&volume);

		if (volume >= m_bgmVolume) return;
		volume = Utility::Lerp(m_bgmVolume, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::ToPlayingState()
	{
		if (m_fadeSprite->GetDiffuseColor().w > 0.0f)
		{
			m_fadeSprite->FadeOutColor(2.0f);
		}
	}

	void GameStage::ToMoneyCalculationState()
	{
		const auto& player = GetSharedGameObject<GamePlayer>(L"Player");
		const auto& railManager = GetSharedGameObject<RailManager>(L"RailManager");
		const auto reward = CSVLoader::LoadFile("StageClearReward");
		vector<int> moneyOperator = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("MoneyOperator")).at(0);

		int addMoney = 0;
		int count = 0;

		count = int(railManager->GetAddRailNum()) * moneyOperator.at(0);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::RailsInstallations, count * -1);
		addMoney += count;

		count = player->GetItemCount(eItemType::Wood) * moneyOperator.at(1);
		count += player->GetItemCount(eItemType::Stone) * moneyOperator.at(1);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::ResourceSales, count);
		addMoney += count;

		count = player->GetItemCount(eItemType::Rail) * moneyOperator.at(2);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::RailsSales, count);
		addMoney += count;

		count = player->GetItemCount(eItemType::GoldBar) * moneyOperator.at(3);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::GoldBarSales, count);
		addMoney += count;

		for (size_t i = 0; i < reward.size(); i++)
		{
			if (reward.at(i).at(0) == m_stagePath)
			{
				count = stoi(reward.at(i).at(1));
				break;
			}
		}

		m_paymentsState->SetNumberGoal(eGamePaymentsState::RewardCount, count);
		addMoney += count;

		AddMoney(addMoney);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::TotalIncome, addMoney);
		m_gameProgress = eGameProgress::MoneyCountDown;
	}

	void GameStage::ToMoneyCountDownState()
	{
		if (m_paymentsState->GetPaymentsState() != eGamePaymentsState::StandBy)
		{
			m_paymentsState->UpdateState();
			return;
		}

		m_paymentsState->ResetState();
		m_gameProgress = eGameProgress::ClearSelect;
	}

	void GameStage::ToClearSelectStage()
	{
		if (m_gameClearState->GetClearState() != eGameClearState::StandBy)
		{
			m_gameClearState->UpdateState();
			return;
		}

		auto select = m_gameClearState->GetSelectStage();
		m_gameProgress = select != eSelectGameClear::TitleBack ? eGameProgress::ToNext : eGameProgress::ToTitleClear;
	}

	void GameStage::ToNextStage()
	{
		// フェード用スプライトのエラーチェック
		if (!m_fadeSprite) return;

		if (m_fadeSprite->FadeInColor(2.0f))
		{
			m_stagePath = App::GetApp()->GetScene<Scene>()->ToNextStage();
			ResetCreateStage();
			ResetGroundStage();
			ResetCameraObject();
			CreateStartBGM();
			m_gameClearState->ResetState();
			m_gameProgress = eGameProgress::FadeIn;
		}

		float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::ToTitleStage()
	{
		// フェード用スプライトのエラーチェック
		if (!m_fadeSprite) return;

		// スプライトのフェードイン処理が終了していた場合の処理
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			// タイトルステージへ遷移
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
		}

		float volume = Utility::Lerp(m_bgmVolume, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::PushButtonStart()
	{
		// QTE中はポーズできないようにする
		if (GetSharedGameObject<Player>(L"Player")->GetStatus(ePlayerStatus::IsCraftQTE)) return;
		auto& menu = GetSharedGameObject<PauseMenu>(L"PAUSE"); // ポーズメニューオブジェクトを取得

		if (m_gameProgress == Pause) // ポーズ中なら
		{
			// ポーズ画面を閉じる
			if (!menu->OnClose()) return; // ポーズ画面を閉じれなかったらSEを鳴らさない
			m_soundManager->StartBGM(L"PAUSE_CLOSE_SE", 0, 1.0f, ThisPtr);
			return;
		}
		if (m_gameProgress == Playing) // プレイ中なら
		{
			// ポーズ画面を表示する
			if (!menu->OnOpen()) return; // ポーズ画面を表示できなかったらSEを鳴らさない
			m_soundManager->StartBGM(L"PAUSE_OPEN_SE", 0, 1.0f, ThisPtr);
		}
	}

	// コンティニュー処理
	void GameStage::ToGameOverStage()
	{
		// フェードイン開始の条件を満たしていた場合の処理
		if (m_countTime >= m_defermentTransition) 
		{
			if (m_gameOverState->GetState() != eGameOverState::StandBy)
			{
				m_gameOverState->UpdateState();
				return;
			}

			auto select = m_gameOverState->GetSelectStage();
			m_gameProgress = select != eSelectGameOver::TitleBack ? eGameProgress::ContinueFadeIn : eGameProgress::ToTitleOver;
		}
		else
		{
			m_countTime += DELTA_TIME;
		}
	}

	void GameStage::ToContinueFadeIn()
	{
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			ResetCreateStage();
			m_gameOverState->ResetState();
			m_gameProgress = eGameProgress::ContinueFadeOut;
		}
	}

	void GameStage::ToContinueFadeOut()
	{
		if (m_fadeSprite->FadeOutColor(2.0f))
		{
			m_gameProgress = eGameProgress::Playing;
		}
	}

	// コンティニュー時のリセット処理
	void GameStage::ResetState()
	{
		ResetCreateStage();

		m_gameProgress = eGameProgress::ContinueFadeIn;
	}

	// タイトルに戻る
	void GameStage::TitleBackState()
	{
		// スプライトをフェードアウト
		m_titleBackSprite->FadeOutColor(1.0f);
		m_continueSprite->FadeOutColor(1.0f);
		if (m_gameSprite->FadeOutColor(2.5f))
		{
			// タイトルステージに遷移
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
		}

		float volume = Utility::Lerp(0.0f, m_bgmVolume, m_gameSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
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

			// STARTボタンを押したら
			if (Input::GetPad().wPressedButtons & XINPUT_GAMEPAD_START)
			{
				PushButtonStart();
			}
			// 演出中かの真偽をプレイ中かどうかで立てる
			m_isStaging = !Utility::OR(m_gameProgress, eGameProgress::FadeIn, eGameProgress::Playing);

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