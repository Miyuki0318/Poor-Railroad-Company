
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
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"TitleStage");
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
			ResetActiveStage<TitleStage>("Title");
		}
		if (event->m_MsgStr == L"GameStage")
		{
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>(m_stagePathMap.at(m_mapLevel));
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

		// ���h��e�N�X�`��
		app->RegisterTexture(L"WHITE_TX", texturePath + L"White.png");

		// �����e�N�X�`��
		app->RegisterTexture(L"NUMBER_TX", texturePath + L"Number.png");
		
		// ���j���[�e�N�X�`��
		app->RegisterTexture(L"MENU_TX", texturePath + L"Menu.png");

		// �|�[�Y���j���[�e�N�X�`��
		app->RegisterTexture(L"PAUSEMENU_TX", texturePath + L"PauseMenu.png");

		// ���[���̉����f��
		RegisterSingleMesh(L"RAIL", modelPath + L"Rail/", L"Rail", false);
		RegisterSingleMesh(L"TURNRAIL", modelPath + L"Rail/", L"TurnRail", false);
		app->RegisterTexture(L"RAIL_TX", modelPath + L"Rail/RailTx.tga");

		RegisterSingleMesh(L"SKY", modelPath + L"SkyBox/", L"sky", false);
		app->RegisterTexture(L"SKY_TX", modelPath + L"SkyBox/sky.jpg");

		// �\�[�h�N���G�C�^�[���f��
		// �⃂�f���̐���
		app->RegisterTexture(L"ROCK_SM_TX", modelPath + L"Rock/Rock.png");
		RegisterSingleMesh(L"ROCK1", modelPath + L"Rock/", L"Rock1", false);
		RegisterSingleMesh(L"ROCK2", modelPath + L"Rock/", L"Rock2", false);
		RegisterSingleMesh(L"ROCK3", modelPath + L"Rock/", L"Rock3", false);

		// �؂̃��f���̐���
		app->RegisterTexture(L"TREE_SM_TX", modelPath + L"Tree/Tree.png");
		RegisterSingleMesh(L"TREE1", modelPath + L"Tree/", L"Tree1", false);
		RegisterSingleMesh(L"TREE2", modelPath + L"Tree/", L"Tree2", false);

		// ��ԃ��f��
		RegisterSingleMesh(L"TRAIN", modelPath + L"Train/", L"train", false);

		// �w���f��
		RegisterSingleMesh(L"STATION", modelPath + L"Station/", L"station", false);

		// �v���C���[���f���̐���
		wstring playerTag = L"SM_PLAYER_";
		RegisterSingleMesh(playerTag + L"WAIT", modelPath + L"Player/", L"wait", true);
		RegisterSingleMesh(playerTag + L"WALK", modelPath + L"Player/", L"walk", true);
		RegisterSingleMesh(playerTag + L"HARVESTING", modelPath + L"Player/", L"harvesting", true);
		RegisterSingleMesh(playerTag + L"C_START", modelPath + L"Player/", L"craftStart", true);
		RegisterSingleMesh(playerTag + L"C_NOW", modelPath + L"Player/", L"crafting", true);
		RegisterSingleMesh(playerTag + L"C_END", modelPath + L"Player/", L"craftFinish", true);
		RegisterSingleMesh(playerTag + L"SUCCES", modelPath + L"Player/", L"succces", true);
		RegisterSingleMesh(playerTag + L"FAILED", modelPath + L"Player/", L"failed", true);

		// �����̃��f��
		app->RegisterTexture(L"COMPANY_TX", modelPath + L"Bilding/" + L"T_Building.png");
		RegisterSingleMesh(L"COMPANY", modelPath + L"Bilding/", L"buillding", false);

		// �Ŕ̃��f��
		RegisterSingleMesh(L"BOARD", modelPath + L"Bilding/", L"RouteMap", false);

		// �s��⃂�f��
		app->RegisterTexture(L"UN_BREAK_ROCK_TX", modelPath + L"UnBreakRock/Rock.png");
		RegisterSingleMesh(L"UN_BREAK_ROCK1", modelPath + L"UnBreakRock/", L"Rock1", false);
		RegisterSingleMesh(L"UN_BREAK_ROCK2", modelPath + L"UnBreakRock/", L"Rock2", false);
		RegisterSingleMesh(L"UN_BREAK_ROCK3", modelPath + L"UnBreakRock/", L"Rock3", false);

		// �؂̑���̃��f��
		app->RegisterTexture(L"WOOD_BRIDGE_TX", modelPath + L"Bridge/TX_Bridge.tga");
		RegisterSingleMesh(L"WOOD_BRIDGE", modelPath + L"Bridge/", L"Bridge", false);

		// ���؂̃��f��
		RegisterSingleMesh(L"CROSSING", modelPath + L"Crossing/", L"crossing", true);

		// ���Ԃ̃��f��
		RegisterSingleMesh(L"GEAR", modelPath + L"Gear/", L"Gear", true);
	}

	void Scene::RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MeshResource::CreateBoneModelMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MeshResource::CreateStaticModelMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}

	void Scene::RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MultiMeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MultiMeshResource::CreateBoneModelMultiMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MultiMeshResource::CreateStaticModelMultiMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}
}