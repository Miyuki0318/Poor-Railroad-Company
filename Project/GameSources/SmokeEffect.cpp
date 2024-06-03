/*!
@file SmokeEffect.cpp
@brief ��Ԃ���o�鉌�̃G�t�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Utility;

	// ���̃G�t�F�N�g��������
	void SmokeEffect::AddSmokeEffect(float rotY)
	{
		// �g�����X�t�H�[���R���|�[�l���g
		auto& ptrTrans = GetComponent<Transform>();

		// �p�[�e�B�N���𐶐�
		auto particlePtr = InsertParticle(2);
		particlePtr->SetEmitterPos(ptrTrans->GetWorldPosition());
		particlePtr->SetTextureResource(L"SMOKE_TX");
		particlePtr->SetMaxTime(m_drawTime);

		// �e�I�u�W�F�N�g�̉�]Y��������������߂�
		float x = -cos(rotY) / 2.0f;
		float z = -sin(rotY) / 2.0f;

		// ���������X�v���C�g�z����擾
		auto& spriteVec = particlePtr->GetParticleSpriteVec();
		for (auto& sprite : spriteVec)
		{
			// �ړ��ʂ�ݒ�
			Vec3 velo; 
			velo.y = m_upVelo;
			velo.x = RangeRand(x + m_veloRange, x - m_veloRange);
			velo.z = RangeRand(z + m_veloRange, z - m_veloRange);
			sprite.m_Velocity = velo;

			// �X�P�[����ݒ�
			sprite.m_LocalScale = Vec2(RangeRand(m_scale, 0.1f));
		}
	}
}