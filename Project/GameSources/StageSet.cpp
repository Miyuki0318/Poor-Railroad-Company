/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

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
		// 生成時の処理
		void StageSet::OnCreate()
		{
			try{

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