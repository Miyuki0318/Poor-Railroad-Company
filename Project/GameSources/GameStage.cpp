/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
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

	void GameStage::CreateResourses()
	{
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		app->RegisterTexture(L"GAMECLEAR_TX", texturePath + L"Win.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"Lose.png");
	}

	void GameStage::CreateSpriteObject()
	{
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));

		m_gameClearLogo->SetDrawActive(false);
		m_gameOverLogo->SetDrawActive(false);
	}

	void GameStage::LogoActive()
	{
		// 電車の状態を取得する
		// 電車が壊れたらGameOverLogoを表示し、
		// 電車が駅にいったらGameClearLogoを表示する
	}

	void GameStage::OnCreate() 
	{
		try 
		{
			CreateResourses();

			//ビューとライトの作成
			CreateViewLight();

			// 床ボックスの生成
			CreateGroundBox();

			// プレイヤーの生成
			CreatePlayer();

			CreateSpriteObject();
		}
		catch (...) 
		{
			throw;
		}
	}
}