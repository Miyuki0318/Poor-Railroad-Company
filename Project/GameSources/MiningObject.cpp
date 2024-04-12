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
		SetAlphaActive(false);

		// �^�O�̐ݒ�
		AddTag(L"MiningObject");
	}

	void MiningObject::OnDelete() {
		Vec3 pos = GetPosition();
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		size_t row, col;
		row = ROW(pos.z);
		col = COL(pos.x);

		if (Utility::WithInElemRange(row, col, stageMap))
		{
			stageMap.at(row).at(col) = 0;
		}
	}


	void Tree::OnCreate() {
		MiningObject::OnCreate();

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"TREE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetOwnShadowActive(true);

		// �^�O�̐ݒ�
		AddTag(L"Tree");
	}

	void Tree::OnUpdate() {
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

		// �̌@�񐔂�����ɒB�����ꍇ�A�I�u�W�F�N�g��j��
		if (m_miningCount == m_miningCountLimit) {
			m_state = eState::Broken;
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();
		}
	}

	void Tree::OnMining() {
		// �̌@�����񐔂̑���
		m_miningCount++;
	}

	void Tree::OnReset() {
		// �ϐ��̏�����
		m_miningCount = 0;

		// �I�u�W�F�N�g�̌����ڂ�������Ԃɂ��鏈��������
		 
		
		// �I�u�W�F�N�g�̍X�V���ĊJ
		SetUpdateActive(true);
	}

	void Tree::AccordingState() {
		switch (m_state)
		{
		case eState::Damage:
			// �������̌����ڂɕύX���鏈��������

			break;
		case eState::Broken:
			// �j�󂳂ꂽ�A�j���[�V�������Đ����鏈��������

			// �j�󎞂̃p�[�e�B�N�����Đ����鏈��������

			break;
		case eState::None:
			// �X�V���~���A�s���ɂ���
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();

			break;
		default:
			break;
		}
	}


	void Rock::OnCreate() {
		MiningObject::OnCreate();

		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"ROCK");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetOwnShadowActive(true);

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
			SetDrawActive(false);
			MiningObject::OnDelete();
		}
	}

	void Rock::OnMining() {
		// �̌@�����񐔂̑���
		m_miningCount++;
	}

	void Rock::OnReset() {
		// �ϐ��̏�����
		m_miningCount = 0;

		//�h���[�R���|�[�l���g
		

		// �I�u�W�F�N�g�̍X�V���ĊJ
		SetUpdateActive(true);
	}

	void Rock::AccordingState() {
		switch (m_state)
		{
		case eState::Damage:
			// �������̌����ڂɕύX���鏈��������

			break;
		case eState::Broken:
			// �j�󂳂ꂽ�A�j���[�V�������Đ����鏈��������

			// �j�󎞂̃p�[�e�B�N�����Đ����鏈��������

			break;
		case eState::None:
			// �X�V���~���A�s���ɂ���
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();

			break;
		default:
			break;
		}
	}

}