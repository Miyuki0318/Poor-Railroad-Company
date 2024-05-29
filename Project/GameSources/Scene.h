/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{
	// �}�b�v�̓�Փx
	enum eMapLevel
	{
		easyMap,
		normalMap,
		hardMap
	};

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	private:
		eMapLevel mapLevel = easyMap;

	public:
		const wstring objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̓ǂݍ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void CreateResourses();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���O�����f�����b�V���̓o�^
		@param[in]  �o�^����L�[
		@param[in]	�f�B���N�g���p�X
		@param[in]	�t�@�C���l�[��
		@param[in]  �{�[�������邩�̐^�U
		*/
		virtual void RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���f�����b�V���̓o�^
		@param[in]  �o�^����L�[
		@param[in]	�f�B���N�g���p�X
		@param[in]	�t�@�C���l�[��
		@param[in]  �{�[�������邩�̐^�U
		*/
		virtual void RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);

		/*!
		@brief	�}�b�v�̓�Փx���擾����Q�b�g�֐�
		*/
		eMapLevel GetMapLevel()
		{
			return mapLevel;
		}

		/*!
		@brief	�}�b�v�̓�Փx��ݒ肷��Z�b�g�֐�
		*/
		void SetMapLevel(eMapLevel level)
		{
			mapLevel = level;
		}


		/*!
		@brief	�^�O���擾
		*/
		wstring GetTagName()
		{
			return objTagName;
		}
	};
}