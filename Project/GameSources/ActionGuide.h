/*!
@file ActionGuide.h
@brief �s���̃K�C�h�A�C�R���`��
@author ���V���M
*/

#pragma once
#include "Billboard.h"
#include "GamePlayer.h"

namespace basecross
{
	// �A�C�R���^�C�venum
	enum class eActionIcon : size_t
	{
		None,	// ��������
		Pick,	// �c���n�V
		Axe,	// �A�b�N�X
		Craft,	// �N���t�g
		Rail,	// ���[��
	};

	/*!
	@brief �s���K�C�h�A�C�R��
	*/
	class ActionGuide : public Billboard
	{
		eActionIcon m_iconType;	// �A�C�R���̃^�C�v
		map<eStageID, eActionIcon> m_iconMap; // �A�C�R���e�[�u��
		map<eActionIcon, wstring> m_texMap;	// �e�N�X�`���e�[�u��

		const weak_ptr<GamePlayer> m_player; // �v���C���[�̃|�C���^
		const weak_ptr<SelectIndicator> m_indicator; // �C���f�B�P�[�^�[�̃|�C���^
		const Vec3 m_diffPosition; // ����܂ł̍��W�̂���

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�|�C���^
		@param �C���f�B�P�[�^�[�|�C���^
		*/
		ActionGuide(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GamePlayer>& playerPtr,
			const shared_ptr<SelectIndicator>& indicatorPtr
		) :
			Billboard(stagePtr, L"WHITE_TX", Vec2(1.0f), Vec3(0.0f)),
			m_player(playerPtr),
			m_indicator(indicatorPtr),
			m_diffPosition(Vec3(0.0f, 2.0f, 0.5f))
		{
			m_iconType = eActionIcon::None;

			m_iconMap.insert(make_pair(eStageID::Rock, eActionIcon::Pick));
			m_iconMap.insert(make_pair(eStageID::Tree, eActionIcon::Axe));
			m_iconMap.insert(make_pair(eStageID::GuideRail, eActionIcon::Rail));

			m_texMap.insert(make_pair(eActionIcon::Pick, L"ICON_PICK_TX"));
			m_texMap.insert(make_pair(eActionIcon::Axe, L"ICON_AXE_TX"));
			m_texMap.insert(make_pair(eActionIcon::Craft, L"ICON_CRAFT_TX"));
			m_texMap.insert(make_pair(eActionIcon::Rail, L"ICON_RAIL_TX"));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ActionGuide() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief ���W�̍X�V�����֐�
		*/
		void UpdatePosition();

		/*!
		@brief �A�C�R���^�C�v�̍X�V�����֐�
		*/
		void UpdateIconType();
	};
}