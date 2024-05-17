/*!
@file RailManager.cpp
@brief ���[���Ǘ��N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

// ���[���Ɋւ���ID
#define RAIL_ID static_cast<size_t>(basecross::eStageID::Rail)
#define DERAIL_ID static_cast<size_t>(basecross::eStageID::DeRail)
#define GUIDE_ID static_cast<size_t>(basecross::eStageID::GuideRail)
#define GOALRAIL_ID static_cast<size_t>(basecross::eStageID::GoalRail)

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// line����row��col�𒊏o
	void GetLineStringToRowCol(size_t& row, size_t& col, string line)
	{
		// ������� '-' �����ɕ���
		istringstream iss(line);
		string token;

		// token�𐔒l�ɕϊ�����row�Ɋi�[
		getline(iss, token, '-');
		row = stoul(token);

		// token�𐔒l�ɕϊ�����col�Ɋi�[
		getline(iss, token);
		col = stoul(token);
	}

	// �������̏���
	void RailManager::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"RAIL");
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);

		// csv�̎擾�ƃX�P�[��
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// ��d���[�v
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// ���[��ID�Ɛ�[���[��ID�ȊO�͖���
				int id = stageMap.at(row).at(col);
				if (!Utility::GetBetween(id, RAIL_ID, GOALRAIL_ID)) continue;

				// �C���X�^���X�`���ǉ�
				AddInstanceRail(row, col);

				// ��[���[���Ȃ�K�C�hID��ݒ�
				if (STAGE_ID(id) == eStageID::DeRail)
				{
					SetGuideID(row, col);
				}

				// �S�[�����[���Ȃ�
				if (STAGE_ID(id) == eStageID::GoalRail)
				{
					stagePtr->AddGameObject<Station>(LINE2POS(row - 1, col));
				}
			}
		}
	}

	// ���[���̒ǉ�
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// �X�e�[�Wcsv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// �C���X�^���X�`���ǉ�
		AddInstanceRail(point.x, point.y);

		// csv�̏�������
		stageMap.at(point.x).at(point.y) = DERAIL_ID;
		m_guideMap = stageMap;
		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);
	}

	// ��[���[���̏�������
	void RailManager::SetRailID(size_t row, size_t col) const
	{
		// csv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		
		// �v�f�����͈͓��ŁA��[���[���Ȃ�ʏ�̃��[���ɂ���
		for (const auto& elem : CSVElementCheck::GetElemsCheck(row, col, stageMap))
		{
			if (elem.isRange)
			{
				int& num = stageMap.at(elem.row).at(elem.col);
				num = num == DERAIL_ID ? RAIL_ID : num;
			}
		}
	}

	// �K�C�h�̒ǉ�
	void RailManager::SetGuideID(size_t row, size_t col)
	{
		m_guidePoints.clear(); // ������
		m_pastDeRailPos = LINE2POS(row, col);

		// �v�f�����͈͓��ŁA���������Ȃ�K�C�h�ɂ���
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			if (elem.isRange)
			{
				int& num = m_guideMap.at(elem.row).at(elem.col);
				if (num == 0 || num == GUIDE_ID)
				{
					num = GUIDE_ID; // �K�C�h�ɏ�������

					// �K�C�h�̗�ƍs�̔ԍ���ێ�
					m_guidePoints.push_back(Point2D<size_t>(elem.row, elem.col));
				}
			}
		}
	}

	// �w��̍��W�ɃK�C�h�����邩
	bool RailManager::GetIsGuidePoint(const Point2D<size_t>& point) const
	{
		// �z��͈͓̔����̃G���[�`�F�b�N
		if (!WithInElemRange(point.x, point.y, m_guideMap))
		{
			return false;
		}

		// �K�C�hID���ǂ���
		int num = m_guideMap.at(point.x).at(point.y);
		return num == GUIDE_ID;
	}

	// �K�C�h�̍Čv�Z����
	void RailManager::GuideRecalculation()
	{
		// �X�e�[�Wcsv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		m_guideMap = stageMap;

		// �T�C�Y�Ɨ�ƍs
		Vec3 pos = m_pastDeRailPos;
		size_t row, col;
		row = ROW(pos.z);
		col = COL(pos.x);

		// �K�C�h�̍Đݒ�
		SetGuideID(row, col);
	}
}