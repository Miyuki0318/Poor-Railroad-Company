/*!
@file CraftPosshibleGuide.cpp
@brief �N���t�g�\�ɂȂ������ɏo��G�t�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void CraftPosshibleGuide::OnCreate()
	{
		// �X�e�[�W����v���C���[�ƃG�t�F�N�g���擾
		const auto& stagePtr = GetStage();
		m_playerPtr = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");
		m_effect = stagePtr->GetSharedGameObject<SpriteParticle>(L"CraftEffect");
	}

	// ���t���[���X�V����
	void CraftPosshibleGuide::OnUpdate()
	{
		// �v���C���[�����݂��Ȃ���ΏI��
		const auto& player = m_playerPtr.lock();
		if (!player) return;

		// �N���t�g�}�l�[�W���[�����݂��Ȃ���ΏI��
		const auto& craft = player->GetCraftManager();
		if (!craft) return;

		// �N���t�g�ł��邩�̐^�U��ݒ�
		Bool8_t<eCraftItem> tempFlag;
		tempFlag.Set(eCraftItem::Rail) = craft->CraftOrder(eCraftItem::Rail);
		tempFlag.Set(eCraftItem::WoodBridge) = craft->CraftOrder(eCraftItem::WoodBridge);
		tempFlag.Set(eCraftItem::Crossing) = craft->CraftOrder(eCraftItem::Crossing);
		
		// �O��̏�ԂƓ����Ȃ�I��
		if (m_posshible == tempFlag) return;

		// �N���t�g�ł����ԂɂȂ������̃`�F�b�N�ƌĂяo��
		CheckPosshible(tempFlag, eCraftItem::Rail);
		CheckPosshible(tempFlag, eCraftItem::WoodBridge);
		CheckPosshible(tempFlag, eCraftItem::Crossing);

		// ��Ԃ�ێ�
		m_posshible = tempFlag;
	}

	// �m�F�����ƌĂяo������
	void CraftPosshibleGuide::CheckPosshible(Bool8_t<eCraftItem>& flag, eCraftItem type)
	{
		// ���݂��N���t�g�\�ŁA�O��̓N���t�g�s��������
		if (flag(type) && !m_posshible(type))
		{
			// �G�t�F�N�g���J�n
			StartEffect(type);

			// �T�E���h�A�C�e�������݂��Ȃ��A�܂��̓��\�[�X����Ȃ�
			if (auto& item = m_seItem.lock()) if (item->m_AudioResource.lock()) return;
			m_seItem = StartSE(L"POSSHIBLE_SE", 1.0f);
		}
	}

	// �G�t�F�N�g�J�n����
	void CraftPosshibleGuide::StartEffect(eCraftItem type)
	{
		// �G�~�b�^�[�̍��W���}�b�v����擾
		Vec3 emitterPos = m_uiPosMap.at(type);

		// �G�t�F�N�g�ɃG�~�b�^�[���W�ƊJ�n�����𑗂�
		m_effect.lock()->SetEmitterPosition(emitterPos);
		m_effect.lock()->AddParticle(m_effScale, Vec2(0.0f), 0.0f, m_drawTime);
	}
}