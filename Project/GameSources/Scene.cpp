
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate()
	{
		try
		{
			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			//�N���A����F��ݒ�
			SetClearColor(Col4(COL_BLUE));

			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"GameStage");
		}
		catch (...)
		{
			throw;
		}
	}

	Scene::~Scene() {}

	void Scene::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"TitleStage")
		{
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"GameStage")
		{
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
		if (event->m_MsgStr == L"StageSet")
		{
			ResetActiveStage<StageSet>();
		}
	} 

	void Scene::CreateResourses()
	{
		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();
		
		// �f�B���N�g���p�X�̐ݒ�
		const wstring mediaPath = app->GetDataDirWString();

		// �e��f�B���N�g���p�X
		const wstring modelPath = mediaPath + L"Models/";
		const wstring texturePath = mediaPath + L"Textures/";
		const wstring soundPath = mediaPath + L"Sounds/";


		// ���h��e�N�X�`��
		app->RegisterTexture(L"WHITE_TX", texturePath + L"White.png");


		//�T�E���h
		app->RegisterWav(L"Rail_SE", soundPath + L"Rail");
		app->RegisterWav(L"Excavation_SE", soundPath + L"Excavation");
		app->RegisterWav(L"Cuttree_SE", soundPath + L"Cuttree");

		//BGM
		app->RegisterWav(L"Stage_BGM", soundPath + L"Stage01");
		


	}
}