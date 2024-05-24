/*!
@file TitleStage.cpp
@brief �^�C�g���X�e�[�W�̎���
@author �V�Ȉ�
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Input;

	// �r���[�ƃ��C�g�̍쐬
	void TitleStage::CreateViewLight()
	{
		auto ptrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto& camera = ObjectFactory::Create<MainCamera>(MainCamera::State::Fixed, m_cameraEye, m_cameraAt);
		ptrView->SetCamera(camera);
		camera->SetAt(m_cameraAt);
		camera->SetEye(m_cameraEye);

		// �X�J�C�{�b�N�X�̍쐬
		auto& skyBox = AddGameObject<SkyBox>(camera);

		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}

	// ���\�[�X�̓Ǎ�
	void TitleStage::CreateResourses()
	{
		BaseStage::CreateResourses();

		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";
		wstring modelPath = mediaPath + L"Models/" + L"Bilding/";
		
		// �^�C�g�����S
		AddTextureResource(L"TITLE_LOGO", texturePath + L"TitleLogo.png");

		// �t�F�[�h�p�̃e�N�X�`��
		AddTextureResource(L"FADE_TX", texturePath + L"Blue.png");

		// �{�[�h�̃e�N�X�`��
		AddTextureResource(L"BOARD_TX", modelPath + L"RouteMapTexture.tga");

		// �}�b�v�I�����f���̃e�N�X�`��
		AddTextureResource(L"EASY_TX", modelPath + L"EasyMapTexture.tga");
		AddTextureResource(L"NORMAL_TX", modelPath + L"NormalMapTexture.tga");
		AddTextureResource(L"HARD_TX", modelPath + L"HardMapTexture.tga");

		// �}�b�v�̃e�N�X�`��
		AddTextureResource(L"EASYMAP_TX", texturePath + L"MapEasy.png");
		AddTextureResource(L"NORMALMAP_TX", texturePath + L"MapNormal.png");
		AddTextureResource(L"HARDMAP_TX", texturePath + L"MapHard.png");

		// �n�ʂ̃e�N�X�`��
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		AddedTextureResources();
		AddedAudioResources();
	}

	// �X�v���C�g�̐���
	void TitleStage::CreateSprite()
	{
		auto& sprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(m_width, m_height));
		sprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", sprite);
	}

	// �I�[�v�j���O��ʂ̐���
	void TitleStage::CreateOpningScreen()
	{
		//auto& opning = AddGameObject<TitleLogo>();
	}

	// �n�ʂ̐���
	void TitleStage::CreateGround()
	{		
		// ���{�b�N�X�I�u�W�F�N�g�̒ǉ�
		auto& ground = AddGameObject<GroundBox>(Vec3(m_cameraAt.x, 0.0f, m_cameraAt.z), m_groundScale);
		ground->SetDrawActive(false);
		SetSharedGameObject(L"TitleGround", ground);

		AddGameObject<GroundManager>();
	}

	// csv�ł̃X�e�[�W����
	void TitleStage::CreateStageCSV(string csvPath)
	{
		// CSV�f�[�^(int�^�̓񎟌��z��)
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// �I�u�W�F�N�g�O���[�v
		const auto& miningGroup = GetSharedObjectGroup(L"MiningObject");

		// ��d���[�v
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// ���I�u�W�F�N�g
				shared_ptr<MiningObject> mining = nullptr;

				// ���W(����+��ԍ��~�X�P�[��,����+�s��-�s�ԍ��~�X�P�[��)
				Vec3 position = LINE2POS(i, j);

				// ���l�̕ʖ�
				eStageID num = STAGE_ID(m_stageMap.at(i).at(j));
				switch (num)
				{
				case eStageID::Stone: // ��Ȃ�
					mining = AddGameObject<Rock>(position);
					break;

				case eStageID::Tree: // �؂Ȃ�
					mining = AddGameObject<Tree>(position);
					break;

				default:
					break;
				}

				// �I�u�W�F�N�g�O���[�v�ւ̒ǉ�
				if (mining)
				{
					miningGroup->IntoGroup(mining);
				}

				tempVec.push_back(position);
			}
			m_positionMap.push_back(tempVec);
		}
	}

	// �v���C���[�̐���
	void TitleStage::CreatePlayer()
	{
		auto& player = AddGameObject<GamePlayer>();
		player->SetPosition(Vec3(m_cameraAt.x, 5.0f, m_cameraAt.z));
		SetSharedGameObject(L"Player", player);
	}

	// ���[���Ǘ��N���X�̐���
	void TitleStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);
	}

	// �����̐���
	void TitleStage::CreateBuilding()
	{
		// ��Ђ̐���
		const auto& company = AddGameObject<Company>();
		SetSharedGameObject(L"Company", company);
		m_objectGroup->IntoGroup(company);

		// �H������̐���
		const auto& construction = AddGameObject<Construction>();
		SetSharedGameObject(L"Construction", construction);
		m_objectGroup->IntoGroup(construction);
		
		// �Ŕ̐���
		const auto& board = AddGameObject<SignBoard>();
		SetSharedGameObject(L"Board", board);
		m_objectGroup->IntoGroup(board);
		
		// �H���}�̐���
		const auto& routeMap = AddGameObject<RouteMap>();
		SetSharedGameObject(L"RouteMap", routeMap);
		m_objectGroup->IntoGroup(routeMap);
	}

	void TitleStage::CreateTrain()
	{
		const auto& train = AddGameObject<TitleTrain>(m_trainPos);
		SetSharedGameObject(L"TitleTrain", train);
		//m_objectGroup->IntoGroup(train);
	}

	// �{�^�������������̏���
	void TitleStage::PushButtonX()
	{
		if (titleProgress == normal)
		{
			titleProgress = push;
		}

		if (titleProgress == select)
		{
			titleProgress = normal;
		}
	}

	// �J�����̃Y�[������
	void TitleStage::TitleCameraZoom()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj&& !m_zooming)
		{
			titleCamera->SetTargetObject(player);
			titleCamera->ZoomStart(titleCamera->GetEye());
			m_zooming = true;
		}

		if (titleProgress == normal)
		{
			titleCamera->SetEye(m_cameraEye);
			titleCamera->SetAt(m_cameraAt);
			m_zooming = false;
		}
	}

	// �X�v���C�g�̃t�F�[�h����
	void TitleStage::FadeSprite()
	{
		auto sprite = GetSharedGameObject<Sprite>(L"FadeSprite", true);
		
		if (titleProgress == select || titleProgress == start)
		{
			sprite->FadeInColor(2.0f);
		}
		else
		{
			sprite->SetDiffuseColor(COL_ALPHA);
		}
	}

	void TitleStage::Progress(shared_ptr<GameObject>& obj)
	{
		if (obj)
		{
			if (obj->FindTag(L"GAMESTART"))
			{
				titleProgress = start;
				return;
			}

			titleProgress = select;
			return;
		}
	}

	// �I�u�W�F�N�g�ƃv���C���[�̋���
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		// �͈�for���ŃO���[�v�ɏ������Ă���I�u�W�F�N�g�����[�v������
		for (auto v : m_objectGroup->GetGroupVector())
		{
			// �I�u�W�F�N�g�����b�N
			auto target = v.lock();
			auto targetPos = target->GetComponent<Transform>()->GetPosition();

			// �I�u�W�F�N�g�ƃv���C���[�̋��������߂�
			m_diff = targetPos - playerPos;
			m_distance = m_diff.length();

			if (m_distance < m_searchArea)
			{
				m_selectObj = target;
				if (!m_selectObj->FindTag(tagName))
				{
					m_selectObj->AddTag(tagName);
				}

				Progress(m_selectObj);
			}

			if (!m_selectObj)
			{
				titleProgress = normal;
			}
		}
	}

	// ���s���A��x�������������֐�
	void TitleStage::OnCreate()
	{
		try
		{
			BaseStage::OnCreate();

			CreateViewLight();

			CreateResourses();

			CreateSprite();

			CreateOpningScreen();

			CreatePlayer();

			CreateStageCSV("Title");

			CreateRailManager();

			CreateGround();

			CreateBuilding();

			CreateTrain();
		}
		catch (...)
		{
			throw;
		}
	}

	// �j������鎞�̏���
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

	// ���t���[�����s�����֐�
	void TitleStage::OnUpdate()
	{
		try 
		{
			if (Input::GetPad().wPressedButtons & XINPUT_GAMEPAD_B)
			{
				PushButtonX();
			}

			Debug::Log(L"�J������At : ", GetView()->GetTargetCamera()->GetAt());

			Debug::Log(L"��Ԃ̈ʒu : ", GetSharedGameObject<TitleTrain>(L"TitleTrain", true)->GetPosition());

			if (titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(titleProgress == normal)
			{
				if (m_selectObj && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
			}

			TitleCameraZoom();

			FadeSprite();
		}
		catch (...)
		{
			throw;
		}
	}
}