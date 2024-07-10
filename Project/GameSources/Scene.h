/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

#define MONEY_LIMIT 99999

namespace basecross{
	// �}�b�v�̓�Փx
	enum class eMapLevel
	{
		FirstMap,
		SecondMap,
		ThirdMap,
		FourthMap,
		FifthMap
	};

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------

	// Player Level
	enum class ePL : char
	{
		Level1,
		Level2,
		Level3,
		Level4,
		Level5,
	};

	class Scene : public SceneBase{
	private:
		eMapLevel m_mapLevel = eMapLevel::FirstMap;
		map<eMapLevel, string> m_stagePathMap;
		map<eMapLevel, eMapLevel> m_nextLevel;

		int m_money;

		// �e�탌�x��
		ePL m_statusLevel;
		ePL m_backPackLevel;
		ePL m_startGearLevel;

		bool m_MovieActive;
	public:
		const wstring m_objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase()
		{
			m_stagePathMap.emplace(eMapLevel::FirstMap, "First");
			m_stagePathMap.emplace(eMapLevel::SecondMap, "Second");
			m_stagePathMap.emplace(eMapLevel::ThirdMap, "Third");
			m_stagePathMap.emplace(eMapLevel::FourthMap, "Fourth");
			m_stagePathMap.emplace(eMapLevel::FifthMap, "Fifth");

			m_nextLevel.emplace(eMapLevel::FirstMap, eMapLevel::SecondMap);
			m_nextLevel.emplace(eMapLevel::SecondMap, eMapLevel::ThirdMap);
			m_nextLevel.emplace(eMapLevel::ThirdMap, eMapLevel::FourthMap);
			m_nextLevel.emplace(eMapLevel::FourthMap, eMapLevel::FifthMap);
			m_nextLevel.emplace(eMapLevel::FifthMap, eMapLevel::FirstMap);

			m_money = 0;

			m_statusLevel = ePL::Level1;
			m_backPackLevel = ePL::Level1;
			m_startGearLevel = ePL::Level1;
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
		@brief	�}�b�v��Փx�����̃X�e�[�W�ɂ��ăt�@�C������Ԃ��֐�
		@return ���̃X�e�[�W�̃p�X
		*/
		string ToNextStage()
		{
			m_mapLevel = m_nextLevel.at(m_mapLevel);
			return m_stagePathMap.at(m_mapLevel);
		}


		/*!
		@brief	�^�O���擾
		*/
		wstring GetTagName()
		{
			return m_objTagName;
		}

		/*!
		@brief �����̎擾
		@return m_money
		*/
		int GetMoney() const
		{
			return m_money;
		}

		/*!
		@brief �����̐ݒ�
		@param[in] �ݒ肷�邨���̒l
		*/
		void SetMoney(int money)
		{
			m_money = money;
		}

		/*!
		@brief �X�e�[�^�X���x���̐ݒ�
		@param[in] �ݒ肷��X�e�[�^�X���x���̒l
		*/
		void SetStatusLevel(ePL level)
		{
			m_statusLevel = level;
		}

		/*!
		@brief �X�e�[�^�X���x���̎擾
		@return �擾����X�e�[�^�X���x���̒l
		*/
		ePL GetStatusLevel() const
		{
			return m_statusLevel;
		}

		/*!
		@brief �o�b�N�p�b�N���x���̐ݒ�
		@param[in] �ݒ肷��o�b�N�p�b�N���x���̒l
		*/
		void SetBackPackLevel(ePL level)
		{
			m_backPackLevel = level;
		}

		/*!
		@brief �o�b�N�p�b�N���x���̎擾
		@return �擾����o�b�N�p�b�N���x���̒l
		*/
		ePL GetBackPackLevel() const
		{
			return m_backPackLevel;
		}

		/*!
		@brief �J�n�������M�A���x���̐ݒ�
		@param[in] �ݒ肷��J�n�������M�A���x���̒l
		*/
		void SetStartGearLevel(ePL level)
		{
			m_startGearLevel = level;
		}

		/*!
		@brief �J�n�������M�A���x���̎擾
		@return �擾����J�n�������M�A���x���̒l
		*/
		ePL GetStartGearLevel() const
		{
			return m_startGearLevel;
		}

		bool IsMovieActive() const 
		{
			return m_MovieActive;
		}
	};
}