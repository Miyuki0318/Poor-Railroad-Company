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
		auto ptrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto& camera = ObjectFactory::Create<MainCamera>(MainCamera::State::Fixed, m_cameraEye, m_cameraAt);
		ptrView->SetCamera(camera);
		camera->SetAt(m_cameraAt);
		camera->SetEye(m_cameraEye);
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	// リソースの読込
	void TitleStage::CreateResourses()
	{
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring modelPath = mediaPath + L"Models/" + L"Bilding";

		AddTextureResource(L"FADE_TX", texturePath + L"Blue.png");

		// マップのテクスチャ
		AddTextureResource(L"EASYMAP_TX", texturePath + L"MapEasy.png");
		AddTextureResource(L"NORMALMAP_TX", texturePath + L"MapNormal.png");
		AddTextureResource(L"HARDMAP_TX", texturePath + L"MapHard.png");
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		AddedTextureResources();
	}

	// リソースの解放
	void TitleStage::ReleasedResourses()
	{
		// 音源とテクスチャの解放
		ReleasedAudioResources();
		ReleasedTextureResources();
	}

	// スプライトの生成
	void TitleStage::CreateSprite()
	{
		auto& sprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(m_width, m_height));
		sprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", sprite);
	}

	// 地面の生成
	void TitleStage::CreateGround()
	{		
		// 床ボックスオブジェクトの追加
		auto& ground = AddGameObject<GroundBox>(Vec3(m_cameraAt.x, 0.0f, m_cameraAt.z), m_groundScale);
		SetSharedGameObject(L"TitleGround", ground);
	}

	// プレイヤーの生成
	void TitleStage::CreatePlayer()
	{
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(Vec3(m_cameraAt.x, 5.0f, m_cameraAt.z));
		SetSharedGameObject(L"TitlePlayer", player);
	}

	// レール管理クラスの生成
	void TitleStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);
	}

	// 建物の生成
	void TitleStage::CreateBuilding()
	{
		// 会社の生成
		const auto& company = AddGameObject<Company>();
		SetSharedGameObject(L"Company", company);
		m_objectGroup->IntoGroup(company);

		// 工事現場の生成
		const auto& construction = AddGameObject<Construction>();
		SetSharedGameObject(L"Construction", construction);
		m_objectGroup->IntoGroup(construction);
		
		// 看板の生成
		const auto& board = AddGameObject<SignBoard>();
		SetSharedGameObject(L"Board", board);
		m_objectGroup->IntoGroup(board);
		
		// 路線図の生成
		const auto& routeMap = AddGameObject<RouteMap>();
		SetSharedGameObject(L"RouteMap", routeMap);
		m_objectGroup->IntoGroup(routeMap);
	}

	void TitleStage::CreateTrain()
	{
		//const auto& train = AddGameObject<Train>();
	}

	// ボタンを押した時の処理
	void TitleStage::PushButtonX()
	{
		if (!m_buttonPush)
		{
			m_buttonPush = true;
		}
		else if (m_buttonPush)
		{
			m_buttonPush = false;
		}
	}

	// カメラのズーム処理
	void TitleStage::TitleCameraZoom()
	{
		auto& player = GetSharedGameObject<TitlePlayer>(L"TitlePlayer", true);

		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj != NULL && !m_zooming)
		{
			titleCamera->SetTargetObject(player);
			titleCamera->ZoomStart(titleCamera->GetEye());
			m_zooming = true;
		}

		if (!m_buttonPush)
		{
			titleCamera->SetEye(m_cameraEye);
			titleCamera->SetAt(m_cameraAt);
			m_zooming = false;
		}
	}

	// スプライトのフェード処理
	void TitleStage::FadeSprite()
	{
		auto sprite = GetSharedGameObject<Sprite>(L"FadeSprite", true);
		
		if (m_zooming)
		{
			sprite->FadeInColor(2.0f);
		}
		else
		{
			sprite->SetDiffuseColor(COL_ALPHA);
		}
	}

	// オブジェクトとプレイヤーの距離
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<TitlePlayer>(L"TitlePlayer", true);

		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		// 範囲for文でグループに所属しているオブジェクト数ループさせる
		for (auto v : m_objectGroup->GetGroupVector())
		{
			// オブジェクトをロック
			auto target = v.lock();
			auto targetPos = target->GetComponent<Transform>()->GetPosition();

			// オブジェクトとプレイヤーの距離を求める
			m_diff = targetPos - playerPos;
			m_distance = m_diff.length();

			if (m_distance < 2.5f)
			{
				m_selectObj = target;
				if (!m_selectObj->FindTag(tagName))
				{
					m_selectObj->AddTag(tagName);
				}
			}
		}

		if (m_selectObj == NULL)
		{
			m_buttonPush = false;
		}
	}

	// 実行時、一度だけ処理される関数
	void TitleStage::OnCreate()
	{
		try
		{
			CreateViewLight();

			CreateResourses();

			CreateSprite();

			CreateGround();

			CreatePlayer();

			WriteCSVMap("TitleStage");

			CreateRailManager();

			CreateBuilding();

			CreateTrain();
		}
		catch (...)
		{
			throw;
		}
	}

	// 破棄される時の処理
	void TitleStage::OnDestroy()
	{
		try
		{
			ReleasedResourses();
		}
		catch (...)
		{
			throw;
		}
	}


	// 毎フレーム実行される関数
	void TitleStage::OnUpdate()
	{
		try 
		{
			if (Input::GetPushX())
			{
				PushButtonX();
			}

			Debug::Log(L"カメラのAt : ", GetView()->GetTargetCamera()->GetAt());

			if (m_buttonPush)
			{
				DistanceToPlayer();
			}
			else
			{
				if (m_selectObj != NULL && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
			}

			TitleCameraZoom();

			FadeSprite();

			Debug::Log(m_buttonPush);
		}
		catch (...)
		{
			throw;
		}
	}

	// 描画処理
	void TitleStage::OnDraw()
	{
		// アプリケーションオブジェクトの取得
		const auto& app = App::GetApp();

		// デバッグ文字列を強制的に空にする
		app->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}