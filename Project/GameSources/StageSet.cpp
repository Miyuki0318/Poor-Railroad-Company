#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// リソースの読み込み
	void StageSet::CreateResourses()
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
	void StageSet::CreateViewLight()
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
	void StageSet::CreateGroundBox()
	{
		// 床ボックスオブジェクトの追加
		AddGameObject<GroundBox>(Vec3(100.0f, 2.0f, 15.0f));
	}

	// プレイヤーの生成
	void StageSet::CreatePlayer()
	{
		// プレイヤーオブジェクトの追加
		const auto& player = AddGameObject<Player>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Player", player);
	}

	// 採掘物の生成
	void StageSet::CreateStageObject()
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
	void StageSet::CreateRail()
	{
		// 線路オブジェクトの追加
		const auto& rail = AddGameObject<Rail>();


		// シェアオブジェクトに登録
		SetSharedGameObject(L"Rail", rail);
	}
	void StageSet::CreateRails()
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

	// 列車の生成
	void StageSet::CreateTrain()
	{
		// 列車オブジェクトの追加
		const auto& train = AddGameObject<Train>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Train", train);
	}

	void StageSet::CreateTarminal()
	{
		// 列車オブジェクトの追加
		const auto& tarminal = AddGameObject<Tarminal>(Vec3(10.0f, 1.0f, 0.0f));

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Tarminal", tarminal);
	}

	// スプライトの生成
	void StageSet::CreateSpriteObject()
	{
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));
	}

	void StageSet::LogoActive()
	{
		switch (m_gameProgress)
		{
		case eGameMenber::Play:
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameMenber::Clear:
			m_gameClearLogo->SetDrawActive(true);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameMenber::Over:
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(true);
			break;
		}
	}
	////BGMの再生
	//void StageSet::PlayBGM() {
	//	auto XAPtr = App::GetApp()->GetXAudio2Manager();
	//	 m_BGM= XAPtr->Start(L"Stage_BGM", XAUDIO2_LOOP_INFINITE, 0.0f);
	//}

	//void StageSet::OnDestroy()
	//{
	//	//BGMのストップ
	//	auto XAPtr = App::GetApp()->GetXAudio2Manager();
	//	XAPtr->Stop(m_BGM);
	//}

	// 生成時の処理
	void StageSet::OnCreate()
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

			// 列車の生成
			CreateTrain();

			CreateTarminal();

			// スプライトの生成
			CreateSpriteObject();

			//BGMの生成
			//PlayBGM();

			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();
		}


		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム更新処理
	void StageSet::OnUpdate()
	{
		try
		{
			LogoActive();
		}
		catch (...)
		{
			throw;
		}
	}

	// 描画処理
	void StageSet::OnDraw()
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