/*!
@file MiningObject.cpp
@brief
@author �����
*/

#pragma once
#include "stdafx.h"
#include "Project.h"


namespace basecross {

	using namespace Utility;

	void MiningObject::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		TemplateObject::OnCreate();
		float rot = RangeRand(2, -1) * XM_PIDIV2;
		SetRotation(Vec3(0.0f, rot, 0.0f));
		SetPosition(m_spawnPos);

		//�`��R���|�[�l���g�̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);
		SetAlphaActive(false);

		const auto& stage = GetTypeStage<BaseStage>();
		auto& stageMap = stage->GetStageMap();

		size_t row, col;
		row = ROW(m_spawnPos.z);
		col = COL(m_spawnPos.x);
		m_csvPos = Point2D<size_t>(row, col);

		// �^�O�̐ݒ�
		AddTag(L"MiningObject");
	}

	void MiningObject::OnMining() {
		// �̌@�����񐔂̑���
		m_miningCount++;

		// �̌@�񐔂�����ɒB�����ꍇ�A�I�u�W�F�N�g��j��
		if (m_miningCount >= m_miningLimit) {
			m_state = eState::Broken;
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();
		}

	}
	void MiningObject::OnDelete() {
		const auto& stage = GetTypeStage<BaseStage>();
		auto& stageMap = stage->GetStageMap();

		if (Utility::WithInElemRange(m_csvPos.x, m_csvPos.y, stageMap))
		{
			stageMap.at(m_csvPos.x).at(m_csvPos.y) = 0;
		}

		stage->GetSharedGameObject<RailManager>(L"RailManager")->GuideRecalculation();
	}


	void Tree::OnCreate() {
		MiningObject::OnCreate();
		SetScale(Vec3(1.0f));

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(2, 1);
		ptrDraw->SetMultiMeshResource(L"TREE" + to_wstring(random));
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		//ptrDraw->SetOwnShadowActive(true);

		// �^�O�̐ݒ�
		AddTag(L"TREE");
	}

	void Tree::OnUpdate() {
		MiningObject::OnUpdate();

	}

	void Tree::OnMining() {
		MiningObject::OnMining();

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
		SetScale(Vec3(1.0f));

		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(3, 1);
		ptrDraw->SetMeshResource(L"ROCK" + to_wstring(random));
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		//ptrDraw->SetOwnShadowActive(true);

		// �^�O�̐ݒ�
		AddTag(L"ROCK");
	}

	void Rock::OnUpdate() {
		MiningObject::OnUpdate();

	}

	void Rock::OnMining() {
		MiningObject::OnMining();
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