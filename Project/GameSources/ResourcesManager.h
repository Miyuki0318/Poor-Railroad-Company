/*!
@file ResourcesManager.h
@brief ステージでのリソース解放用クラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{

	/*!
	@brief リソースの管理クラス
	*/
	class ResourcesManager
	{
		map<wstring, wstring> m_audioResoucesKeys;		// 音声リソース
		map<wstring, wstring> m_textureResoucesKeys;	// テクスチャリソース

	public:

		/*!
		@brief コンストラクタ
		*/
		ResourcesManager() {}

		/*!
		@brief デストラクタ
		*/
		~ResourcesManager() {}

		/*!
		@brief 音声リソースをリストに追加する関数
		@param リソースのキー
		@param ファイルパス
		*/
		void AddAudioResource(wstring key, wstring filePath)
		{
			// 重複キーのチェック
			if (m_audioResoucesKeys.find(key) != m_audioResoucesKeys.end())
			{
				throw BaseException(
					L"指定したキーは既に存在します",
					L"オーディオリソースの読み込みを確認してください",
					L"重複キー「" + key + L"」"
				);

				return;
			}

			m_audioResoucesKeys.emplace(key, filePath);
		}

		/*!
		@brief テクスチャリソースをリストに追加する関数
		@param リソースのキー
		@param ファイルパス
		*/
		void AddTextureResource(wstring key, wstring filePath)
		{
			// 重複キーのチェック
			if (m_textureResoucesKeys.find(key) != m_textureResoucesKeys.end())
			{
				throw BaseException(
					L"指定したキーは既に存在します",
					L"テクスチャリソースの読み込みを確認してください",
					L"重複キー「" + key + L"」"
				);

				return;
			}

			m_textureResoucesKeys.emplace(key, filePath);
		}

		/*!
		@brief 音声リソースをメモリに追加する関数
		*/
		void AddedAudioResources()
		{
			const auto& app = App::GetApp();
			for (const auto& res : m_audioResoucesKeys)
			{
				app->RegisterWav(res.first, res.second);
			}
		}

		/*!
		@brief テクスチャリソースをメモリに追加する関数
		*/
		void AddedTextureResources()
		{
			const auto& app = App::GetApp();
			for (const auto& res : m_textureResoucesKeys)
			{
				app->RegisterTexture(res.first, res.second);
			}
		}

		/*!
		@brief 音声リソースをメモリから解放する関数
		*/
		void ReleasedAudioResources()
		{
			const auto& app = App::GetApp();
			for (const auto& res : m_audioResoucesKeys)
			{
				if (app->CheckResourceKey(res.first))
				{
					app->UnRegisterResource<AudioResource>(res.first);
				}
			}
		}

		/*!
		@brief テクスチャリソースをメモリから解放する関数
		*/
		void ReleasedTextureResources()
		{
			const auto& app = App::GetApp();
			for (const auto& res : m_textureResoucesKeys)
			{
				if (app->CheckResourceKey(res.first))
				{
					app->UnRegisterResource<TextureResource>(res.first);
				}
			}
		}
	};
}