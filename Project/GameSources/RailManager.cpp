/*!
@file RailManager.cpp
@brief ���[���Ǘ��N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

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
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (!Utility::GetBetween(id, eStageID::Rail, eStageID::GoalRail)) continue;

				// �C���X�^���X�`���ǉ�
				AddInstanceRail(row, col);

				// ��[���[���Ȃ�K�C�hID��ݒ�
				if (id == eStageID::DeRail)
				{
					SetGuideID(row, col);
				}

				// �S�[�����[���Ȃ�
				if (id == eStageID::GoalRail)
				{
					stagePtr->AddGameObject<Station>(LINE2POS(row - 2, col));
				}

				// ���[���Ȃ�
				if (id == eStageID::Rail || id == eStageID::DeRail)
				{
					AddRailDataMap(row, col);
					m_pastLine = LINE(row, col);
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

		// ���[���}�b�v�ɒǉ�
		AddRailDataMap(point.x, point.y);

		// �S�[�����[���ƌq���������̊m�F
		CheckConnectionGoalRail(point.x, point.y);

		// �ێ�����f�[�^��ύX
		m_railNum++;
		m_pastLine = LINE(point.x, point.y);

		// csv�̏�������
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::DeRail);
		m_guideMap = stageMap;
		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);
	}

	// �C���X�^���X�`��̒ǉ�
	void RailManager::AddInstanceRail(size_t row, size_t col)
	{
		// ���W�̐ݒ�
		Vec3 addPos = LINE2POS(row, col);

		// �g�����X�t�H�[���s��̐ݒ�
		Mat4x4 matrix, mtxPosition;
		mtxPosition.translation(addPos);

		// �s��̐ݒ�ƒǉ�
		matrix = m_mtxScale * m_mtxRotation * mtxPosition;
		m_ptrDraw->AddMatrix(matrix);
	}

	// �}�b�v�f�[�^�ɒǉ�
	void RailManager::AddRailDataMap(size_t row, size_t col)
	{
		// ���W�̐ݒ�
		Vec3 addPos = LINE2POS(row, col);

		// ���[���f�[�^�̐ݒ�
		RailData data = {};

		// 1�O�̃��[���f�[�^(������΋�)
		RailData* pastData = (m_railDataMap.find(m_pastLine) != m_railDataMap.end()) ? &m_railDataMap.at(m_pastLine) : &RailData();
		data.thisPos = addPos;				// ���g�̍��W
		data.pastPos = pastData->thisPos;	// 1�O�̍��W
		pastData->futurePos = addPos;		// 1�O�̃f�[�^�Ɏ��g�̍��W��o�^

		bool isLineX = (addPos.z == pastData->thisPos.z);	// X���Œ�����
		bool isLineZ = (addPos.x == pastData->thisPos.x);	// Z���Œ�����
		bool isRight = (addPos.x > pastData->thisPos.x);	// �E�Ɉړ�����̂�
		bool isUpper = (addPos.z > pastData->thisPos.z);	// ��Ɉړ�����̂�

		// �������A��������Ȃ����1�O�̂�
		data.type = isLineX ? eRailType::AxisXLine : isLineZ ? eRailType::AxisZLine : pastData->type;

		// 1�O�̃��[���ƃ^�C�v��������������ݒ�
		if (data.type != pastData->type)
		{
			pastData->angle = isRight ? (isUpper ? eRailAngle::Right : eRailAngle::Left) : (isUpper ? eRailAngle::Left : eRailAngle::Right);
			pastData->type = isRight ? (isUpper ? eRailType::Right2Under : eRailType::Right2Upper) : (isUpper ? eRailType::Left2Upper : eRailType::Left2Under);
		}

		// �}�b�v�ɒǉ�
		m_railDataMap.emplace(LINE(row, col), data);
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
				num = num == UnSTAGE_ID(eStageID::DeRail) ? UnSTAGE_ID(eStageID::Rail) : num;
			}
		}
	}

	// �K�C�h�̒ǉ�
	void RailManager::SetGuideID(size_t row, size_t col)
	{
		m_guidePoints.clear(); // ������
		m_pastDeRailPos = LINE2POS(row, col);

		// �G���[�`�F�b�N
		if (m_railDataMap.find(m_pastLine) == m_railDataMap.end()) return;

		// ���[���f�[�^�̎擾
		RailData data = m_railDataMap.at(m_pastLine);
		RailData past = m_railDataMap.at(POS2LINE(data.pastPos));

		// �K�C�h�t��csv�}�b�v����ݒu�ʒu�̏㉺���E���擾
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			// �z��͈̔͊O�Ȃ疳��
			if (!elem.isRange) continue;

			// 1�O�̃��[������������Ȃ����
			if (past.angle != eRailAngle::Straight)
			{
				// �ݒu�������[�������E�ł̒����Ȃ�
				if (data.type == eRailType::AxisXLine)
				{
					// ���E�����ɂ̂݃K�C�h��ǉ�
					if (elem.dir == eNextElemDir::DirLeft || elem.dir == eNextElemDir::DirRight)
					{
						AddGuideID(elem.row, elem.col);
					}
				}

				// �ݒu�������[�����㉺�ł̒����Ȃ�
				if (data.type == eRailType::AxisZLine)
				{
					// �㉺�����ɂ̂݃K�C�h��ǉ�
					if (elem.dir == eNextElemDir::DirBack || elem.dir == eNextElemDir::DirFlont)
					{
						AddGuideID(elem.row, elem.col);
					}
				}

				continue;
			}

			// 1�O�̃��[���������Ȃ�㉺���E�ɃK�C�h��ǉ�
			AddGuideID(elem.row, elem.col);
		}
	}

	// �K�C�h�̒ǉ�
	void RailManager::AddGuideID(size_t row, size_t col)
	{
		// �K�C�h�t��csv���珑�������ԍ����擾
		int& num = m_guideMap.at(row).at(col);
		int guideID = UnSTAGE_ID(eStageID::GuideRail);

		// ���������A�܂��̓K�C�h�Ȃ�
		if (num == 0 || num == guideID)
		{
			num = guideID; // �K�C�h�ɏ�������

			// �K�C�h�̗�ƍs�̔ԍ���ێ�
			m_guidePoints.push_back(Point2D<size_t>(row, col));
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
		return num == UnSTAGE_ID(eStageID::GuideRail);
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

	void RailManager::CheckConnectionGoalRail(size_t row, size_t col)
	{
		// ���[���f�[�^�̎擾
		RailData data = m_railDataMap.at(LINE(row, col));

		// �K�C�h�t��csv�}�b�v����ݒu�ʒu�̏㉺���E���擾
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			if (!elem.isRange) continue;
			if (eStageID::GoalRail != STAGE_ID(m_guideMap.at(elem.row).at(elem.col))) continue;

			// �ݒu�������[�������E�ł̒����Ȃ�
			if (data.type == eRailType::AxisXLine)
			{
				// ���E�����ɂ̂݃K�C�h��ǉ�
				if (elem.dir == eNextElemDir::DirLeft || elem.dir == eNextElemDir::DirRight)
				{
					AddRailDataMap(elem.row, elem.col);
				}
			}

			// �ݒu�������[�����㉺�ł̒����Ȃ�
			if (data.type == eRailType::AxisZLine)
			{
				// �㉺�����ɂ̂݃K�C�h��ǉ�
				if (elem.dir == eNextElemDir::DirBack || elem.dir == eNextElemDir::DirFlont)
				{
					AddRailDataMap(elem.row, elem.col);
				}
			}
		}
	}
}