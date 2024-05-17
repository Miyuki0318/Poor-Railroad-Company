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
	class SEManager
	{
	public:

		// SE�Ǘ��\����
		struct SE
		{
			weak_ptr<SoundItem> item; // �T�E���h�A�C�e��
			wstring seKey = L"";	  // �t�@�C���L�[
			const void* objectPtr = nullptr; // �I�u�W�F�N�g�̃|�C���^

			/*!
			@brief �R���X�g���N�^
			@param �T�E���h�A�C�e��
			@param �T�E���h�L�[
			@param �Ăяo�����|�C���^
			*/
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key, const void* objPtr
			) :
				item(ptr),
				seKey(key),
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
				seKey = L"";
				objectPtr = nullptr;
			}
		};

	private:

		vector<SE> m_seList; // SE�Ǘ��z��

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		SEManager() {}

		/*!
		@brief �f�X�g���N�^
		*/
		~SEManager()
		{
			m_seList.clear();
		}

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StartSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);
	};
}