/*!
@file ResourcesManager.h
@brief �X�e�[�W�ł̃��\�[�X����p�N���X
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{

	/*!
	@brief ���\�[�X�̊Ǘ��N���X
	*/
	class ResourcesManager
	{
		map<wstring, wstring> m_audioResoucesKeys;		// �������\�[�X
		map<wstring, wstring> m_textureResoucesKeys;	// �e�N�X�`�����\�[�X

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		ResourcesManager() {}

		/*!
		@brief �f�X�g���N�^
		*/
		~ResourcesManager() {}

		/*!
		@brief �������\�[�X�����X�g�ɒǉ�����֐�
		@param ���\�[�X�̃L�[
		@param �t�@�C���p�X
		*/
		void AddAudioResource(wstring key, wstring filePath)
		{
			m_audioResoucesKeys.emplace(key, filePath);
		}

		/*!
		@brief �e�N�X�`�����\�[�X�����X�g�ɒǉ�����֐�
		@param ���\�[�X�̃L�[
		@param �t�@�C���p�X
		*/
		void AddTextureResource(wstring key, wstring filePath)
		{
			m_textureResoucesKeys.emplace(key, filePath);
		}

		/*!
		@brief �������\�[�X���������ɒǉ�����֐�
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
		@brief �e�N�X�`�����\�[�X���������ɒǉ�����֐�
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
		@brief �������\�[�X������������������֐�
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
		@brief �e�N�X�`�����\�[�X������������������֐�
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