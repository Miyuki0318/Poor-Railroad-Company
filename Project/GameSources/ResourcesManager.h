#pragma once
#include "stdafx.h"

namespace basecross
{
	class ResourcesManager
	{
		map<wstring, wstring> m_audioResoucesKeys;
		map<wstring, wstring> m_textureResoucesKeys;

	public:

		ResourcesManager() {}

		~ResourcesManager() {}

		void AddAudioResource(wstring key, wstring filePath)
		{
			m_audioResoucesKeys.emplace(key, filePath);
		}

		void AddTextureResource(wstring key, wstring filePath)
		{
			m_textureResoucesKeys.emplace(key, filePath);
		}

		void AddedAudioResources()
		{
			const auto& app = App::GetApp();
			for (const auto& res : m_audioResoucesKeys)
			{
				app->RegisterWav(res.first, res.second);
			}
		}

		void AddedTextureResources()
		{
			const auto& app = App::GetApp();
			for (const auto& res : m_textureResoucesKeys)
			{
				app->RegisterTexture(res.first, res.second);
			}
		}

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