/*!
@file SEManager.cpp
@brief SE�Ǘ��N���X
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// SE�̍Đ�
	void SEManager::StartSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// �L�[�ƃ|�C���^���󂶂�Ȃ����
		if (seKey != L"" && objPtr != nullptr)
		{
			// �I�[�f�B�I�}�l�[�W���[����SE���Đ������ĕۑ�����
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			m_seList.push_back(SE(audioPtr->Start(seKey, 0, volume), seKey, objPtr));
		}
	}

	// SE�̒�~
	void SEManager::StopSE(const wstring& seKey, const void* objPtr)
	{
		// �L�[�ƃ|�C���^���󂶂�Ȃ����
		if (seKey != L"" && objPtr != nullptr)
		{
			// �z��̃T�C�Y�����[�v
			for (size_t i = 0; i < m_seList.size(); i++)
			{
				// �L�[����v���邩
				if (m_seList.at(i).seKey == seKey)
				{
					// �|�C���^����v���邩
					if (m_seList.at(i).objectPtr == objPtr)
					{
						// �I�[�f�B�I�}�l�[�W���[����SE���~������
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_seList.at(i).item.lock());

						// �\���̂̒��g�����Z�b�g
						m_seList.at(i).Reset();
						break;
					}
				}
			}
		}
	}
}