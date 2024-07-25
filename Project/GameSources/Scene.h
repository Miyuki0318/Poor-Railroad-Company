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
		FifthMap,
		SixthMap,
		SeventhMap,
		EighthMap,
		NinthMap,
		TenthMap
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
		ePL m_limitChoicesLevel;
		ePL m_startGearLevel;

	public:
		const wstring m_objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase()
		{
			m_stagePathMap.emplace(eMapLevel::FirstMap, "Lv1");
			m_stagePathMap.emplace(eMapLevel::SecondMap, "Lv2");
			m_stagePathMap.emplace(eMapLevel::ThirdMap, "Lv3");
			m_stagePathMap.emplace(eMapLevel::FourthMap, "Lv4");
			m_stagePathMap.emplace(eMapLevel::FifthMap, "Lv5");
			m_stagePathMap.emplace(eMapLevel::SixthMap, "Lv6");
			m_stagePathMap.emplace(eMapLevel::SeventhMap, "Lv7");
			m_stagePathMap.emplace(eMapLevel::EighthMap, "Lv8");
			m_stagePathMap.emplace(eMapLevel::NinthMap, "Lv9");
			m_stagePathMap.emplace(eMapLevel::TenthMap, "Lv10");

			m_nextLevel.emplace(eMapLevel::FirstMap, eMapLevel::SecondMap);
			m_nextLevel.emplace(eMapLevel::SecondMap, eMapLevel::ThirdMap);
			m_nextLevel.emplace(eMapLevel::ThirdMap, eMapLevel::FourthMap);
			m_nextLevel.emplace(eMapLevel::FourthMap, eMapLevel::FifthMap);
			m_nextLevel.emplace(eMapLevel::FifthMap, eMapLevel::SixthMap);
			m_nextLevel.emplace(eMapLevel::SixthMap, eMapLevel::SeventhMap);
			m_nextLevel.emplace(eMapLevel::SeventhMap, eMapLevel::EighthMap);
			m_nextLevel.emplace(eMapLevel::EighthMap, eMapLevel::NinthMap);
			m_nextLevel.emplace(eMapLevel::NinthMap, eMapLevel::TenthMap);
			m_nextLevel.emplace(eMapLevel::TenthMap, eMapLevel::FirstMap);

			m_money = 0;

			m_statusLevel = ePL::Level1;
			m_limitChoicesLevel = ePL::Level1;
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
		@brief �̌@/�ړ����x�������x���̐ݒ�
		@param[in] �ݒ肷��̌@/�ړ����x�������x���̒l
		*/
		void SetStatusLevel(ePL level)
		{
			m_statusLevel = level;
		}

		/*!
		@brief �̌@/�ړ����x�������x���̎擾
		@return �擾����̌@/�ړ����x�������x���̒l
		*/
		ePL GetStatusLevel() const
		{
			return m_statusLevel;
		}

		/*!
		@brief ���ޏ���������x���̐ݒ�
		@param[in] �ݒ肷�鎑�ޏ���������x���̒l
		*/
		void SetLimitChoicesLevel(ePL level)
		{
			m_limitChoicesLevel = level;
		}

		/*!
		@brief ���ޏ���������x���̎擾
		@return �擾���鎑�ޏ���������x���̒l
		*/
		ePL GetLimitChoicesLevel() const
		{
			return m_limitChoicesLevel;
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
	};
}