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

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void RailManager::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		// csv�̎擾�ƃX�P�[��
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		m_guideMap = stageMap;

		// ��d���[�v
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// ���[��ID�Ɛ�[���[��ID�ȊO�͖���
				if (!Utility::GetBetween(stageMap.at(row).at(col), RAIL_ID, DERAIL_ID)) continue;

				// �C���X�^���X�`���ǉ�
				AddInstanceRail(row, col);

				// ��[���[���Ȃ�K�C�hID��ݒ�
				if (stageMap.at(row).at(col) == DERAIL_ID)
				{
					SetGuideID(row, col);
				}
			}
		}
	}

	// ���[���̒ǉ�
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// �X�e�[�Wcsv�̎擾
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

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
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		
		// �v�f�����͈͓��ŁA��[���[���Ȃ�ʏ�̃��[���ɂ���
		for (const auto& elem : GetElemsCheck(row, col, stageMap))
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

		// �v�f�����͈͓��ŁA���������Ȃ�K�C�h�ɂ���
		for (auto& elem : GetElemsCheck(row, col, m_guideMap))
		{
			if (elem.isRange)
			{
				int& num = m_guideMap.at(elem.row).at(elem.col);
				if (num == 0)
				{
					num = GUIDE_ID; // �K�C�h�ɏ�������

					// �K�C�h�̗�ƍs�̔ԍ���ێ�
					m_guidePoints.push_back(Point2D<size_t>(elem.row, elem.col));
				}
			}
		}
	}

	// �O�㍶�E�̔z��`�F�b�J�[�擾
	vector<CSVElementCheck> RailManager::GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap) const
	{
		vector<CSVElementCheck> elems; // �O�㍶�E�̌��ʕۑ��p�z��
		if (csvMap.empty()) return elems; // �Q�Ƃ���z�񂪋�Ȃ���Ԃ�

		elems = {
			{row - 1, col, WithInElemRange(row - 1, csvMap.size())},		// �O
			{row + 1, col, WithInElemRange(row + 1, csvMap.size())},		// ��
			{row, col - 1, WithInElemRange(col - 1, csvMap.at(row).size())},// ��
			{row, col + 1, WithInElemRange(col + 1, csvMap.at(row).size())},// �E
		};

		return elems;
	}

	// �w��̍��W�ɃK�C�h�����邩
	bool RailManager::GetIsGuidePoint(const Point2D<size_t>& point) const
	{
		// �z��͈͓̔����̃G���[�`�F�b�N
		if (!WithInElemRange(point.y, point.y, m_guideMap))
		{
			return false;
		}

		// �K�C�hID���ǂ���
		int num = m_guideMap.at(point.y).at(point.y);
		return num == GUIDE_ID;
	}
}