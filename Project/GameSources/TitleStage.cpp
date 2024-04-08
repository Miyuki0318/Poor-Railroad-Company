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
		// �r���[���쐬
		const auto& ptrView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		const auto& camera = ptrView->GetTargetCamera();
		camera->SetEye(m_cameraEye);
		camera->SetAt(m_cameraAt);
		SetView(ptrView);

		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();

		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	// ���\�[�X�̓Ǎ�
	void TitleStage::CreateResourses()
	{
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		app->RegisterTexture(L"FADE_TX", texturePath + L"Blue.png");
	}

	// �X�v���C�g�̐���
	void TitleStage::CreateSpriteObject()
	{
		const float width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float height = static_cast<float>(App::GetApp()->GetGameHeight());

		m_fadeSprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(width, height), Vec3(0.0f));
		
		m_fadeSprite->SetDiffuseColor(COL_WHITE);
	}

	void TitleStage::CreateGround()
	{		
		AddGameObject<Company>(Vec3(-10.0f, 2.5f,10.0f));
		// ���{�b�N�X�I�u�W�F�N�g�̒ǉ�
		AddGameObject<GroundBox>(m_grountScale);
	}

	// �X�^�[�g�{�^�������������̏���
	void TitleStage::PushStartButton()
	{
		if (GetPushA())
		{
			m_startPush = true;
		}
	}

	// �t�F�[�h�C������
	void TitleStage::FadeInSprite()
	{
		// �X�e�[�W�ύX�̂��߁A�V�[�����擾
		auto& scene = App::GetApp()->GetScene<Scene>();

		if (m_startPush && m_fadeSprite->FadeInColor(m_fadeInTime))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	// �t�F�[�h�A�E�g����
	void TitleStage::FadeOutSprite()
	{
		if (!m_fadeStop && m_fadeSprite->FadeOutColor(m_fadeOutTime))
		{
			m_fadeStop = true;
		}
	}

	// ���s���A��x�������������֐�
	void TitleStage::OnCreate()
	{
		try
		{
			CreateViewLight();

			CreateResourses();

			CreateSpriteObject();

			CreateGround();
		}
		catch (...)
		{
			throw;
		}
	}

	// ���t���[�����s�����֐�
	void TitleStage::OnUpdate()
	{
		FadeOutSprite();

		PushStartButton();

		FadeInSprite();
	}

	// �I�u�W�F�N�g�j�����ɌĂяo����鏈��
	void TitleStage::OnDestroy()
	{

	}
}