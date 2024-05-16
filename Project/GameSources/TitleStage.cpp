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
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring modelPath = mediaPath + L"Models/" + L"Bilding";
		
		// �^�C�g�����S
		AddTextureResource(L"TITLE_LOGO", texturePath + L"TitleLogo.png");

		// �t�F�[�h�p�̃e�N�X�`��
		AddTextureResource(L"FADE_TX", texturePath + L"Blue.png");

		// �{�[�h�̃e�N�X�`��
		AddTextureResource(L"BOARD_TX", modelPath + L"Bilding/RouteMapTexture.tga");

		// �}�b�v�I�����f���̃e�N�X�`��
		AddTextureResource(L"EASY_TX", modelPath + L"Bilding/EasyMapTexture.tga");
		AddTextureResource(L"NORMAL_TX", modelPath + L"Bilding/NormalMapTexture.tga");
		AddTextureResource(L"HARD_TX", modelPath + L"Bilding/HardMapTexture.tga");

		// �}�b�v�̃e�N�X�`��
		AddTextureResource(L"EASYMAP_TX", texturePath + L"MapEasy.png");
		AddTextureResource(L"NORMALMAP_TX", texturePath + L"MapNormal.png");
		AddTextureResource(L"HARDMAP_TX", texturePath + L"MapHard.png");

		// �n�ʂ̃e�N�X�`��
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		AddedTextureResources();
	}

	// ���\�[�X�̉��
	void TitleStage::ReleasedResourses()
	{
		// �����ƃe�N�X�`���̉��
		ReleasedAudioResources();
		ReleasedTextureResources();
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
		auto& opning = AddGameObject<TitleLogo>();
	}

	// �n�ʂ̐���
	void TitleStage::CreateGround()
	{		
		// ���{�b�N�X�I�u�W�F�N�g�̒ǉ�
		auto& ground = AddGameObject<GroundBox>(Vec3(m_cameraAt.x, 0.0f, m_cameraAt.z), m_groundScale);
		SetSharedGameObject(L"TitleGround", ground);
	}

	// �v���C���[�̐���
	void TitleStage::CreatePlayer()
	{
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(Vec3(m_cameraAt.x, 5.0f, m_cameraAt.z));
		SetSharedGameObject(L"TitlePlayer", player);
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

		//const auto& ma = AddGameObject<RailManager>();
		//const auto& train = AddGameObject<Train>();
	}

	void TitleStage::CreateTrain()
	{
		const auto& train = AddGameObject<Train>();
		SetSharedGameObject(L"TitleTrain", train);
	}

	// �{�^�������������̏���
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

	// �J�����̃Y�[������
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

	// �X�v���C�g�̃t�F�[�h����
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

	// �I�u�W�F�N�g�ƃv���C���[�̋���
	void TitleStage::DistanceToPlayer()
	{
		auto& player = GetSharedGameObject<TitlePlayer>(L"TitlePlayer", true);

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

	// ���s���A��x�������������֐�
	void TitleStage::OnCreate()
	{
		try
		{
			CreateViewLight();

			CreateResourses();

			CreateSprite();

			CreateOpningScreen();

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
			if (Input::GetPushX())
			{
				PushButtonX();
			}

			Debug::Log(L"�J������At : ", GetView()->GetTargetCamera()->GetAt());

			Debug::Log(L"��Ԃ̈ʒu : ", GetSharedGameObject<Train>(L"TitleTrain", true)->GetPosition());

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

	// �`�揈��
	void TitleStage::OnDraw()
	{
		// �A�v���P�[�V�����I�u�W�F�N�g�̎擾
		const auto& app = App::GetApp();

		// �f�o�b�O������������I�ɋ�ɂ���
		app->GetScene<Scene>()->SetDebugString(L"");

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}