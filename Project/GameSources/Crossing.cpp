/*!
@file Crossing.cpp
@brief ���؂Ɠ��؊Ǘ��N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;
	
	// �������̏���
	void Crossing::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();

		// �g�����X�t�H�[���̐ݒ�
		size_t row, col;
		GetLineStringToRowCol(row, col, m_csvLine);
		SetPosition(ROWCOL2POS(row, col));
		SetScale(1.0f);

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"CROSSING");
		m_ptrDraw->AddAnimation(L"OPEN", 0, 15, false);
		m_ptrDraw->AddAnimation(L"CROSS", 20, 15, false);
	}

	// ���t���[���X�V����
	void Crossing::OnUpdate()
	{
		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME);

		// �A�j���[�V�������I�����Ă��Ȃ��Ȃ炱��ȍ~�͖���
		if (!m_ptrDraw->IsTargetAnimeEnd()) return;

		// csv��row��col���擾
		size_t row, col;
		GetLineStringToRowCol(row, col, m_csvLine);

		// �X�e�[�Wcsv���玩�g�̈ʒu��ID���擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		eStageID id = STAGE_ID(stageMap.at(row).at(col));

		// �A�j���[�V������OPEN�ŁA���Ă��鎞��ID�Ȃ�J���Ă��鎞��ID�ɐ؂�ւ���
		if (m_ptrDraw->GetCurrentAnimation() == L"OPEN" && eStageID::CrossingCross == id)
		{
			stageMap.at(row).at(col) = UnSTAGE_ID(eStageID::CrossingOpen);
		}

		// �A�j���[�V������CROSS�ŁA�J���Ă��鎞��ID�Ȃ���Ă��鎞��ID�ɐ؂�ւ���
		if (m_ptrDraw->GetCurrentAnimation() == L"CROSS" && eStageID::CrossingOpen == id)
		{
			stageMap.at(row).at(col) = UnSTAGE_ID(eStageID::CrossingCross);
		}
	}

	// �������̏���
	void CrossingManager::OnCreate()
	{
		// �X�e�[�W�̃|�C���^���擾
		const auto& stagePtr = GetStage();

		// ��Ԃ̃|�C���^��ێ�
		m_trainPtr = stagePtr->GetSharedGameObject<GameTrain>(L"Train");

		// ���[���f�[�^�}�b�v�̃|�C���^��ێ�
		m_railDataMap = &stagePtr->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
	}

	// ���t���[���X�V����
	void CrossingManager::OnUpdate()
	{
		// ��Ԃ̃|�C���^��������Ζ���
		if (!m_trainPtr.lock()) return;

		// ��Ԃ̍��W���擾
		Vec3 trainPos = m_trainPtr.lock()->GetPosition();

		// ���؂�S�Ċm�F
		for (auto& crossing : m_crossingMap)
		{
			auto& ptr = crossing.second.lock(); // �V�F�A�h�|�C���^�̎擾

			// ���������߁A�J���邩��X�V���������邩��ݒ�
			float length = (ptr->GetPosition() - trainPos).length();
			length <= m_openRange ? ptr->SetCross() : ptr->SetOpen();
			ptr->SetUpdateActive(length <= m_updateRange);
		}
	}

	// �w��̍��W�ɃK�C�h�����邩
	bool CrossingManager::GetIsRailPoint(const Point2D<size_t>& point) const
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();

		// �z��͈͓̔����̃G���[�`�F�b�N
		const auto& stageMap = stagePtr->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// ���[��ID���ǂ���
		if (eStageID::Rail != STAGE_ID(stageMap.at(point.x).at(point.y))) return false;

		// ���[�����������[�����ǂ���
		if (m_railDataMap->find(ROWCOL2LINE(point.x, point.y)) == m_railDataMap->end()) return false;
		if (m_railDataMap->at(ROWCOL2LINE(point.x, point.y)).angle != eRailAngle::Straight) return false;

		// �S�Ă̏�����ʉ߂�����true
		return true;
	}

	// ���؂̒ǉ�����
	void CrossingManager::AddCrossing(const Point2D<size_t>& point)
	{
		// �X�e�[�W��csv�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		auto& stageMap = stagePtr->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return;

		// ���؂̐���
		string addLine = ROWCOL2LINE(point.x, point.y);
		auto& crossing = stagePtr->AddGameObject<Crossing>(addLine);
		m_crossingMap.emplace(addLine, crossing);

		// ���[���̌����ɉ����č����s���ݒ�
		crossing->SetModelMatrix(m_railDataMap->at(addLine).type == eRailType::AxisXLine ? m_xLineModelMat : m_zLineModelMat);

		// csv�̏�������
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::CrossingCross);
	}
}