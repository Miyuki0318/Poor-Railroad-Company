/*!
@file SEManager.h
@brief SE�Ǘ��N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief SE�}�l�[�W���[
	*/
	class SoundManager
	{
	public:

		// SE�Ǘ��\����
		struct SoundData
		{
			weak_ptr<SoundItem> item; // �T�E���h�A�C�e��
			wstring soundKey = L"";	  // �t�@�C���L�[
			const void* objectPtr = nullptr; // �I�u�W�F�N�g�̃|�C���^

			/*!
			@brief �R���X�g���N�^
			@param �T�E���h�A�C�e��
			@param �T�E���h�L�[
			@param �Ăяo�����|�C���^
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
			@brief ���Z�b�g�֐�
			*/
			void Reset()
			{
				item.reset();
				soundKey = L"";
				objectPtr = nullptr;
			}
		};

	private:

		vector<SoundData> m_soundList; // SE�Ǘ��z��

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		SoundManager() {}

		/*!
		@brief �f�X�g���N�^
		*/
		~SoundManager()
		{
			m_soundList.clear();
		}

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual shared_ptr<SoundItem> StartSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual shared_ptr<SoundItem> StartBGM(const wstring& bgmKey, UINT loopNum, float volume, const void* objPtr);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StopBGM(const wstring& bgmKey, const void* objPtr);

		/*!
		@brief SE���Đ�����Ă邩�̃`�F�b�N(���Stage��OnUpdate�ŌĂяo��)
		*/
		virtual void CheckSoundList();
	};
}