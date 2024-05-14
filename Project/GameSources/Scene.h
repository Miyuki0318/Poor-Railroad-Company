/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	public:
		const wstring objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの読み込み
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CreateResourses();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シングルモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterTangentMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);

		wstring GetTagName()
		{
			return objTagName;
		}
	};
}