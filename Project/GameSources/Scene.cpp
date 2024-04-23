
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate()
	{
		try
		{
			// リソースの読み込み
			CreateResourses();

			//クリアする色を設定
			SetClearColor(Col4(COL_BLUE));

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
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
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"GameStage")
		{
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}

	void Scene::CreateResourses()
	{
		// アプリケーションの取得
		const auto& app = App::GetApp();

		// ディレクトリパスの設定
		const wstring mediaPath = app->GetDataDirWString();

		// 各種ディレクトリパス
		const wstring modelPath = mediaPath + L"Models/";
		const wstring texturePath = mediaPath + L"Textures/";

		// 白塗りテクスチャ
		app->RegisterTexture(L"WHITE_TX", texturePath + L"White.png");


		// レールの仮モデル
		RegisterSingleMesh(L"RAIL", modelPath + L"Rail/", L"Rail", false);
		app->RegisterTexture(L"RAIL_TX", modelPath + L"Rail/TX_Rail.tga");

		// ソードクリエイターモデル
		// 壊れる岩モデルの生成
		RegisterMultiMesh(L"ROCK", modelPath + L"Rock/", L"rockBreakModel", true);

		// 木のモデリング
		RegisterMultiMesh(L"TREE", modelPath + L"Tree/", L"treeAnimation", true);

		// プレイヤーモデルの生成
		RegisterMultiMesh(L"PLAYER", modelPath + L"Player/", L"Player", true);

		// 建物の仮モデル
		RegisterSingleMesh(L"COMPANY", modelPath + L"Bilding/", L"TestCompany", false);
		RegisterSingleMesh(L"CONSTRUCTION", modelPath + L"Bilding/", L"TestConstruction", false);

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