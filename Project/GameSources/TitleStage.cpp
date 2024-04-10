/*!
@file TitleStage.cpp
@brief タイトルステージの実体
@author 新妻葵
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Input;

	// ビューとライトの作成
	void TitleStage::CreateViewLight()
	{
		// ビューを作成
		const auto& ptrView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		const auto& camera = ptrView->GetTargetCamera();
		camera->SetEye(m_cameraEye);
		camera->SetAt(m_cameraAt);
		SetView(ptrView);

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();

		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	// リソースの読込
	void TitleStage::CreateResourses()
	{
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring modelPath = mediaPath + L"Models/" + L"Bilding";

		app->RegisterTexture(L"FADE_TX", texturePath + L"Blue.png");
		app->RegisterTexture(L"MENU_TX", texturePath + L"Menu.png");

		app->RegisterTexture(L"GROUND_TX", texturePath + L"ForestGround.png");
	}

	// スプライトの生成
	void TitleStage::CreateSpriteObject()
	{
		const float width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float height = static_cast<float>(App::GetApp()->GetGameHeight());

		//m_fadeSprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(width, height), Vec3(0.0f));
		//
		//m_fadeSprite->SetDiffuseColor(COL_WHITE);
	}

	// 地面の生成
	void TitleStage::CreateGround()
	{		
		// 床ボックスオブジェクトの追加
		AddGameObject<GroundBox>(m_groundScale);
	}

	// 建物の生成
	void TitleStage::CreateBuilding()
	{
		AddGameObject<Company>(Vec3(35.0f,3.0f,10.0f));

		AddGameObject<Construction>(Vec3(65.0f, 3.0f, 0.0f));
	}

	// 実行時、一度だけ処理される関数
	void TitleStage::OnCreate()
	{
		try
		{
			CreateViewLight();

			CreateResourses();

			CreateSpriteObject();

			CreateGround();

			CreateBuilding();
		}
		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム実行される関数
	void TitleStage::OnUpdate()
	{
	}

	// オブジェクト破棄時に呼び出される処理
	void TitleStage::OnDestroy()
	{

	}
}