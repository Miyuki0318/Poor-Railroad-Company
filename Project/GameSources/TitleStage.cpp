/*!
@file GameStage.cpp
@brief ゲームステージ実体
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
		camera->SetEye(Vec3(46.0f, 6.6f, -60.0f));
		camera->SetAt(Vec3(49.0f, 6.3f, -61.0f));
		SetView(ptrView);

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();

		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

		// クリアする色を設定
		Col4 color = Col4(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
		App::GetApp()->GetScene<Scene>()->SetClearColor(color);
	}

	// リソースの読込
	void TitleStage::CreateResourses()
	{
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		app->RegisterTexture(L"FADE_TX", texturePath + L"Blue.png");
	}

	// スプライトの生成
	void TitleStage::CreateSpriteObject()
	{
		const float width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float height = static_cast<float>(App::GetApp()->GetGameHeight());

		m_fadeSprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(width, height), Vec3(0.0f));
		
		m_fadeSprite->SetDiffuseColor(COL_WHITE);
	}

	// スタートボタンを押した時の処理
	void TitleStage::PushStartButton()
	{
		if (GetPushA())
		{
			m_startPush = true;
		}
	}

	void TitleStage::FadeInSprite()
	{
		// ステージ変更のため、シーンを取得
		auto& scene = App::GetApp()->GetScene<Scene>();

		if (m_startPush && m_fadeSprite->FadeInColor(3.0f))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void TitleStage::FadeOutSprite()
	{
		if (!m_canPush && m_fadeSprite->FadeOutColor(1.0f))
		{
			m_canPush = true;
		}
	}

	// 実行時、一度だけ処理される関数
	void TitleStage::OnCreate()
	{
		try
		{
			CreateViewLight();

			CreateResourses();

			CreateSpriteObject();
		}
		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム実行される関数
	void TitleStage::OnUpdate()
	{
		FadeOutSprite();

		PushStartButton();

		FadeInSprite();
	}

	// オブジェクト破棄時に呼び出される処理
	void TitleStage::OnDestroy()
	{

	}
}