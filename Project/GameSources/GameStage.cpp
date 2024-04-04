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

		app->RegisterTexture(L"GAMECLEAR_TX", texturePath + L"Win.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"Lose.png");


		// クラフトウィンドウのテクスチャ
		app->RegisterTexture(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTEバーのフレームのテクスチャ
		app->RegisterTexture(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");
	}

	//ビューとライトの生成
	void GameStage::CreateViewLight()
	{
		const Vec3 eye(-1.0f, 20.0f, -15.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	// 床ボックスの生成
	void GameStage::CreateGroundBox()
	{
		// 床ボックスオブジェクトの追加
		AddGameObject<GroundBox>(Vec3(100.0f, 2.0f, 15.0f));
	}

	// プレイヤーの生成
	void GameStage::CreatePlayer()
	{
		// プレイヤーオブジェクトの追加
		const auto& player = AddGameObject<Player>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Player", player);
	}

	// csvでのステージ生成
	void GameStage::CreateStageCSV(string csvPath)
	{
		// 左と下の端
		const float leftX = -7.0f;
		const float upperZ = 7.0f;

		// CSVデータ(int型の二次元配列)
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath));

		// オブジェクトグループ
		const auto& railGroup = GetSharedObjectGroup(L"Rails");
		const auto& miningGroup = GetSharedObjectGroup(L"MiningObject");

		// 二重ループ
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// 仮オブジェクト
				shared_ptr<Rail> rail = nullptr;
				shared_ptr<MiningObject> mining = nullptr;

				// 座標(左限+列番号×スケール,下限+行数-行番号×スケール)
				Vec3 position = Vec3(leftX + j, 1.5f, upperZ + -i);

				// 数値の別名
				const int& num = m_stageMap.at(i).at(j);
				switch (static_cast<eStageID>(num))
				{
				case eStageID::Rail: // レールなら
					//rail = AddGameObject<Rail>(position);
					break;

				case eStageID::GoalRail: // ゴールレールなら
					AddGameObject<GoalRail>(position);
					break;
					
				case eStageID::Rock: // 岩なら
					mining = AddGameObject<Rock>(position, 2); 
					break;

				case eStageID::Tree: // 木なら
					mining = AddGameObject<Tree>(position, 2);
					break;

				default:
					break;
				}

				// オブジェクトグループへの追加
				if (rail)
				{
					railGroup->IntoGroup(rail);
				}
				if (mining)
				{
					miningGroup->IntoGroup(mining);
				}
			}
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

	// 線路の生成
	void GameStage::CreateRail()
	{
		// 線路オブジェクトの追加
		const auto& rail = AddGameObject<Rail>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Rail", rail);
	}
	void GameStage::CreateRails()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);
	}

	void GameStage::CreateGoalRail()
	{
		AddGameObject<GoalRail>(Vec3(9.0f, 1.0f, 0.0f));
	}

	// 列車の生成
	void GameStage::CreateTrain()
	{
		// 列車オブジェクトの追加
		const auto& train = AddGameObject<Train>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Train", train);
	}

	void GameStage::CreateTarminal()
	{
		// 列車オブジェクトの追加
		const auto& tarminal = AddGameObject<Tarminal>(Vec3(10.0f, 1.0f, 0.0f));

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Tarminal", tarminal);
	}

	// スプライトの生成
	void GameStage::CreateSpriteObject()
	{
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));
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
			// オブジェクトグループの作成
			CreateSharedObjectGroup(L"MiningObject"); // 採掘可能オブジェクト
			CreateSharedObjectGroup(L"Rails"); // レールオブジェクト

			// リソースの読み込み
			CreateResourses();

			//ビューとライトの作成
			CreateViewLight();

			// 床ボックスの生成
			CreateGroundBox();

			// プレイヤーの生成
			CreatePlayer();

			//// MiningObjectの生成
			//CreateStageObject();

			//// ゴール地点の生成
			//CreateGoalRail();

			// CSVでステージを生成
			CreateStageCSV();

			// 線路の生成
			CreateRails();

			// 列車の生成
			CreateTrain();

			CreateTarminal();

			// スプライトの生成
			CreateSpriteObject();

			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();
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
		debugStr->SetDrawActive(true);
	}
}