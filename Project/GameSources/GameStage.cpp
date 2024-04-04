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
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<MainCamera>();
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
		// シェアオブジェクトグループ
		const auto& railGroup = GetSharedObjectGroup(L"Rails");

		for (int i = 0; i < 5; i++)
		{
			// 線路オブジェクトの追加
			const auto& rail = AddGameObject<Rail>(Vec3((float)-i, 1.0f, 0.0f));
			// シェアオブジェクトに登録
			SetSharedGameObject(L"Rails" + i, rail);
			railGroup->IntoGroup(rail);
		}
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

			// MiningObjectの生成
			CreateStageObject();

			// 線路の生成
			CreateRails();

			// ゴール地点の生成
			CreateGoalRail();

			// 列車の生成
			CreateTrain();

			CreateTarminal();

			// スプライトの生成
			CreateSpriteObject();

			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();

			// プレイヤーとカメラを取得
			auto train = GetSharedGameObject<Train>(L"Train");
			auto& camera = GetView()->GetTargetCamera();
			auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
			// メインカメラのターゲットに列車をセットする
			mainCamera->SetTargetObject(train);
			mainCamera->SetAt(train->GetDefaultPosition());
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