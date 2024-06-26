/*!
@file GatherEffect.h
@brief ����@�\
@author ���V���M
*/

#pragma once
#include "CraftManager.h"
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief ����G�t�F�N�g
	*/
	class GatherEffect : public Billboard
	{
		const float m_effectTime; // �`�掞��
		float m_totalTime; // �o�ߎ���

		const Vec2 m_drawScale; // �X�P�[��
		const Vec3 m_boundVal; // �o�E���h����
		Vec3 m_startPos; // �J�n���W

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GatherEffect(const shared_ptr<Stage>& stagePtr) :
			Billboard(stagePtr, L"TREASURE_TX", Vec2(0.0f), Vec3(0.0f)),
			m_effectTime(2.0f),
			m_drawScale(0.75f),
			m_boundVal(0.0f, 0.75f, 0.0f)
		{
			m_totalTime = 0.0f;
			m_startPos.zero();
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GatherEffect() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �`��J�n�֐�
		@param �J�n���W
		*/
		void StartDraw(const Vec3& position);
	};

	/*!
	@brief ����@�\
	*/
	class GatherTreasure : public TemplateObject
	{
		// �G�t�F�N�g�z��
		vector<weak_ptr<GatherEffect>> m_effectVec;

		const eItemType m_items[3]; // �A�C�e���^�C�v
		map<eItemType, int> m_addVal; // �A�C�e�����Z��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GatherTreasure(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_items{eItemType::Rail, eItemType::Gear, eItemType::GoldBar}
		{
			m_addVal.emplace(eItemType::Rail, 5);
			m_addVal.emplace(eItemType::Gear, 1);
			m_addVal.emplace(eItemType::GoldBar, 3);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GatherTreasure() {}

		/*!
		@brief ����m�F�����֐�
		@param �m�F���W
		@return ������������f�[�^
		*/
		pair<eItemType, int> TreasureCheck(const Vec3& position);
	};
}