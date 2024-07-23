/*!
@file BaseStage.cpp
@brief 継承元のステージオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "BaseStage.h"
#include "MouseCursor.h"
#include "CSVLoader.h"
#include "DebugLog.h"

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
		AddTextureResource(L"FADE_TX", texturePath + L"Fade_1.png");

		// 地面のテクスチャ
		AddTextureResource(L"G_GRASS_TX", texturePath + L"Grass.png");
		AddTextureResource(L"G_WATER_TX", texturePath + L"Water.png");
		AddTextureResource(L"G_SAND_TX", texturePath + L"Sand.png");
		AddTextureResource(L"G_ROCK_TX", texturePath + L"Rock.png");

		// ボタン入力アイコンテクスチャ
		AddTextureResource(L"BUTTON_A_TX", texturePath + L"A.png");
		AddTextureResource(L"BUTTON_B_TX", texturePath + L"B.png");
		AddTextureResource(L"BUTTON_Y_TX", texturePath + L"Y.png");

		// マウス入力アイコンテクスチャ
		AddTextureResource(L"BUTTON_M1_TX", texturePath + L"ClickM1.png");
		AddTextureResource(L"BUTTON_M2_TX", texturePath + L"ClickM2.png");
		AddTextureResource(L"BUTTON_M3_TX", texturePath + L"ClickM3.png");

		// マウスカーソルのテクスチャ
		AddTextureResource(L"CURSOR_TX", texturePath + L"Cursor.png");

		// アイコンテクスチャ
		AddTextureResource(L"I_PAD_AXE_TX", texturePath + L"AxeIcon.png");
		AddTextureResource(L"I_PAD_PICK_TX", texturePath + L"PickIcon.png");
		AddTextureResource(L"I_PAD_RAIL_TX", texturePath + L"RailIcon.png");
		AddTextureResource(L"I_PAD_CRAFT_TX", texturePath + L"CraftIcon.png");
		AddTextureResource(L"I_PAD_BRIDGE_TX", texturePath + L"BridgeIcon.png");
		AddTextureResource(L"I_PAD_CROSSING_TX", texturePath + L"CrossingIcon.png");
		AddTextureResource(L"I_MOUSE_AXE_TX", texturePath + L"MouseAxeIcon.png");
		AddTextureResource(L"I_MOUSE_PICK_TX", texturePath + L"MousePickIcon.png");
		AddTextureResource(L"I_MOUSE_RAIL_TX", texturePath + L"MouseRailIcon.png");
		AddTextureResource(L"I_MOUSE_CRAFT_TX", texturePath + L"MouseCraftIcon.png");
		AddTextureResource(L"I_MOUSE_BRIDGE_TX", texturePath + L"MouseBridgeIcon.png");
		AddTextureResource(L"I_MOUSE_CROSSING_TX", texturePath + L"MouseCrossingIcon.png");
		AddTextureResource(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		AddTextureResource(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		AddTextureResource(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");

		// UIテクスチャ
		AddTextureResource(L"UI_MENUBACK_TX", texturePath + L"ItemCountBack.png");
		AddTextureResource(L"UI_WOOD_TX", texturePath + L"Wood.png");
		AddTextureResource(L"UI_STONE_TX", texturePath + L"Stone.png");
		AddTextureResource(L"UI_GEAR_TX", texturePath + L"Gear.png");
		AddTextureResource(L"UI_RAIL_TX", texturePath + L"Rail.png");
		AddTextureResource(L"UI_BRIDGE_TX", texturePath + L"Bridge.png");
		AddTextureResource(L"UI_CROSSING_TX", texturePath + L"Crossing.png");
		AddTextureResource(L"UI_GOLDBAR_TX", texturePath + L"GoldBar.png");
		AddTextureResource(L"UI_COIN_TX", texturePath + L"Coin.png");

		// 宝箱のテクスチャ
		AddTextureResource(L"TREASURE_TX", texturePath + L"TreasureBox.png");

		// 列車の煙テクスチャ
		AddTextureResource(L"SMOKE_TX", texturePath + L"Smoke.png");

		// 星マークテクスチャ
		AddTextureResource(L"STAR_TX", texturePath + L"Star.png");

		// 赤丸テクスチャ
		AddTextureResource(L"RED_CIRCLE_EFEC_TX", texturePath + L"Effect.png");
		AddTextureResource(L"RED_CIRCLE_FILL_TX", texturePath + L"RedCircle.png");

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
		AddAudioResource(L"START_WHISTLE_SE", soundPath + L"StartWhistle");
		AddAudioResource(L"SHORT_WHISTLE_SE", soundPath + L"ShortWhistle");

		// ボタン入力時のSE
		AddAudioResource(L"PUSH_SE", soundPath + L"ButtonPush");
		AddAudioResource(L"SELECT_SE", soundPath + L"Select");

		// お金関係のSE
		AddAudioResource(L"COIN_SE", soundPath + L"Coin");
		AddAudioResource(L"BUY_SE", soundPath + L"Buy");

		// ショップを閉じる時のSE
		AddAudioResource(L"CLOSE_SE", soundPath + L"ShopClose");

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

		AddGameObject<MouseCursor>();
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
			throw;
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
			throw;
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
			throw;
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