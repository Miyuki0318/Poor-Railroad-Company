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
		SetScale(Vec3(3.0f, 3.0f, 3.0f)); // �傫����3*3�}�X
		SetRotation(Vec3(0.0f));
		Vec3 hulfScale = GetScale() / 2.0f;
		SetPosition(m_spawnPos.x, m_spawnPos.y + hulfScale.y, m_spawnPos.z);

		//CollisionObb�Փ˔����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"STATION");
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

	}

	void Station::OnReset(){
	
	}
}