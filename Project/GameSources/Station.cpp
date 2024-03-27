/*!
@file Station.cpp
@brief Tarminal�̌���}�X�ɔz�u�����w�I�u�W�F
@author �����
*/

#pragma once
#include "stdafx.h"
#include "Project.h"


namespace basecross {
	void Station::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		TemplateObject::OnCreate();
		SetScale(Vec3(1.0f, 2.0f, 1.0f));
		SetRotation(Vec3(0.0f));
		float hulfScale = GetScale().getY() / 2.0f;
		SetPosition(m_spawnPos.x, m_spawnPos.y + hulfScale, m_spawnPos.z);

		//CollisionObb�Փ˔����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));

		//�`��R���|�[�l���g�̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);
		SetAlphaActive(false);

		// �^�O�̐ݒ�
		AddTag(L"Station");

	}

	void Station::OnUpdate() {
		if (!m_setFlg) {
			// ���g�̃R���W�������擾
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroup���擾
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup���m�̏Փ˔�����I�t
			ptrColl->SetExcludeCollisionGroup(group);

			// �t���O��false�ɕύX
			m_setFlg = true;
		}
	}

	void Station::OnReset(){
	
	}
}