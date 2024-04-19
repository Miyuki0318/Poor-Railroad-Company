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
		auto& camera = ObjectFactory::Create<MainCamera>(MainCamera::State::Fixed);
		ptrView->SetCamera(camera);
		camera->SetAt(m_cameraAt);
		camera->SetEye(m_cameraEye);
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

		app->RegisterTexture(L"FADE_TX", texturePath + L"Blue.png");
		app->RegisterTexture(L"MENU_TX", texturePath + L"Menu.png");

		app->RegisterTexture(L"GROUND_TX", texturePath + L"ForestGround.png");
	}

	// �X�v���C�g�̐���
	void TitleStage::CreateSprite()
	{
		auto& sprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(m_width, m_height));
		sprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", sprite);
	}

	// �n�ʂ̐���
	void TitleStage::CreateGround()
	{		
		// ���{�b�N�X�I�u�W�F�N�g�̒ǉ�
		auto& ground = AddGameObject<GroundBox>(m_groundScale);
		SetSharedGameObject(L"TITLEGROUND", ground);
	}

	// �v���C���[�̐���
	void TitleStage::CreatePlayer()
	{
		auto& player = AddGameObject<TitlePlayer>();
		player->SetPosition(Vec3(m_cameraAt.x, 5.0f, m_cameraAt.z));
		SetSharedGameObject(L"TitlePlayer", player);
	}

	// �����̐���
	void TitleStage::CreateBuilding()
	{
		const auto& company = AddGameObject<Company>(Vec3(35.0f,1.0f,10.0f));

		const auto& construction = AddGameObject<Construction>(Vec3(65.0f, 1.0f, 0.0f));
	}

	// �Ŕ̐���
	void TitleStage::CreateSignBoard()
	{
		const auto& board = AddGameObject<SignBoard>();
		SetSharedGameObject(L"BOARD", board);
	}

	// �H���}�̐���
	void TitleStage::CreateRouteMap()
	{
		const auto& routeMap = AddGameObject<RouteMap>();
		SetSharedGameObject(L"ROUTEMAP", routeMap);
	}

	// �����蔻��̐���
	void TitleStage::CreateCollision()
	{
		const auto& companyColl = AddGameObject<CompanyCollision>(Vec3(35.0f, 2.0f, 10.0f));
		SetSharedGameObject(L"COMPANYCOLL", companyColl);

		const auto& constructionColl = AddGameObject<ConstructionCollision>(Vec3(65.0f, 2.0f, 0.0f));
		SetSharedGameObject(L"CONSTRUCTCOLL", constructionColl);
	}

	// �J�����̃Y�[������
	void TitleStage::TitleCameraZoom()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto titleCamera = dynamic_pointer_cast<MainCamera>(camera);

		if (GetSharedGameObject<SignBoard>(L"BOARD", true)->GetPushButton() && !m_zooming)
		{
			auto player = GetSharedGameObject<SignBoard>(L"BOARD");

			titleCamera->SetTargetObject(player);
			titleCamera->ZoomStart(titleCamera->GetEye());
			m_zooming = true;
		}
		else if(!GetSharedGameObject<SignBoard>(L"BOARD", true)->GetPushButton() && m_zooming)
		{
			titleCamera->SetEye(m_cameraEye);
			titleCamera->SetAt(m_cameraAt);

			m_zooming = false;
		}

		Debug::Log(titleCamera->GetEye());
	}

	// �X�v���C�g�̃t�F�[�h����
	void TitleStage::FadeSprite()
	{
		auto sprite = GetSharedGameObject<Sprite>(L"FadeSprite", true);
		
		if (GetSharedGameObject<SignBoard>(L"BOARD", true)->GetPushButton())
		{
			sprite->FadeInColor(2.0f);
		}
		else
		{
			sprite->SetDiffuseColor(COL_ALPHA);
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

			CreateGround();

			CreatePlayer();

			CreateBuilding();

			CreateSignBoard();

			CreateRouteMap();

			CreateCollision();
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
			TitleCameraZoom();

			FadeSprite();
		}
		catch (...)
		{
			throw;
		}
	}

	// �I�u�W�F�N�g�j�����ɌĂяo����鏈��
	void TitleStage::OnDestroy()
	{

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