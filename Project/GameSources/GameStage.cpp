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

	void GameStage::OnCreate() 
	{
		try 
		{
			//ビューとライトの作成
			CreateViewLight();

			// 床ボックスの生成
			CreateGroundBox();
		}
		catch (...) 
		{
			throw;
		}
	}
}
//end basecross
