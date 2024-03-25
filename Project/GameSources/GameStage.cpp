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

	// 採掘物の生成
	void GameStage::CreateStageObject()
	{
		// Treeの追加
		shared_ptr<Tree> treeObj;
		shared_ptr<Rock> rockObj;
		for (int i = 0; i < 10; i++) {
			treeObj = AddGameObject<Tree>(Vec3(1.0f * i, 1.5f, 1.0f), 2);
			rockObj = AddGameObject<Rock>(Vec3(1.0f * i, 1.5f, 4.0f), 2);
		}

		// シェアドオブジェクトグループに登録
		const auto& group = GetSharedObjectGroup(L"MiningObject");
		group->IntoGroup(treeObj);
		group->IntoGroup(rockObj);
	}

	void GameStage::OnCreate() 
	{
		try 
		{
			// オブジェクトグループの作成
			CreateSharedObjectGroup(L"MiningObject");

			//ビューとライトの作成
			CreateViewLight();

			// 床ボックスの生成
			CreateGroundBox();

			// プレイヤーの生成
			CreatePlayer();

			// MiningObjectの生成
			CreateStageObject();
		}
		catch (...) 
		{
			throw;
		}
	}
}