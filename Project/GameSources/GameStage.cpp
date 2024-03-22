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

	// 生成時の処理
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
		}
		catch (...) 
		{
			throw;
		}
	}

	// 描画処理
	void GameStage::OnDraw()
	{
		// デバッグ文字列を強制的に空にする
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}