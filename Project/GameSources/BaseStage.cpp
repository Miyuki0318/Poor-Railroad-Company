/*!
@file BaseStage.cpp
@brief 継承元のステージオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// リソースの読み込み
	void BaseStage::CreateResourses()
	{
		// アプリケーションオブジェクトの取得
		const auto& app = App::GetApp();

		// ディレクトリパスの定義
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";

		// フェード用のテクスチャ
		AddTextureResource(L"FADE_TX", texturePath + L"Blue.png");

		// 地面のテクスチャ
		AddTextureResource(L"G_GRASS_TX", texturePath + L"Grass.png");
		AddTextureResource(L"G_WATER_TX", texturePath + L"Water.png");
		AddTextureResource(L"G_SAND_TX", texturePath + L"Sand.png");
		AddTextureResource(L"G_ROCK_TX", texturePath + L"Rock.png");

		// クラフトウィンドウのテクスチャ
		AddTextureResource(L"C_WINDOW_TX", texturePath + L"CraftWindow2.png");

		// QTEバーのフレームのテクスチャ
		AddTextureResource(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");

		// アイコンテクスチャ
		AddTextureResource(L"I_AXE_TX", texturePath + L"AxeIcon.png");
		AddTextureResource(L"I_PICK_TX", texturePath + L"PickIcon.png");
		AddTextureResource(L"I_RAIL_TX", texturePath + L"RailIcon.png");
		AddTextureResource(L"I_CRAFT_TX", texturePath + L"CraftIcon.png");
		AddTextureResource(L"I_BRIDGE_TX", texturePath + L"BridgeIcon.png");
		AddTextureResource(L"I_CROSSING_TX", texturePath + L"CrossingIcon.png");
		AddTextureResource(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		AddTextureResource(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		AddTextureResource(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");

		// UIテクスチャ
		AddTextureResource(L"UI_WOOD_TX", texturePath + L"Wood.png");
		AddTextureResource(L"UI_STONE_TX", texturePath + L"Stone.png");
		AddTextureResource(L"UI_GEAR_TX", texturePath + L"Gear.png");
		AddTextureResource(L"UI_RAIL_TX", texturePath + L"Rail.png");
		AddTextureResource(L"UI_BRIDGE_TX", texturePath + L"Bridge.png");
		AddTextureResource(L"UI_CROSSING_TX", texturePath + L"Crossing.png");

		// 列車の煙テクスチャ
		AddTextureResource(L"SMOKE_TX", texturePath + L"Smoke.png");

		// レール設置時のSE
		AddAudioResource(L"ADDRAIL_SE", soundPath + L"Rail");

		// 採掘、伐採時のSE
		AddAudioResource(L"ROCK_SE", soundPath + L"Mining");
		AddAudioResource(L"TREE_SE", soundPath + L"CutTree");

		// クラフト結果のSE
		AddAudioResource(L"C_SUCCES_SE", soundPath + L"CraftSucces");
		AddAudioResource(L"C_FAILURE_SE", soundPath + L"CraftFailure");

		// 歩いた時のSE
		AddAudioResource(L"WALK_GRASS_SE", soundPath + L"walk_grass");
		AddAudioResource(L"WALK_SAND_SE", soundPath + L"walk_sand");
		AddAudioResource(L"WALK_ROCK_SE", soundPath + L"walk_stone");

		// 汽笛のSE
		AddAudioResource(L"WHISTLE_SE", soundPath + L"Whistle");

		// 追加したリソースをメモリに追加
		AddedTextureResources();
		AddedAudioResources();
	}

	// リソースの解放
	void BaseStage::ReleasedResourses()
	{
		// 音源とテクスチャの解放
		ReleasedAudioResources();
		ReleasedTextureResources();
	}

	// スプライトの生成
	void BaseStage::CreateFadeSprite()
	{
		m_fadeSprite = AddGameObject<Sprite>(L"FADE_TX", WINDOW_SIZE);
		m_fadeSprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", m_fadeSprite);
	}

	// CSVの読み込み用
	void BaseStage::WriteCSVMap(string csvPath)
	{
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// 二重ループ
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				tempVec.push_back(Vec3(float(j), 0.0f, -float(i)));
			}
			m_positionMap.push_back(tempVec);
		}
	}

	// SEマネージャーの生成
	void BaseStage::CreateSoundManager()
	{
		// SEマネージャーがNULLなら
		if (!m_soundManager)
		{
			// SEマネージャーをmake_sharedで生成
			m_soundManager.reset(new SoundManager);
		}
	}

	// SEの再生
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SEマネージャーがNULLなら
		if (!m_soundManager)
		{
			// SEマネージャーの生成を行う
			CreateSoundManager();
		}

		// SEマネージャーからSEの再生を送る
		return m_soundManager->StartSE(seKey, volume, ThisPtr);
	}

	// SEの再生
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SEマネージャーがNULL
		if (!m_soundManager)
		{
			// SEマネージャーの生成を行う
			CreateSoundManager();
		}

		// SEマネージャーからSEの再生を送る
		return m_soundManager->StartSE(seKey, volume, objPtr);
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SEマネージャーがあるなら
		if (m_soundManager)
		{
			// SEマネージャーからSEの停止を送る
			m_soundManager->StopSE(seKey, ThisPtr);
		}
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SEマネージャーがあるなら
		if (m_soundManager)
		{
			// SEマネージャーからSEの停止を送る
			m_soundManager->StopSE(seKey, objPtr);
		}
	}

	// 生成時の処理
	void BaseStage::OnCreate()
	{
		try
		{
			// リソースの読み込み
			CreateResourses();

			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();

			// サウンドマネージャーの生成
			CreateSoundManager();

			// フェード用スプライトの生成
			CreateFadeSprite();
		}
		catch (...)
		{
		}
	}

	// 破棄時の処理
	void BaseStage::OnDestroy()
	{
		try
		{
			// 解放
			m_soundManager->ReleasedSounds();
			m_soundManager.reset();
			ReleasedResourses();
		}
		catch (...)
		{
		}
	}

	// 破棄時の処理
	void BaseStage::OnUpdate()
	{
		try
		{
			// SEリストの確認
			m_soundManager->CheckSoundList();
		}
		catch (...)
		{
		}
	}


	// 描画処理
	void BaseStage::OnDraw()
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