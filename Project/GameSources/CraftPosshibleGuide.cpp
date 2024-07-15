/*!
@file CraftPosshibleGuide.cpp
@brief �N���t�g�\�ɂȂ������ɏo��G�t�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "CraftPosshibleGuide.h"

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
		// �X�e�[�W���C�x���g������Ȃ����
		const auto& stagePtr = GetTypeStage<GameStage>();
		if (stagePtr->GetIsStaging())
		{
			UpdatePosshibleIcon(Bool8_t<eCraftItem>());
			return;
		}

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
		
		// �A�C�R���̍X�V�����𑗂�
		UpdatePosshibleIcon(tempFlag);

		// �O��̏�ԂƓ����Ȃ�I��
		if (m_posshible == tempFlag) return;

		// �N���t�g�ł����ԂɂȂ������̃`�F�b�N�ƌĂяo��
		CheckPosshible(tempFlag, eCraftItem::Rail);
		CheckPosshible(tempFlag, eCraftItem::WoodBridge);
		CheckPosshible(tempFlag, eCraftItem::Crossing);

		// ��Ԃ�ێ�
		m_posshible = tempFlag;
	}

	// �A�C�R���̍X�V����
	void CraftPosshibleGuide::UpdatePosshibleIcon(Bool8_t<eCraftItem>& flag)
	{
		// �S���[�v
		for (auto& icon : m_posshibleIcons)
		{
			// �`�悷�邩�̐ݒ�
			bool isDraw = flag(icon.first);
			auto& data = icon.second;
			data.sprite.lock()->SetDrawActive(isDraw);

			// �\�����Ȃ��Ȃ炱��ȍ~�͖���
			if (!isDraw) continue;

			// �o�ߎ��Ԃ����Z
			data.totalTime += DELTA_TIME * XM_PI;
			if (data.totalTime >= XM_2PI) data.totalTime = 0.0f;

			// �G�~�b�V�u�F���T�C���J�[�u�ŕύX
			Col4 emissive = Utility::SinCurve(data.totalTime, COL_BLACK, COL_WHITE);
			data.sprite.lock()->SetEmissiveColor(emissive);
		}
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

	// UI�̃^�C�v�ƍ��W����̓o�^
	void CraftPosshibleGuide::SetUIPosition(eCraftItem type, Vec3 position)
	{
		// UI�̍��W��ێ�
		m_uiPosMap.emplace(type, position);

		// �ԊۃA�C�R���̐���
		const auto& stagePtr = GetStage();
		auto& sprite = stagePtr->AddGameObject<Sprite>(L"RED_CIRCLE_FILL_TX", m_iconScale, position + Vec3(m_iconScale));
		sprite->SetDrawLayer(5);
		sprite->SetDiffuseColor(COL_RED);
		sprite->SetEmissiveColor(COL_GRAY);
		sprite->GetDrawComponent()->SetBlendState(BlendState::AlphaBlend);

		// �p�[�e�B�N���f�[�^�Ɋi�[���ێ�
		ParticleData data = {};
		data.sprite = sprite;
		m_posshibleIcons.emplace(type, data);
	}
}