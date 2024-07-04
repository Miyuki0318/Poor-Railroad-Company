/*!
@file SEManager.cpp
@brief SE管理クラス
*/

#include "stdafx.h"
#include "OzawaFuncs.h"
namespace basecross
{
	// SEの再生
	shared_ptr<SoundItem> SoundManager::StartSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// キーとポインタが空じゃなければ
		if (seKey != L"" && objPtr != nullptr)
		{
			// オーディオマネージャーからSEを再生させて保存する
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			auto item = audioPtr->Start(seKey, 0, volume);

			// リストに空きがあるなら挿入
			bool empty = false;
			for (auto& se : m_soundList)
			{
				if (!se.item.lock())
				{
					se = SoundData(item, seKey, objPtr);
					empty = true;
				}
			}

			// 空きが無いなら追加
			if (!empty)
			{
				m_soundList.push_back(SoundData(item, seKey, objPtr));
			}

			return item;
		}
		return nullptr;
	}

	// SEの停止
	void SoundManager::StopSE(const wstring& seKey, const void* objPtr)
	{
		// キーとポインタが空じゃなければ
		if (seKey != L"" && objPtr != nullptr)
		{
			// 配列のサイズ分ループ
			for (size_t i = 0; i < m_soundList.size(); i++)
			{
				// キーが一致するか
				if (m_soundList.at(i).soundKey == seKey)
				{
					// ポインタが一致するか
					if (m_soundList.at(i).objectPtr == objPtr)
					{
						// オーディオマネージャーからSEを停止させる
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_soundList.at(i).item.lock());

						// 構造体の中身をリセット
						m_soundList.at(i).Reset();
						break;
					}
				}
			}
		}
	}

	// BGMの再生
	shared_ptr<SoundItem> SoundManager::StartBGM(const wstring& bgmKey, UINT loopNum,  float volume, const void* objPtr)
	{
		// キーとポインタが空じゃなければ
		if (bgmKey != L"" && objPtr != nullptr)
		{
			// オーディオマネージャーからSEを再生させて保存する
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			auto item = audioPtr->Start(bgmKey, loopNum, volume);

			// リストに空きがあるなら挿入
			bool empty = false;
			for (auto& sound : m_soundList)
			{
				if (!sound.item.lock())
				{
					sound = SoundData(item, bgmKey, objPtr);
					empty = true;
				}
			}

			// 空きが無いなら追加
			if (!empty)
			{
				m_soundList.push_back(SoundData(item, bgmKey, objPtr));
			}

			return item;
		}
		return nullptr;
	}

	// SEの停止
	void SoundManager::StopBGM(const wstring& bgmKey, const void* objPtr)
	{
		// キーとポインタが空じゃなければ
		if (bgmKey != L"" && objPtr != nullptr)
		{
			// 配列のサイズ分ループ
			for (size_t i = 0; i < m_soundList.size(); i++)
			{
				// キーが一致するか
				if (m_soundList.at(i).soundKey == bgmKey)
				{
					// ポインタが一致するか
					if (m_soundList.at(i).objectPtr == objPtr)
					{
						// オーディオマネージャーからSEを停止させる
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_soundList.at(i).item.lock());

						// 構造体の中身をリセット
						m_soundList.at(i).Reset();
						break;
					}
				}
			}
		}
	}

	// SEリストのチェック
	void SoundManager::CheckSoundList()
	{
		for (auto& SE : m_soundList)
		{
			// 空なら無視
			auto& item = SE.item.lock();
			if (!item) continue;

			// 音声が停止しているならリセット
			if (!item->m_AudioResource.lock())
			{
				SE.Reset();
			}
		}
	}

	// 音声の破棄
	void SoundManager::ReleasedSounds()
	{
		for (auto& sound : m_soundList)
		{
			// 空なら無視
			auto& item = sound.item.lock();
			if (!item) continue;

			// 音声があるなら
			if (item->m_SourceVoice)
			{
				// オーディオマネージャーからSEを停止させる
				const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
				audioPtr->Stop(item);
			}

			sound.Reset();
		}
	}
}