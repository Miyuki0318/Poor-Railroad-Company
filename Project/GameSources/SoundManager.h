/*!
@file SoundManager.h
@brief SE管理クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief SEマネージャー
	*/
	class SoundManager
	{
	public:

		// SE管理構造体
		struct SoundData
		{
			weak_ptr<SoundItem> item; // サウンドアイテム
			wstring soundKey = L"";	  // ファイルキー
			const void* objectPtr = nullptr; // オブジェクトのポインタ

			/*!
			@brief コンストラクタ
			@param サウンドアイテム
			@param サウンドキー
			@param 呼び出し元ポインタ
			*/
			SoundData(const weak_ptr<SoundItem>& ptr, const wstring& key, const void* objPtr
			) :
				item(ptr),
				soundKey(key),
				objectPtr(objPtr)
			{
			}

		public:

			/*!
			@brief リセット関数
			*/
			void Reset()
			{
				item.reset();
				soundKey = L"";
				objectPtr = nullptr;
			}
		};

	private:

		vector<SoundData> m_soundList; // SE管理配列

	public:

		/*!
		@brief コンストラクタ
		*/
		SoundManager() {}

		/*!
		@brief デストラクタ
		*/
		~SoundManager()
		{
			m_soundList.clear();
		}

		/*!
		@brief SEの再生関数
		*/
		virtual shared_ptr<SoundItem> StartSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SEの停止関数
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);

		/*!
		@brief BGMの再生関数
		*/
		virtual shared_ptr<SoundItem> StartBGM(const wstring& bgmKey, UINT loopNum, float volume, const void* objPtr);

		/*!
		@brief BGMの停止関数
		*/
		virtual void StopBGM(const wstring& bgmKey, const void* objPtr);

		/*!
		@brief 音声が再生されてるかのチェック(主にStageのOnUpdateで呼び出す)
		*/
		virtual void CheckSoundList();

		/*!
		@brief 音声を破棄し、再生中の場合は停止処理を送り解放する
		*/
		virtual void ReleasedSounds();
	};
}