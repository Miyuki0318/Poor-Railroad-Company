/*!
@file SEManager.cpp
@brief SE�Ǘ��N���X
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// SE�̍Đ�
	shared_ptr<SoundItem> SoundManager::StartSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// �L�[�ƃ|�C���^���󂶂�Ȃ����
		if (seKey != L"" && objPtr != nullptr)
		{
			// �I�[�f�B�I�}�l�[�W���[����SE���Đ������ĕۑ�����
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			auto item = audioPtr->Start(seKey, 0, volume);

			// ���X�g�ɋ󂫂�����Ȃ�}��
			bool empty = false;
			for (auto& se : m_soundList)
			{
				if (!se.item.lock())
				{
					se = SoundData(item, seKey, objPtr);
					empty = true;
				}
			}

			// �󂫂������Ȃ�ǉ�
			if (empty)
			{
				m_soundList.push_back(SoundData(item, seKey, objPtr));
			}

			return item;
		}
		return nullptr;
	}

	// SE�̒�~
	void SoundManager::StopSE(const wstring& seKey, const void* objPtr)
	{
		// �L�[�ƃ|�C���^���󂶂�Ȃ����
		if (seKey != L"" && objPtr != nullptr)
		{
			// �z��̃T�C�Y�����[�v
			for (size_t i = 0; i < m_soundList.size(); i++)
			{
				// �L�[����v���邩
				if (m_soundList.at(i).soundKey == seKey)
				{
					// �|�C���^����v���邩
					if (m_soundList.at(i).objectPtr == objPtr)
					{
						// �I�[�f�B�I�}�l�[�W���[����SE���~������
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_soundList.at(i).item.lock());

						// �\���̂̒��g�����Z�b�g
						m_soundList.at(i).Reset();
						break;
					}
				}
			}
		}
	}

	// BGM�̍Đ�
	shared_ptr<SoundItem> SoundManager::StartBGM(const wstring& bgmKey, UINT loopNum,  float volume, const void* objPtr)
	{
		// �L�[�ƃ|�C���^���󂶂�Ȃ����
		if (bgmKey != L"" && objPtr != nullptr)
		{
			// �I�[�f�B�I�}�l�[�W���[����SE���Đ������ĕۑ�����
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			auto item = audioPtr->Start(bgmKey, loopNum, volume);

			// ���X�g�ɋ󂫂�����Ȃ�}��
			bool empty = false;
			for (auto& sound : m_soundList)
			{
				if (!sound.item.lock())
				{
					sound = SoundData(item, bgmKey, objPtr);
					empty = true;
				}
			}

			// �󂫂������Ȃ�ǉ�
			if (empty)
			{
				m_soundList.push_back(SoundData(item, bgmKey, objPtr));
			}

			return item;
		}
		return nullptr;
	}

	// SE�̒�~
	void SoundManager::StopBGM(const wstring& bgmKey, const void* objPtr)
	{
		// �L�[�ƃ|�C���^���󂶂�Ȃ����
		if (bgmKey != L"" && objPtr != nullptr)
		{
			// �z��̃T�C�Y�����[�v
			for (size_t i = 0; i < m_soundList.size(); i++)
			{
				// �L�[����v���邩
				if (m_soundList.at(i).soundKey == bgmKey)
				{
					// �|�C���^����v���邩
					if (m_soundList.at(i).objectPtr == objPtr)
					{
						// �I�[�f�B�I�}�l�[�W���[����SE���~������
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_soundList.at(i).item.lock());

						// �\���̂̒��g�����Z�b�g
						m_soundList.at(i).Reset();
						break;
					}
				}
			}
		}
	}

	// SE���X�g�̃`�F�b�N
	void SoundManager::CheckSoundList()
	{
		for (auto& SE : m_soundList)
		{
			// ��Ȃ疳��
			auto& item = SE.item.lock();
			if (!item) continue;

			// ��������~���Ă���Ȃ烊�Z�b�g
			if (!item->m_AudioResource.lock())
			{
				SE.Reset();
			}
		}
	}

	// �����̔j��
	void SoundManager::ReleasedSounds()
	{
		for (auto& sound : m_soundList)
		{
			// ��Ȃ疳��
			auto& item = sound.item.lock();
			if (!item) continue;

			// ����������Ȃ�
			if (item->m_SourceVoice)
			{
				// �I�[�f�B�I�}�l�[�W���[����SE���~������
				const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
				audioPtr->Stop(item);
			}

			sound.Reset();
		}
	}
}