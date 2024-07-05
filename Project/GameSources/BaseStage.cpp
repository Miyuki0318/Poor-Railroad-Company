/*!
@file BaseStage.cpp
@brief �p�����̃X�e�[�W�I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "BaseStage.h"
#include "MouseCursor.h"
#include "CSVLoader.h"
#include "DebugLog.h"

namespace basecross
{
	// ���\�[�X�̓ǂݍ���
	void BaseStage::CreateResourses()
	{
		// �A�v���P�[�V�����I�u�W�F�N�g�̎擾
		const auto& app = App::GetApp();

		// �f�B���N�g���p�X�̒�`
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";

		// �t�F�[�h�p�̃e�N�X�`��
		AddTextureResource(L"FADE_TX", texturePath + L"Fade_1.png");

		// �n�ʂ̃e�N�X�`��
		AddTextureResource(L"G_GRASS_TX", texturePath + L"Grass.png");
		AddTextureResource(L"G_WATER_TX", texturePath + L"Water.png");
		AddTextureResource(L"G_SAND_TX", texturePath + L"Sand.png");
		AddTextureResource(L"G_ROCK_TX", texturePath + L"Rock.png");

		// �N���t�g�E�B���h�E�̃e�N�X�`��
		AddTextureResource(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTE�o�[�̃t���[���̃e�N�X�`��
		AddTextureResource(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");

		// �N���t�g�A�C�e���A�C�R���e�N�X�`��
		AddTextureResource(L"C_RAIL_TX", texturePath + L"CraftRailIcon.png");
		AddTextureResource(L"C_BRIDGE_TX", texturePath + L"CraftBridgeIcon.png");
		AddTextureResource(L"C_CROSSING_TX", texturePath + L"CraftCrossingIcon.png");

		// �{�^�����̓A�C�R���e�N�X�`��
		AddTextureResource(L"BUTTON_A_TX", texturePath + L"A.png");
		AddTextureResource(L"BUTTON_B_TX", texturePath + L"B.png");
		AddTextureResource(L"BUTTON_Y_TX", texturePath + L"Y.png");

		AddTextureResource(L"CURSOR_TX", texturePath + L"Cursor.png");

		// �A�C�R���e�N�X�`��
		AddTextureResource(L"I_AXE_TX", texturePath + L"AxeIcon.png");
		AddTextureResource(L"I_PICK_TX", texturePath + L"PickIcon.png");
		AddTextureResource(L"I_RAIL_TX", texturePath + L"RailIcon.png");
		AddTextureResource(L"I_CRAFT_TX", texturePath + L"CraftIcon.png");
		AddTextureResource(L"I_BRIDGE_TX", texturePath + L"BridgeIcon.png");
		AddTextureResource(L"I_CROSSING_TX", texturePath + L"CrossingIcon.png");
		AddTextureResource(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		AddTextureResource(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		AddTextureResource(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");

		// UI�e�N�X�`��
		AddTextureResource(L"UI_MENUBACK_TX", texturePath + L"ItemCountBack.png");
		AddTextureResource(L"UI_WOOD_TX", texturePath + L"Wood.png");
		AddTextureResource(L"UI_STONE_TX", texturePath + L"Stone.png");
		AddTextureResource(L"UI_GEAR_TX", texturePath + L"Gear.png");
		AddTextureResource(L"UI_RAIL_TX", texturePath + L"Rail.png");
		AddTextureResource(L"UI_BRIDGE_TX", texturePath + L"Bridge.png");
		AddTextureResource(L"UI_CROSSING_TX", texturePath + L"Crossing.png");
		AddTextureResource(L"UI_GOLDBAR_TX", texturePath + L"GoldBar.png");
		AddTextureResource(L"UI_COIN_TX", texturePath + L"Coin.png");

		// �󔠂̃e�N�X�`��
		AddTextureResource(L"TREASURE_TX", texturePath + L"TreasureBox.png");

		// ��Ԃ̉��e�N�X�`��
		AddTextureResource(L"SMOKE_TX", texturePath + L"Smoke.png");

		// ���}�[�N�e�N�X�`��
		AddTextureResource(L"STAR_TX", texturePath + L"Star.png");

		// ���}�[�N�e�N�X�`��
		AddTextureResource(L"RED_CIRCLE_TX", texturePath + L"Effect.png");

		// ���[���ݒu����SE
		AddAudioResource(L"ADDRAIL_SE", soundPath + L"Rail");

		// �̌@�A���̎���SE
		AddAudioResource(L"ROCK_SE", soundPath + L"Mining");
		AddAudioResource(L"TREE_SE", soundPath + L"CutTree");

		// �N���t�g���ʂ�SE
		AddAudioResource(L"C_SUCCES_SE", soundPath + L"CraftSucces");
		AddAudioResource(L"C_FAILURE_SE", soundPath + L"CraftFailure");

		// ����������SE
		AddAudioResource(L"WALK_GRASS_SE", soundPath + L"walk_grass");
		AddAudioResource(L"WALK_SAND_SE", soundPath + L"walk_sand");
		AddAudioResource(L"WALK_ROCK_SE", soundPath + L"walk_stone");

		// �D�J��SE
		AddAudioResource(L"WHISTLE_SE", soundPath + L"Whistle");
		AddAudioResource(L"SHORT_WHISTLE_SE", soundPath + L"ShortWhistle");

		// �{�^�����͎���SE
		AddAudioResource(L"PUSH_SE", soundPath + L"ButtonPush");

		// �����֌W��SE
		AddAudioResource(L"COIN_SE", soundPath + L"Coin");
		AddAudioResource(L"BUY_SE", soundPath + L"Buy");

		// �ǉ��������\�[�X���������ɒǉ�
		AddedTextureResources();
		AddedAudioResources();
	}

	// ���\�[�X�̉��
	void BaseStage::ReleasedResourses()
	{
		// �����ƃe�N�X�`���̉��
		ReleasedAudioResources();
		ReleasedTextureResources();
	}

	// �X�v���C�g�̐���
	void BaseStage::CreateFadeSprite()
	{
		m_fadeSprite = AddGameObject<Sprite>(L"FADE_TX", WINDOW_SIZE);
		m_fadeSprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", m_fadeSprite);

		AddGameObject<MouseCursor>();
	}

	// CSV�̓ǂݍ��ݗp
	void BaseStage::WriteCSVMap(string csvPath)
	{
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// ��d���[�v
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

	// SE�}�l�[�W���[�̐���
	void BaseStage::CreateSoundManager()
	{
		// SE�}�l�[�W���[��NULL�Ȃ�
		if (!m_soundManager)
		{
			// SE�}�l�[�W���[��make_shared�Ő���
			m_soundManager.reset(new SoundManager);
		}
	}

	// SE�̍Đ�
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SE�}�l�[�W���[��NULL�Ȃ�
		if (!m_soundManager)
		{
			// SE�}�l�[�W���[�̐������s��
			CreateSoundManager();
		}

		// SE�}�l�[�W���[����SE�̍Đ��𑗂�
		return m_soundManager->StartSE(seKey, volume, ThisPtr);
	}

	// SE�̍Đ�
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SE�}�l�[�W���[��NULL
		if (!m_soundManager)
		{
			// SE�}�l�[�W���[�̐������s��
			CreateSoundManager();
		}

		// SE�}�l�[�W���[����SE�̍Đ��𑗂�
		return m_soundManager->StartSE(seKey, volume, objPtr);
	}

	// SE�̒�~
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SE�}�l�[�W���[������Ȃ�
		if (m_soundManager)
		{
			// SE�}�l�[�W���[����SE�̒�~�𑗂�
			m_soundManager->StopSE(seKey, ThisPtr);
		}
	}

	// SE�̒�~
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SE�}�l�[�W���[������Ȃ�
		if (m_soundManager)
		{
			// SE�}�l�[�W���[����SE�̒�~�𑗂�
			m_soundManager->StopSE(seKey, objPtr);
		}
	}

	// �������̏���
	void BaseStage::OnCreate()
	{
		try
		{
			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();

			// �T�E���h�}�l�[�W���[�̐���
			CreateSoundManager();

			// �t�F�[�h�p�X�v���C�g�̐���
			CreateFadeSprite();
		}
		catch (...)
		{
			throw;
		}
	}

	// �j�����̏���
	void BaseStage::OnDestroy()
	{
		try
		{
			// ���
			m_soundManager->ReleasedSounds();
			m_soundManager.reset();
			ReleasedResourses();
		}
		catch (...)
		{
			throw;
		}
	}

	// �j�����̏���
	void BaseStage::OnUpdate()
	{
		try
		{
			// SE���X�g�̊m�F
			m_soundManager->CheckSoundList();
		}
		catch (...)
		{
			throw;
		}
	}


	// �`�揈��
	void BaseStage::OnDraw()
	{
		// �A�v���P�[�V�����I�u�W�F�N�g�̎擾
		const auto& app = App::GetApp();

		// �f�o�b�O������������I�ɋ�ɂ���
		app->GetScene<Scene>()->SetDebugString(L"");

		// FPS�̕`��
		const auto& fps = app->GetStepTimer().GetFramesPerSecond();
		Debug::Log(L"FPS : ", fps);

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}