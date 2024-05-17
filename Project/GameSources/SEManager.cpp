/*!
@file SEManager.cpp
@brief SE管理クラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// SEの再生
	shared_ptr<SoundItem> SEManager::StartSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// キーとポインタが空じゃなければ
		if (seKey != L"" && objPtr != nullptr)
		{
			// オーディオマネージャーからSEを再生させて保存する
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			auto item = audioPtr->Start(seKey, 0, volume);

			// リストに空きがあるなら挿入
			bool empty = false;
			for (auto& se : m_seList)
			{
				if (!se.item.lock())
				{
					se = SE(item, seKey, objPtr);
					empty = true;
				}
			}

			// 空きが無いなら追加
			if (empty)
			{
				m_seList.push_back(SE(item, seKey, objPtr));
			}

			return item;
		}
	}

	// SEの停止
	void SEManager::StopSE(const wstring& seKey, const void* objPtr)
	{
		// キーとポインタが空じゃなければ
		if (seKey != L"" && objPtr != nullptr)
		{
			// 配列のサイズ分ループ
			for (size_t i = 0; i < m_seList.size(); i++)
			{
				// キーが一致するか
				if (m_seList.at(i).seKey == seKey)
				{
					// ポインタが一致するか
					if (m_seList.at(i).objectPtr == objPtr)
					{
						// オーディオマネージャーからSEを停止させる
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_seList.at(i).item.lock());

						// 構造体の中身をリセット
						m_seList.at(i).Reset();
						break;
					}
				}
			}
		}
	}

	// SEリストのチェック
	void SEManager::CheckSEList()
	{
		for (auto& SE : m_seList)
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
}