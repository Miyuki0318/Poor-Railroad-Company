/*!
@file ActionGuide.cpp
@brief �s���̃K�C�h�A�C�R���`��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏��������s
	void ActionGuide::OnCreate()
	{
		// �p�����̐������̏��������s
		Billboard::OnCreate();

		// �e�N�X�`����ύX
		SetTexture(m_texMap.at(m_iconType));

		// ��\����
		SetDrawActive(false);
	}

	// ���t���[���X�V����
	void ActionGuide::OnUpdate()
	{
		// �r���{�[�h�ƍ��W�̍X�V
		UpdateBillboard();
		UpdatePosition();

		// �A�C�R���^�C�v�̍X�V
		m_iconType = eActionIcon::None;
		eActionIcon icon = eActionIcon::None;
		UpdateIconType();

		// �ύX������΃e�N�X�`���̕ύX
		if (icon != m_iconType) SetTexture(m_texMap.at(m_iconType));

		// �\����\���؂�ւ�
		SetDrawActive(m_iconType != eActionIcon::None);
	}

	// ���W�̍X�V
	void ActionGuide::UpdatePosition()
	{
		// �v���C���[�����݂��邩�̃G���[�`�F�b�N
		const auto& player = m_player.lock();
		if (!player) return;

		// �v���C���[�̍��W+���㕪�̍��W
		SetPosition(player->GetPosition() + m_diffPosition);
	}

	// �A�C�R���^�C�v�̍X�V
	void ActionGuide::UpdateIconType()
	{
		// �v���C���[�ƃC���f�B�P�[�^�[�����݂��邩
		const auto& player = m_player.lock();
		const auto& indicator = m_indicator.lock();
		if (!player || !indicator) return;

		// �N���t�g�\�Ȃ�N���t�g�A�C�R��
		if (player->GetCraftPosshible()) m_iconType = eActionIcon::Craft;

		// �X�e�[�WcsvID�ɉ����Đݒ�
		const auto& stageMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();
		const auto& selectPoint = indicator->GetSelectPoint();
		if (Utility::WithInElemRange(selectPoint.x, selectPoint.y, stageMap))
		{
			eStageID id = static_cast<eStageID>(stageMap.at(selectPoint.x).at(selectPoint.y));
			m_iconType = m_iconMap.at(id);

			// ���[���𖢏����Ȃ�None��
			if (m_iconType == eActionIcon::Rail && !player->GetStatus(ePlayerStatus::IsHaveRail))
			{
				m_iconType = eActionIcon::None;
			}
		}
	}
}