/*!
@file TitleStage.cpp
@brief �^�C�g���X�e�[�W�̎���
@author �V�Ȉ�
*/

#include "stdafx.h"
#include "Input.h"
#include "TitleStage.h"
#include "MainCamera.h"
#include "SkyBox.h"
#include "Opening.h"
#include "GroundManager.h"
#include "RailManager.h"
#include "GatheringObject.h"
#include "TitlePlayer.h"
#include "Company.h"
#include "RouteMap.h"
#include "TitleTrain.h"
#include "TitlePlayerState.h"
#include "UnBreakRock.h"
#include "MoneyCountUI.h"
#include "TitleGuide.h"
#include "Arrow.h"

namespace basecross
{
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
		wstring routePath = mediaPath + L"Models/" + L"RouteMap/" ;
		wstring modelPath = mediaPath + L"Models/" + L"Bilding/";
		
		// �^�C�g�����S
		AddTextureResource(L"TITLE_LOGO", texturePath + L"TitleLogo.png");

		// �J���J�n�̃e�N�X�`��
		AddTextureResource(L"START_TX", texturePath + L"PleaseButton.png");

		// ���̃e�N�X�`��
		AddTextureResource(L"RIGHTARROW_TX", texturePath + L"RightArrow.png");
		AddTextureResource(L"LEFTARROW_TX", texturePath + L"LeftArrow.png");
		
		// �{�[�h�̃e�N�X�`��
		AddTextureResource(L"BOARD_TX", routePath + L"MapTexture.tga");

		// �}�b�v�I�����f���̃e�N�X�`��
		AddTextureResource(L"FIRST_TX", routePath + L"FirstMapTexture.tga");
		AddTextureResource(L"SECOND_TX", routePath + L"SecondMapTexture.tga");
		AddTextureResource(L"THIRD_TX", routePath + L"ThirdMapTexture.tga");
		AddTextureResource(L"FOURTH_TX", routePath + L"FourthMapTexture.tga");
		AddTextureResource(L"FIFTH_TX", routePath + L"FifthMapTexture.tga");

		// �K�C�h�̃e�N�X�`��
		AddTextureResource(L"ROUTEMAP_TX", texturePath + L"StageSelectGuide.png");
		AddTextureResource(L"START_TRAIN_TX", texturePath + L"GameStartGuide.png");
		AddTextureResource(L"COMPANIY_TX", texturePath + L"ShopGuide.png");

		// �{�^��UI
		AddTextureResource(L"PUSH_GAMESTART_TX", texturePath + L"BGameStart.png");
		AddTextureResource(L"PUSH_GAMEBACK_TX", texturePath + L"AGameBack.png");
		AddTextureResource(L"CLICK_GAMESTART_TX", texturePath + L"M1GameStart.png");
		AddTextureResource(L"CLICK_GAMEBACK_TX", texturePath + L"M2GameBack.png");

		// �^�C�g��BGM
		AddAudioResource(L"TITLE_BGM", soundPath + L"Title");

