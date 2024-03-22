/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Input;

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

	void TitleStage::CreateResourses()
	{

	}

	void TitleStage::CreateSpriteObject()
	{

	}

	void TitleStage::PushStartButton()
	{
		// �V�[���̎擾
		const auto& scene = App::GetApp()->GetScene<Scene>();

		if (GetPushA())
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void TitleStage::OnCreate()
	{
		try
		{
			CreateViewLight();
		}
		catch (...)
		{
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		PushStartButton();
	}

	void TitleStage::OnDestroy()
	{

	}
}