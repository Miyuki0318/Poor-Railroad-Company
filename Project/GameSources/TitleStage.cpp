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
		AddTextureResource(L"FIRST_TX", modelPath + L"FirstMapTexture.tga");
		AddTextureResource(L"SECOND_TX", modelPath + L"SecondMapTexture.tga");
		AddTextureResource(L"THIRD_TX", modelPath + L"ThirdMapTexture.tga");
		AddTextureResource(L"FOURTH_TX", modelPath + L"FourthMapTexture.tga");
		AddTextureResource(L"FIFTH_TX", modelPath + L"FifthMapTexture.tga");

		// �^�C�g��BGM
		AddAudioResource(L"TITLE_BGM", soundPath + L"Title");

		AddedTextureResources();
		AddedAudioResources();
	}

	// BGM�J�n�֐�
	void TitleStage::StartBGM()
	{
		m_bgmItem = m_soundManager->StartBGM(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.0f, ThisPtr);
	}

	// �^�C�g�����S�̐���
	void TitleStage::CreateOpningScreen()
	{
		if (titleProgress != opening) return;

		auto& opning = AddGameObject<TitleLogo>();
		SetSharedGameObject(L"TitleLogo", opning);
	}

	// �n�ʂ̐���
	void TitleStage::CreateGround()
	{		
		const float scale = 5.5f;
		AddGameObject<GroundManager>(scale);
		AddGameObject<UnBreakRock>();	// �󂹂Ȃ���̐���
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

	// �̏W�I�u�W�F�N�g�̐���
	void TitleStage::CreateGatheringManager()
	{
		const auto& gatheringManager = AddGameObject<GatheringManager>();
		SetSharedGameObject(L"GatheringManager", gatheringManager);
	}

	// �v���C���[�̐���
	void TitleStage::CreatePlayer()
	{
		Vec3 startPos = Vec3(m_cameraAt.x, 2.0f, m_cameraAt.z);
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(startPos);
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

	// �Ŕ̐���
	void TitleStage::CreateSignBoard()
	{
		for (int i = 0; i < m_boardQuantity; i++)
		{
			AddGameObject<SignBoard>(m_textureKeys[i], m_boardPositions[i]);
		}
	}

	// ������UI�̐���
	void TitleStage::CreateUISprite()
	{
		const float scale = 75.0f;
		const Vec3 position = Vec3(650.0f, 460.0f, 0.2f);
		AddGameObject<MoneyCountUI>(scale, position);
	}

	// �{�^�������������̏���
	void TitleStage::PushButtonB()
	{
		if (titleProgress == normal)
		{
			titleProgress = push;
		}

		if (titleProgress == select || titleProgress == zoom)
		{
			titleProgress = normal;
		}
	}

	// �J�����̃Y�[������
	void TitleStage::TitleCameraZoom()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (m_selectObj && !m_zooming)
		{
			m_zooming = true;
			
			Vec3 objPos = m_selectObj->GetComponent<Transform>()->GetPosition();
			titleCamera->SetTargetObject(m_selectObj);

			bool isTrain = bool(dynamic_pointer_cast<TitleTrain>(m_selectObj));
			objPos += isTrain ? m_trainDiffEye : m_objDiffEye;

			titleCamera->ZoomStart(objPos);
		}

		if (Utility::OR(titleProgress, normal, opening))
		{
			titleCamera->ZoomEnd();
		}
	}

	void TitleStage::CameraReset()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		titleCamera->SetEye(m_cameraEye);
		titleCamera->SetAt(m_cameraAt);
		titleCamera->SetTargetObject(nullptr);
		m_zooming = false;
	}

	// �X�v���C�g�̃t�F�[�h����
	void TitleStage::FadeSprite()
	{
		if (!m_fadeSprite) return;
		
		// normal �� push �̏ꍇ�c
		if(Utility::OR(titleProgress,normal,push))
		{
			m_fadeSprite->SetDiffuseColor(COL_ALPHA);
		}

		// zoom �̏ꍇ�c
		if (titleProgress == zoom)
		{
			if (m_selectObj != NULL)
			{
				titleProgress = select;
			}
		}

		// start �̏ꍇ�c
		if (titleProgress == start)
		{
			m_fadeSprite->FadeInColor(2.0f);
		}
	}

	// �I�u�W�F�N�g�ƃv���C���[�̋���
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<TitlePlayer>(L"Player");
		Vec3 playerPos = player->GetPosition();

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
					titleProgress = zoom;
				}

				player->SetState(TitlePlayerPauseState::Instance());
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

			CreateOpningScreen();

			CreatePlayer();

			CreateStageCSV(m_stagePath);

			CreateRailManager();

			CreateGatheringManager();

			CreateGround();

			CreateBuilding();

			CreateTrain();

			CreateSignBoard();
			
			CreateUISprite();
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
			if (m_bgmItem.lock() && Utility::OR(titleProgress, opening, normal))
			{
				auto& item = m_bgmItem.lock()->m_SourceVoice;
				
				float volume = 0.0f;
				item->GetVolume(&volume);

				if (volume <= m_bgmVolume)
				{
					item->SetVolume(volume + DELTA_TIME);
				}
			}

			if (Input::GetPushB())
			{
				PushButtonB();
			}
			
			auto& camera = GetView()->GetTargetCamera();
			auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);
			
			if (titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(titleProgress == normal || titleProgress == move)
			{
				if (m_selectObj && m_selectObj->FindTag(tagName))
				{
					m_selectObj->RemoveTag(tagName);
				}
				m_selectObj = NULL;
				m_zooming = false;
			}

			TitleCameraZoom();

			FadeSprite();

			// �ʏ펞�ȊO�͉��o���̃t���O�𗧂Ă�
			m_isStaging = titleProgress != normal;

			Debug::Log(m_boardQuantity);

		}
		catch (...)
		{
			throw;
		}
	}
}