		AddedTextureResources();
		AddedAudioResources();
	}

	// BGM�J�n�֐�
	void TitleStage::StartBGM()
	{
		if (m_bgmItem.lock()) return;
		m_bgmItem = m_soundManager->StartBGM(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.0f, ThisPtr);
	}

	// �^�C�g�����S�̐���
	void TitleStage::CreateOpningScreen()
	{
		if (m_titleProgress != opening) return;

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

				// �v���C���[�̊J�n�ʒu�ƃN���A���o���̈ړ����ێ�
				if (num == eStageID::PlayerStart)
				{
					m_startPosition = Vec3(float(j), 2.0f, -float(i));
					id = 0;
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
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(m_startPosition);
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
		auto& scene = App::GetApp()->GetScene<Scene>();

		// ��Ђ̐���
		const auto& company = AddGameObject<Company>();
		SetSharedGameObject(L"Company", company);
		m_objectGroup->IntoGroup(company);

		// �H���}�̐���
		const auto& routeMap = AddGameObject<RouteMap>(scene->GetMapLevel());
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

	// ������UI�̐���
	void TitleStage::CreateUISprite()
	{
		const float scale = 75.0f;
		const Vec3 position = Vec3(650.0f, 460.0f, 0.2f);
		AddGameObject<MoneyCountUI>(scale, position);

		AddGameObject<TitleGuide>();
	}

	// ���̐���
	void TitleStage::CreateArrowSprite()
	{
		const auto& arrow = AddGameObject<SelectArrow>();
		SetSharedGameObject(L"SelectArrow", arrow);
	}

	// �{�^��UI�̐���
	void TitleStage::CreateButtonUI()
	{
		const Vec2 scale = Vec2(200.0f, 100.0f);
		const float posX = 1920.0f / 4.0f;
		const float posY = (1080.0f / 2.0f) - scale.y;

		if (Input::GetPadConected())
		{
			m_pushAButton = AddGameObject<Sprite>(L"PUSH_GAMEBACK_TX", scale, Vec3(posX, -posY, 0.0f));
			m_pushBButton = AddGameObject<Sprite>(L"PUSH_GAMESTART_TX", scale, Vec3(-posX, -posY, 0.0f));
		}
		else
		{
			m_pushAButton = AddGameObject<Sprite>(L"CLICK_GAMEBACK_TX", scale, Vec3(posX, -posY, 0.0f));
			m_pushBButton = AddGameObject<Sprite>(L"CLICK_GAMESTART_TX", scale, Vec3(-posX, -posY, 0.0f));
		}

		m_pushAButton.lock()->SetDrawActive(false);
		m_pushBButton.lock()->SetDrawActive(false);
	}

	// ��莞�ԕ��u�����烀�[�r�[�X�e�[�W�Ɉڍs���鏈��
	void TitleStage::ChengeMovieTime(Vec3 pos)
	{
		Vec3 diff = pos - m_oldPlayerPos;
		float distance = diff.length();

		if (distance != 0.0f)
		{
			m_leaveTime = 0.0f;
			//m_fadeSprite->SetDiffuseColor(COL_ALPHA);
			return;
		}

		const auto& app = App::GetApp();
		m_leaveTime += app->GetElapsedTime();

		if (m_leaveTime >= 5.0f)
		{
			m_fadeSprite->FadeInColor(2.0f);
			if (m_fadeSprite->GetDiffuseColor() == COL_WHITE)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"MovieStage");
			}
		}
	}

	// A�{�^�������������̏���
	void TitleStage::PushButtonA()
	{
		if (m_titleProgress == select)
		{
			m_titleProgress = normal;
		}
	}

	// B�{�^�������������̏���
	void TitleStage::PushButtonB()
	{
		if (m_titleProgress == opening) return;

		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (titleCamera->m_cameraState == MainCamera::Fixed)
		{
			m_titleProgress = push;
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

			// ��ԑI�𒆁H
			bool isTrain = bool(dynamic_pointer_cast<TitleTrain>(m_selectObj));
			// �H���}�I�𒆁H
			bool isRouteMap = bool(dynamic_pointer_cast<RouteMap>(m_selectObj));
			objPos += isTrain ? m_trainDiffEye : isRouteMap ? m_routeMapDiffEye : m_shopDiffEye;

			titleCamera->ZoomStart(objPos, 2.75f);
		}

		if (Utility::OR(m_titleProgress, normal, opening))
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

	// �{�^��UI�̕\������
	void TitleStage::ButtonUIActive()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		bool isMatch = MatchSelectObject(GetSharedGameObject<RouteMap>(L"RouteMap"));

		if (titleCamera->m_cameraState == MainCamera::ZoomedIn && isMatch)
		{
			m_pushAButton.lock()->SetDrawActive(true);
			m_pushBButton.lock()->SetDrawActive(true);
		}
		else
		{
			m_pushAButton.lock()->SetDrawActive(false);
			m_pushBButton.lock()->SetDrawActive(false);
		}
	}

	// �X�v���C�g�̃t�F�[�h����
	void TitleStage::FadeSprite()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		switch (m_titleProgress)
		{
		case basecross::opening:
			if (m_leaveTime >= 5.0f) return;
			m_fadeSprite->FadeOutColor(1.0f);
			break;

		case basecross::normal:
		case basecross::push:
			if (m_leaveTime >= 5.0f) return;

			if (m_fadeSprite->GetDiffuseColor() != Col4(COL_ALPHA))
			{
				m_fadeSprite->FadeOutColor(1.0f);
			}
			break;

		case basecross::zoom:
			if (m_selectObj == GetSharedGameObject<TitleTrain>(L"TitleTrain"))
			{
				if (m_fadeSprite->FadeInColor(1.0f))
				{
					m_titleProgress = select;
				}
			}
			else
			{
				if (titleCamera->m_cameraState == MainCamera::ZoomedIn)
				{
					m_titleProgress = select;
				}
			}
			break;

		case basecross::start:
			m_fadeSprite->FadeInColor(2.0f);
			break;

		default:
			break;
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
			Vec3 diff = targetPos - playerPos;
			float distance = diff.length();

			if (distance < m_searchArea)
			{
				m_selectObj = target;

				if (!m_selectObj->FindTag(tagName))
				{
					m_selectObj->AddTag(tagName);
					m_titleProgress = zoom;
				}

				player->SetState(TitlePlayerPauseState::Instance());
			}

			if (!m_selectObj)
			{
				m_titleProgress = normal;
			}
		}
	}

	// ���s���A��x�������������֐�
	void TitleStage::OnCreate()
	{
		try
		{
			BaseStage::OnCreate();

			m_fadeSprite->SetDiffuseColor(COL_WHITE);

			CreateViewLight();

			CreateOpningScreen();

			CreateStageCSV(m_stagePath);

			CreatePlayer();

			CreateRailManager();

			CreateGatheringManager();

			CreateGround();

			CreateBuilding();

			CreateTrain();
			
			CreateUISprite();

			CreateArrowSprite();

			CreateButtonUI();
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
			const auto& player = GetSharedGameObject<TitlePlayer>(L"Player");

			if (m_bgmItem.lock() && Utility::OR(m_titleProgress, opening, normal))
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

			if (Input::GetPushA())
			{
				PushButtonA();
			}
			
			if (m_titleProgress == push)
			{
				DistanceToPlayer();
			}
			else if(Utility::OR(m_titleProgress,normal,move))
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

			ButtonUIActive();

			if (Utility::OR(m_titleProgress, opening, normal))
			{
				ChengeMovieTime(player->GetPosition());
			}
			
			Debug::Log(L"���� : ", m_leaveTime);

			Debug::Log(L"�F�@ : ", m_fadeSprite->GetDiffuseColor());

			// �ʏ펞�ȊO�͉��o���̃t���O�𗧂Ă�
			m_isStaging = m_titleProgress != normal;

			m_oldPlayerPos = player->GetPosition();
		}
		catch (...)
		{
			throw;
		}
	}
}