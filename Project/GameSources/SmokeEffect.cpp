/*!
@file SmokeEffect.cpp
@brief ��Ԃ���o�鉌�̃G�t�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ���̃G�t�F�N�g��������
	void SmokeEffect::AddSmokeEffect()
	{
		auto& ptrTrans = GetComponent<Transform>();

		// �p�[�e�B�N���𐶐�
		auto particlePtr = InsertParticle(3);
		particlePtr->SetEmitterPos(ptrTrans->GetWorldPosition());
		particlePtr->SetTextureResource(L"SMOKE_TX");
		particlePtr->SetMaxTime(m_drawTime);

		// ���������X�v���C�g�z����擾
		auto& spriteVec = particlePtr->GetParticleSpriteVec();
		for (auto& sprite : spriteVec)
		{
			// �ړ��ʂ�ݒ�
			Vec3 velo;
			velo.y = m_upVelo;
			velo.x = Utility::RangeRand(m_veloRange, -m_veloRange);
			velo.z = Utility::RangeRand(m_veloRange, -m_veloRange);
			sprite.m_Velocity = velo;

			// �X�P�[����ݒ�
			sprite.m_LocalScale = Vec2(Utility::RangeRand(m_scale, 0.1f));
		}
	}
}