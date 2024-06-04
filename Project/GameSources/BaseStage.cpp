/*!
@file BaseStage.cpp
@brief �p�����̃X�e�[�W�I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

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

		// �n�ʂ̃e�N�X�`��
		AddTextureResource(L"G_GRASS_TX", texturePath + L"Grass.png");
		AddTextureResource(L"G_WATER_TX", texturePath + L"Water.png");
		AddTextureResource(L"G_SAND_TX", texturePath + L"Sand.png");
		AddTextureResource(L"G_ROCK_TX", texturePath + L"Rock.png");

		// �N���t�g�E�B���h�E�̃e�N�X�`��
		AddTextureResource(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTE�o�[�̃t���[���̃e�N�X�`��
		AddTextureResource(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");

		// �A�C�R���e�N�X�`��
		AddTextureResource(L"I_AXE_TX", texturePath + L"AxeIcon.png");
		AddTextureResource(L"I_PICK_TX", texturePath + L"PickIcon.png");
		AddTextureResource(L"I_RAIL_TX", texturePath + L"RailIcon.png");
		AddTextureResource(L"I_CRAFT_TX", texturePath + L"CraftIcon.png");
		AddTextureResource(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		AddTextureResource(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		AddTextureResource(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");

		// UI�e�N�X�`��
		AddTextureResource(L"UI_WOOD_TX", texturePath + L"Wood.png");
		AddTextureResource(L"UI_STONE_TX", texturePath + L"Stone.png");
		AddTextureResource(L"UI_GEAR_TX", texturePath + L"Gear.png");
		AddTextureResource(L"UI_RAIL_TX", texturePath + L"Rail.png");
		AddTextureResource(L"UI_BRIDGE_TX", texturePath + L"Bridge.png");

		// ��Ԃ̉��e�N�X�`��
		AddTextureResource(L"SMOKE_TX", texturePath + L"Smoke.png");

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
			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();

			// �T�E���h�}�l�[�W���[�̐���
			CreateSoundManager();
		}
		catch (...)
		{
		}
	}

	// �j�����̏���
	void BaseStage::OnDestroy()
	{
		try
		{
			// ���
			m_soundManager.reset();
			ReleasedResourses();
		}
		catch (...)
		{
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