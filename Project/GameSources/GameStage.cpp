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
		for (int i = 0; i < 5; i++)
		{
			// 線路オブジェクトの追加
			const auto& rail = AddGameObject<Rail>(Vec3((float)-i, 1.0f, 0.0f));
			// シェアオブジェクトに登録
			SetSharedGameObject(L"Rails" + i, rail);
		}

	}


	// 列車の生成
	void GameStage::CreateTrain()
	{
		// 列車オブジェクトの追加
		const auto& train = AddGameObject<Train>();

		// シェアオブジェクトに登録
		SetSharedGameObject(L"Train", train);
	}

	void GameStage::OnCreate() 
	{
		try 
		{
			//ビューとライトの作成
			CreateViewLight();

			// 床ボックスの生成
			CreateGroundBox();

			// プレイヤーの生成
			CreatePlayer();

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
}