/*!
@file MiningObject.cpp
@brief
@author �����
*/

#pragma once
#include "stdafx.h"
#include "Project.h"


namespace basecross {
	void MiningObject::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		TemplateObject::OnCreate();
		SetScale(Vec3(1.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_spawnPos);

		//CollisionObb�Փ˔����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		//�`��R���|�[�l���g�̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);

		// �^�O�̐ݒ�
		AddTag(L"MiningObject");
	}

	void Tree::OnCreate() {
		MiningObject::OnCreate();

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetDiffuse(Col4(0.0f, 0.4f, 0.1f, 1.0f));

		// �^�O�̐ݒ�
		AddTag(L"Tree");
	}

	void Tree::OnUpdate() {
		if (m_setFlg) {
			// ���g�̃R���W�������擾
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroup���擾
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup���m�̏Փ˔�����I�t
			ptrColl->SetExcludeCollisionGroup(group);

			// �t���O��false�ɕύX
			m_setFlg = false;
		}

		// �̌@�񐔂�����ɒB�����ꍇ�A�I�u�W�F�N�g��j��
		if (m_miningCount == m_miningCountLimit) {
			// ����A�j�󂳂��A�j���[�V�������Ȃ��̂ōX�V�~�߂邾��
			SetUpdateActive(false);
		}
	}

	void Tree::OnMining() {
		// �̌@�����񐔂̑���
		m_miningCount++;
	}

	void Tree::OnReset() {
		// �ϐ��̏�����
		m_miningCount = 0;
		// �I�u�W�F�N�g�̍X�V���ĊJ
		SetUpdateActive(true);
	}






	void Rock::OnCreate() {
		MiningObject::OnCreate();

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetDiffuse(Col4(0.3f, 0.3f, 0.3f, 1.0f));

		// �^�O�̐ݒ�
		AddTag(L"Rock");
	}

	void Rock::OnUpdate() {
		if (m_setFlg) {
			// ���g�̃R���W�������擾
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroup���擾
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup���m�̏Փ˔�����I�t
			ptrColl->SetExcludeCollisionGroup(group);

			// �t���O��false�ɕύX
			m_setFlg = false;
		}

		// �̌@�񐔂�����ɒB�����ꍇ�A�I�u�W�F�N�g��j��
		if (m_miningCount == m_miningCountLimit) {
			// ����A�j�󂳂��A�j���[�V�������Ȃ��̂ōX�V�~�߂邾��
			SetUpdateActive(false);
		}
	}

	void Rock::OnMining() {
		// �̌@�����񐔂̑���
		m_miningCount++;
	}

	void Rock::OnReset() {
		// �ϐ��̏�����
		m_miningCount = 0;
		// �I�u�W�F�N�g�̍X�V���ĊJ
		SetUpdateActive(true);
	}


}