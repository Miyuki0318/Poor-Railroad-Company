/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{
	// �}�b�v�̓�Փx
	enum class eMapLevel
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
		eMapLevel m_mapLevel = eMapLevel::easyMap;
		map<eMapLevel, string> m_stagePathMap;
	public:
		const wstring m_objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase()
		{
			m_stagePathMap.emplace(eMapLevel::easyMap, "First");
			m_stagePathMap.emplace(eMapLevel::normalMap, "Second");
			m_stagePathMap.emplace(eMapLevel::hardMap, "Third");
		}
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
		eMapLevel GetMapLevel() const
		{
			return m_mapLevel;
		}

		/*!
		@brief	�}�b�v�̓�Փx��ݒ肷��Z�b�g�֐�
		*/
		void SetMapLevel(eMapLevel level)
		{
			m_mapLevel = level;
		}


		/*!
		@brief	�^�O���擾
		*/
		wstring GetTagName()
		{
			return m_objTagName;
		}
	};
}