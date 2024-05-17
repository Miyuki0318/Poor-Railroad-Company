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
		// アプリケーションオブジェクトの取得
		const auto& app = App::GetApp();

		// ディレクトリパスの定義
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";

		// ゲーム結果テクスチャ
		AddTextureResource(L"GAMECLEAR_TX", texturePath + L"Win.png");
		AddTextureResource(L"GAMEOVER_TX", texturePath + L"Lose.png");

		// クラフトウィンドウのテクスチャ
		AddTextureResource(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTEバーのフレームのテクスチャ
		AddTextureResource(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");

		// 地面の仮テクスチャ
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		// アイコンテクスチャ
		AddTextureResource(L"I_AXE_TX", texturePath + L"AxeIcon.png");
		AddTextureResource(L"I_PICK_TX", texturePath + L"PickIcon.png");
		AddTextureResource(L"I_RAIL_TX", texturePath + L"RailIcon.png");
		AddTextureResource(L"I_CRAFT_TX", texturePath + L"CraftIcon.png");
		AddTextureResource(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		AddTextureResource(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		AddTextureResource(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");

		// UIテクスチャ
		AddTextureResource(L"UI_WOOD_TX", texturePath + L"Wood.png");
		AddTextureResource(L"UI_STONE_TX", texturePath + L"Stone.png");
		AddTextureResource(L"UI_RAIL_TX", texturePath + L"Rail.png");

		AddAudioResource(L"ADDRAIL_SE", soundPath + L"Rail");
		AddAudioResource(L"ROCK_SE", soundPath + L"Mining");
		AddAudioResource(L"TREE_SE", soundPath + L"CutTree");

		// 追加したリソースをメモリに追加
		AddedTextureResources();
		AddedAudioResources();
	}

	// リソースの解放
	void GameStage::ReleasedResourses()
	{
		// 音源とテクスチャの解放
		ReleasedAudioResources();
		ReleasedTextureResources();
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

		AddGameObject<GroundManager>();
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

	// 採掘物の生成
	void GameStage::CreateStageObject()
	{
		// シェアドオブジェクトグループを取得
		const auto& group = GetSharedObjectGroup(L"MiningObject");

		for (int i = 0; i < 10; i++) {
			const auto& treeObj = AddGameObject<Tree>(Vec3(1.0f * i, 1.5f, 2.0f), 2);
			const auto& rockObj = AddGameObject<Rock>(Vec3(1.0f * i, 1.5f, 5.0f), 2);

			group->IntoGroup(treeObj);
			group->IntoGroup(rockObj);
		}
	}

	void GameStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);

		AddGameObject<RailGuide>();
	}

	void GameStage::CreateGoalRail()
	{
		AddGameObject<GoalRail>(Vec3(9.0f, 1.0f, 0.0f));
	}

	// 列車の生成
	void GameStage::CreateTrain()
	{
		// 列車オブジェクトの追加
		const auto& train = AddGameObject<GameTrain>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Train", train);
	}

	// スプライトの生成
	void GameStage::CreateSpriteObject()
	{
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));
	}

	// UIの生成
	void GameStage::CreateUIObject()
	{
		// パラメータ
		const float scale = 40.0f;
		const Vec3 startPos = Vec3(-910.0f, 500.0f, 0.0f);
		const Vec3 distance = Vec3(0.0f, -scale * 2.0f, 0.0f);

		// アイテム数UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_RAIL_TX", eItemType::Rail);
	}

	// スプライトの表示
	void GameStage::LogoActive()
	{
		switch (m_gameProgress)
		{
		case eGameProgress::Playing :
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameProgress::GameClear:
			m_gameClearLogo->SetDrawActive(true);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameProgress::GameOver:
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(true);
			break;
		}
	}

	// 生成時の処理
	void GameStage::OnCreate() 
	{
		try 
		{
			BaseStage::OnCreate();

			// オブジェクトグループの作成
			CreateSharedObjectGroup(L"MiningObject"); // 採掘可能オブジェクト

			// リソースの読み込み
			CreateResourses();

			//ビューとライトの作成
			CreateViewLight();

			// プレイヤーの生成
			CreatePlayer();

			// CSVでステージを生成
			CreateStageCSV();

			// 線路の生成
			CreateRailManager();

			// 床ボックスの生成
			CreateGroundBox();

			// 列車の生成
			CreateTrain();

			// スプライトの生成
			CreateSpriteObject();

			// UIの生成
			CreateUIObject();

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
			// スプライトの表示
			LogoActive();
		}
		catch (...)
		{
			throw;
		}
	}

	// 描画処理
	void GameStage::OnDraw()
	{
		// アプリケーションオブジェクトの取得
		const auto& app = App::GetApp();

		// デバッグ文字列を強制的に空にする
		app->GetScene<Scene>()->SetDebugString(L"");
		
		// FPSの描画
		const auto& fps = app->GetStepTimer().GetFramesPerSecond();
		Debug::Log(L"FPS : ", fps);

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(false);
	}
}