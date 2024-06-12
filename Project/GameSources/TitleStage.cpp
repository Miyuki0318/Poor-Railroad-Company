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

		// �J���J�n�̃e�N�X�`��
		AddTextureResource(L"START_TX", texturePath + L"PleaseButton.png");

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

		AddAudioResource(L"TITLE_BGM", soundPath + L"Title");

		AddedTextureResources();
		AddedAudioResources();
	}

	void TitleStage::StartBGM()
	{
		m_bgmItem = m_soundManager->StartBGM(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.5f, ThisPtr);
	}

	// �I�[�v�j���O��ʂ̐���
	void TitleStage::CreateOpningScreen()
	{
		auto& opning = AddGameObject<TitleLogo>();
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

		// ��d���[�v
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// ID���擾
				int& id = m_stageMap.at(i).at(j);
				eStageID num = STAGE_ID(id);

				// ���W(����+��ԍ��~�X�P�[��,����+�s��-�s�ԍ��~�X�P�[��)
				tempVec.push_back(ROWCOL2POS(i, j));

				// �΂�ID�Ȃ�
				if (num == eStageID::Stone1)
				{
					int random = Utility::RangeRand(3, 0);
					id = id + random;
				}

				// �؂�ID�Ȃ�
				if (num == eStageID::Tree1)
				{
					int random = Utility::RangeRand(2, 0);
					id = id + random;
				}
			}
			m_positionMap.push_back(tempVec);
		}
	}

	void TitleStage::CreateGatheringManager()
	{
		const auto& gatheringManager = AddGameObject<GatheringManager>();
		SetSharedGameObject(L"GatheringManager", gatheringManager);
	}

	// �v���C���[�̐���
	void TitleStage::CreatePlayer()
	{
		Vec3 startPos = Vec3(m_cameraAt.x, 2.0f, m_cameraAt.z);
		auto& player = AddGameObject<GamePlayer>(startPos, Vec3(0.0f));
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

		// �Ŕ̐���
		const auto& board = AddGameObject<SignBoard>();
		SetSharedGameObject(L"Board", board);
		m_objectGroup->IntoGroup(board);
		
		// �H���}�̐���
		const auto& routeMap = AddGameObject<RouteMap>();
		SetSharedGameObject(L"RouteMap", routeMap);
		m_objectGroup->IntoGroup(routeMap);
	}

	// ��Ԃ̐���
	void TitleStage::CreateTrain()
	{
		const auto& train = AddGameObject<TitleTrain>(m_trainPos);
		SetSharedGameObject(L"TitleTrain", train);
		m_objectGroup->IntoGroup(train);
	}

	// �{�^�������������̏���
	void TitleStage::PushButtonB()
	{
		if (titleProgress == usually)
		{
			titleProgress = push;
		}

		if (titleProgress == select)
		{
			titleProgress = usually;
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

		if (titleProgress == usually)
		{
			titleCamera->SetEye(m_cameraEye);
			titleCamera->SetAt(m_cameraAt);
			m_zooming = false;
		}
	}

	// �X�v���C�g�̃t�F�[�h����
	void TitleStage::FadeSprite()
	{
		if (!m_fadeSprite) return;

		if (titleProgress == zoom)
		{
			if (m_fadeSprite->FadeInColor(2.0f))
			{
				titleProgress = select;
			}
		}
		else if (titleProgress == start)
		{
			m_fadeSprite->FadeInColor(2.0f);
		}
		else if(titleProgress == usually || titleProgress == push)
		{
			m_fadeSprite->SetDiffuseColor(COL_ALPHA);
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

			titleProgress = zoom;
			return;
		}
	}

	// �I�u�W�F�N�g�ƃv���C���[�̋���
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<GamePlayer>(L"Player", true);

		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		// �͈�for���ŃO���[�v�ɏ������Ă���I�u�W�F�N�g�����[�v������
		for (auto& v : m_objectGroup->GetGroupVector())
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
				titleProgress = usually;
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

			CreateOpningScreen();

			CreatePlayer();

			CreateStageCSV(m_stagePath);

			CreateRailManager();

			CreateGatheringManager();

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
			BaseStage::OnDestroy();
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
			if (Input::GetPushB())
			{
				PushButtonB();
			}

			Debug::Log(L"�J������At : ", GetView()->GetTargetCamera()->GetAt());

			Debug::Log(L"��Ԃ̈ʒu : ", GetSharedGameObject<TitleTrain>(L"TitleTrain", true)->GetPosition());

			if (titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(titleProgress == usually)
			{
				if (m_selectObj && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
			}

			TitleCameraZoom();

			FadeSprite();

			m_fadeSprite->SetDrawActive(false);
		}
		catch (...)
		{
			throw;
		}
	}
}