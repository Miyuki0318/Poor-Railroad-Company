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
		SetScale(m_startScale);

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

		switch (m_currentState)
		{
		case eState::Normal:
			m_currentState = eState::Damage;
			break;

		case eState::Damage:
			m_currentState = eState::None;
			break;

		default:
			break;
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
		AccordingState();
	}

	void Tree::OnMining() {
		MiningObject::OnMining();
	}

	void Tree::OnReset() {
		// �ϐ��̏�����
		m_miningCount = 0;
		m_currentState = eState::Normal;
		m_pastState = m_currentState;

		// �I�u�W�F�N�g�X�P�[����������Ԃɂ���
		SetScale(m_startScale);
		
		// �I�u�W�F�N�g�̍X�V���ĊJ
		SetUpdateActive(true);
	}

	void Tree::AccordingState() {
		if (m_currentState != m_pastState) {
			switch (m_currentState)
			{
			case eState::Damage:
				// �X�P�[���𔼕��ɕύX
				SetScale(m_damageScale);
				break;

			case eState::None:
				// �X�P�[����0�ɕύX
				SetScale(0.0f);

				// �X�V���~���A�s���ɂ���
				SetUpdateActive(false);
				SetDrawActive(false);
				MiningObject::OnDelete();

				break;
			default:
				break;
			}

			// �O�t���[���̃X�e�[�g�Ƃ��ĕۑ�
			m_pastState = m_currentState;
		}
	}


	void Rock::OnCreate() {
		MiningObject::OnCreate();

		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(3, 1);
		ptrDraw->SetMeshResource(L"ROCK" + to_wstring(random));
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		//ptrDraw->SetOwnShadowActive(true);

		// �^�O�̐ݒ�
		AddTag(L"ROCK");
	}

	void Rock::OnUpdate() {
		AccordingState();
	}

	void Rock::OnMining() {
		MiningObject::OnMining();
	}

	void Rock::OnReset() {
		// �ϐ��̏�����
		m_miningCount = 0;
		m_currentState = eState::Normal;
		m_pastState = m_currentState;

		// �I�u�W�F�N�g�X�P�[����������Ԃɂ���
		SetScale(m_startScale);

		// �I�u�W�F�N�g�̍X�V���ĊJ
		SetUpdateActive(true);
	}

	void Rock::AccordingState() {
		if (m_currentState != m_pastState) {
			switch (m_currentState)
			{
			case eState::Damage:
				// �X�P�[���𔼕��ɕύX
				SetScale(m_damageScale);
				break;

			case eState::None:
				// �X�P�[����0�ɕύX
				SetScale(0.0f);

				// �X�V���~���A�s���ɂ���
				SetUpdateActive(false);
				SetDrawActive(false);
				MiningObject::OnDelete();

				break;
			default:
				break;
			}

			// �O�t���[���̃X�e�[�g�Ƃ��ĕۑ�
			m_pastState = m_currentState;
		}
	}

}