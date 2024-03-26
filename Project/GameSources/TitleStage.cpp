/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
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
		camera->SetEye(Vec3(46.0f, 6.6f, -60.0f));
		camera->SetAt(Vec3(49.0f, 6.3f, -61.0f));
		SetView(ptrView);

		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();

		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

		// �N���A����F��ݒ�
		Col4 color = Col4(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
		App::GetApp()->GetScene<Scene>()->SetClearColor(color);
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

	// �X�^�[�g�{�^�������������̏���
	void TitleStage::PushStartButton()
	{
		if (GetPushA())
		{
			m_startPush = true;
		}
	}

	void TitleStage::FadeInSprite()
	{
		// �X�e�[�W�ύX�̂��߁A�V�[�����擾
		auto& scene = App::GetApp()->GetScene<Scene>();

		if (m_startPush && m_fadeSprite->FadeInColor(3.0f))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void TitleStage::FadeOutSprite()
	{
		if (!m_canPush && m_fadeSprite->FadeOutColor(1.0f))
		{
			m_canPush = true;
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