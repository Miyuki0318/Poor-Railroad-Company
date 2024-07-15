/*!
@file SpriteParticle.cpp
@brief �X�v���C�g�ł̃p�[�e�B�N��
@author ���V���M
*/

#include "stdafx.h"
#include "SpriteParticle.h"

namespace basecross
{
	// �X�V����
	void SpriteParticle::OnUpdate()
	{
		if (m_spriteVec.empty()) return; // ��Ȃ�I��

		for (auto& spriteData : m_spriteVec)
		{
			// �G���[�`�F�b�N
			auto& sprite = spriteData.sprite.lock();
			if (!sprite) continue;
			if (!sprite->GetUpdateActive()) continue;

			// �o�ߎ��Ԃ��`�掞�Ԉȏ�ɂȂ��Ă����A�N�e�B�u��
			if (spriteData.totalTime >= spriteData.drawTime)
			{
				sprite->SetDrawActive(false);
				sprite->SetUpdateActive(false);
				continue;
			}

			// �ړ������𑗂�A�o�ߎ��Ԏ��Ԃ̊����ɉ����ē����ɂ��Ă���
			sprite->MoveSprite();
			sprite->SetDiffuseColor(Utility::Lerp(COL_WHITE, COL_ALPHA, spriteData.totalTime / spriteData.drawTime));
			
			// �f���^�^�C���Ōo�ߎ��Ԃ����Z
			spriteData.totalTime += DELTA_TIME;
		}
	}

	// ���Z�b�g����
	void SpriteParticle::ResetParticle()
	{
		// �S���[�v
		const auto& stagePtr = GetStage();
		for (auto& spriteData : m_spriteVec)
		{
			// �G���[�`�F�b�N
			auto& sprite = spriteData.sprite.lock();
			if (!sprite) continue;

			// ������
			stagePtr->RemoveGameObject<GameObject>(sprite);
		}
	}

	// �p�[�e�B�N���̒ǉ�
	void SpriteParticle::AddParticle(Vec2 scale, Vec2 velo, float rotZ, float drawTime)
	{
		// ��A�N�e�B�u��Ԃ̃X�v���C�g������΍ė��p
		for (auto& spriteData : m_spriteVec)
		{
			// �|�C���^�̎擾
			auto& sprite = spriteData.sprite.lock();

			// ��Ȃ�㏑��
			if (!sprite)
			{
				// �㏑�����ďI��
				spriteData = CreateParticleData(scale, velo, rotZ, drawTime);
				return;
			}

			// ��A�N�e�B�u�Ȃ珉�����ݒ���s���ďI��
			if (!sprite->GetDrawActive() || !sprite->GetUpdateActive())
			{
				spriteData.totalTime = 0.0f;
				spriteData.drawTime = drawTime;

				auto& sprite = spriteData.sprite.lock();
				sprite->SetPosition(m_emitterPosition);
				sprite->SetScale(scale);
				sprite->SetRotation(Vec3(0.0f, 0.0f, rotZ));
				sprite->SetVelocity(velo);
				sprite->SetDiffuseColor(COL_WHITE);
				sprite->SetDrawActive(true);
				sprite->SetUpdateActive(true);

				return;
			}
		}

		// �z��̃T�C�Y��������ȏ�Ȃ疳��
		if (m_spriteVec.size() >= m_maxSprite) return;
		
		// �z��ɃX�v���C�g�f�[�^��ǉ�
		m_spriteVec.push_back(CreateParticleData(scale, velo, rotZ, drawTime));
	}

	// �p�[�e�B�N���f�[�^�̐���
	ParticleData SpriteParticle::CreateParticleData(Vec2 scale, Vec2 velo, float rotZ, float drawTime)
	{
		// �V�K�X�v���C�g�f�[�^�𐶐�
		ParticleData data = {};
		data.drawTime = drawTime;

		// �X�e�[�W�ɃX�v���C�g��ǉ�����
		auto& sprite = GetStage()->AddGameObject<Sprite>(m_textureStr, scale, m_emitterPosition, rotZ);
		sprite->GetDrawComponent()->SetBlendState(BlendState::Additive);
		sprite->SetVelocity(velo);
		sprite->SetDrawLayer(11);
		data.sprite = sprite;

		// ���������f�[�^��Ԃ�
		return data;
	}
}