/*!
@file RailManager.cpp
@brief ���[���Ǘ��N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

// �J�[�u���[���̕����ݒ�p�^�U
#define LEFT2UNDER(current, past)  past.pastPos.x < current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisZLine	// �����牺
#define LEFT2UPPER(current, past)  past.pastPos.x < current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisZLine	// �������
#define RIGHT2UNDER(current, past) past.pastPos.x > current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisZLine	// �E���牺	
#define RIGHT2UPPER(current, past) past.pastPos.x > current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisZLine	// �E�����
#define UPPER2LEFT(current, past)  past.pastPos.x > current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisXLine	// �ォ�獶
#define UPPER2RIGHT(current, past) past.pastPos.x < current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisXLine	// �ォ��E
#define UNDER2LEFT(current, past)  past.pastPos.x > current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisXLine	// �����獶
#define UNDER2RIGHT(current, past) past.pastPos.x < current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisXLine	// ������E

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �s��v�Z�Ɏg���X�P�[��
	const Mat4x4 mtxScale = Mat4x4().scale(Vec3(0.675f));

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
	void InstanceRail::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(m_meshKey);
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);
	}

	// �C���X�^���X�`��̍s���ǉ�
	void InstanceRail::AddRail(Mat4x4 matrix)
	{
		if (!m_ptrDraw) return;
		m_ptrDraw->AddMatrix(matrix);
	}

	// �C���X�^���X�`��̍s���ǉ�
	void InstanceRail::ResetRail()
	{
		m_ptrDraw->ClearMatrixVec();
	}

	// �s��̔z����擾
	vector<Mat4x4>& InstanceRail::GetMatrix()
	{
		return m_ptrDraw->GetMatrixVec();
	}

	// �������̏���
	void RailManager::OnCreate()
	{
		// csv�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// �C���X�^���X�`��̃��[���𐶐�
		m_instanceRail.emplace(eRailAngle::Left, stagePtr->AddGameObject<InstanceRail>(L"TURNRAIL"));
		m_instanceRail.emplace(eRailAngle::Straight, stagePtr->AddGameObject<InstanceRail>(L"RAIL"));

		// ���[���`�搶��
		ResetInstanceRail();
	}

	// �X�V����
	void RailManager::OnUpdate()
	{
		// �X�e�[�W�Ɨ�Ԃ̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		auto& train = stagePtr->GetSharedGameObject<GameTrain>(L"Train", false);
		if (!train) return;

		// ��Ԃ̈ړ��悪��[���[������Ȃ���Ζ���
		string moveLine = train->GetRailPosLine();
		if (m_pastLine != moveLine) return;
		if (m_railDataMap.find(moveLine) == m_railDataMap.end()) return;

		// ���[���f�[�^���擾���K�C�h��������
		auto& data = m_railDataMap.at(moveLine);
		m_guidePoints.clear();
		m_guideMap = stagePtr->GetStageMap();

		// �z��̗v�f�ԍ����擾
		size_t row, col;
		GetLineStringToRowCol(row, col, moveLine);

		// �K�C�h�t��csv�}�b�v����ݒu�ʒu�̏㉺���E���擾
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			// �z��͈̔͊O�Ȃ疳��
			if (!elem.isRange) continue;

			// �㉺���E�̓��Œ����Ȃ�K�C�h��ǉ�
			if (CheckStraightRail(data, elem.dir))
			{
				AddGuideID(elem.row, elem.col);
			}
		}
	}

	// �C���X�^���X�`��̃��[���𐶐�
	void RailManager::ResetInstanceRail()
	{
		// �X�e�[�W��CSV�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// ������
		m_isConnectionGoal = false;
		m_railDataMap.clear();

		// ������
		for (auto& rail : m_instanceRail)
		{
			rail.second.lock()->ResetRail();
		}

		// ��d���[�v
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// ���[��ID�Ɛ�[���[��ID�ȊO�͖���
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (!GetBetween(id, eStageID::Rail, eStageID::DrawRail)) continue;

				// �J�n���[���Ȃ�K�C�hID��ݒ�
				if (id == eStageID::StartRail) m_startRailPos = ROWCOL2POS(row, col);

				// ��[���[���Ȃ�K�C�hID��ݒ�
				if (id == eStageID::DeRail) SetGuideID(row, col);

				// �S�[�����[���Ȃ�
				if (id == eStageID::GoalRail)
				{
					// �w�����݂���΍��W�̕ύX�A�Ȃ���ΐ���
					auto station = stagePtr->GetSharedGameObject<Station>(L"Station", false);
					if (!station)
					{
						station = stagePtr->AddGameObject<Station>(ROWCOL2POS(row - 2, col));
						stagePtr->SetSharedGameObject(L"Station", station);
					}
					else {
						station->OnReset(ROWCOL2POS(row - 2, col));
					}
				}

				// ���[���A�܂��͐�[���[���Ȃ�
				if (ORS(id, eStageID::Rail, eStageID::DeRail, eStageID::StartRail))
				{
					AddRailDataMap(row, col);
					m_pastLine = ROWCOL2LINE(row, col);
				}

				// �C���X�^���X�`���ǉ�
				AddInstanceRail(row, col);
			}
		}
	}

	// ���[���̒ǉ�
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// �X�e�[�Wcsv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// ���[���}�b�v�ɒǉ�
		AddRailDataMap(point.x, point.y);

		// �C���X�^���X�`���ǉ�
		AddInstanceRail(point.x, point.y);

		// �S�[�����[���ƌq���������̊m�F
		CheckConnectionGoalRail(point.x, point.y);

		// �ێ�����f�[�^��ύX
		m_railNum++;
		m_pastLine = ROWCOL2LINE(point.x, point.y);

		// csv�̏�������
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::DeRail);
		m_guideMap = stageMap;
		if (m_isConnectionGoal)
		{
			m_guidePoints.clear();
			return;
		}

		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);
	}

	// �C���X�^���X�`��̒ǉ�
	void RailManager::AddInstanceRail(size_t row, size_t col, eRailAngle angle)
	{
		// ���W��LINE�̐ݒ�
		Vec3 addPos = ROWCOL2POS(row, col);
		string line = ROWCOL2LINE(row, col);

		// �g�����X�t�H�[���s��̐ݒ�
		Mat4x4 matrix, mtxPosition, mtxRotation;
		mtxPosition.translation(addPos);

		// ���[�e�[�V�����̐ݒ�
		eRailType type = m_railDataMap.find(line) != m_railDataMap.end() ? m_railDataMap.at(line).type : eRailType::AxisXLine;
		mtxRotation.rotation((Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, m_railAngleMap.at(type), 0.0f)));

		// �s��̐ݒ�ƒǉ�
		matrix = mtxScale * mtxRotation * mtxPosition;
		m_instanceRail.at(angle).lock()->AddRail(matrix);
	}

	// �C���X�^���X�`��̃J�[�u���[����ǉ�
	void RailManager::AddInstanceCurveRail(RailData& pastData)
	{
		// �V�����s��z��
		vector<Mat4x4> newVec;

		// ���݂̒������[���̍s��z����擾
		auto& matVec = m_instanceRail.at(eRailAngle::Straight).lock()->GetMatrix();
		for (size_t i = 0; i < matVec.size(); i++)
		{
			// �J�[�u�������[���ȊO���R�s�[
			if (matVec.at(i).getTranslation() != pastData.thisPos) newVec.push_back(matVec.at(i));
		}

		// �z����㏑��
		matVec.swap(newVec);

		// �J�[�u���[���`���ǉ�
		AddInstanceRail(ROW(pastData.thisPos.z), COL(pastData.thisPos.x), eRailAngle::Left);
	}

	// �}�b�v�f�[�^�ɒǉ�
	void RailManager::AddRailDataMap(size_t row, size_t col)
	{
		// ���W�̐ݒ�
		Vec3 addPos = ROWCOL2POS(row, col);

		// ���[���f�[�^�̐ݒ�
		RailData data = {};
		RailData* pastData = (m_railDataMap.find(m_pastLine) != m_railDataMap.end()) ? &m_railDataMap.at(m_pastLine) : &RailData();
		data.thisPos = addPos; // ���g�̍��W
		if (pastData->thisPos == Vec3(0.0f)) pastData->thisPos = addPos; // ��O�̃f�[�^�̍��W����Ȃ�
		data.pastPos = pastData->thisPos; // 1�O�̍��W�����W��0.0f�ŏ����l�Ȃ玩�g�̍��W
		pastData->futurePos = addPos; // 1�O�̃f�[�^�Ɏ��g�̍��W��o�^

		bool isLineX = (addPos.z == pastData->thisPos.z);	// X���Œ�����
		bool isLineZ = (addPos.x == pastData->thisPos.x);	// Z���Œ�����
		bool isRight = (addPos.x > pastData->thisPos.x);	// �E�Ɉړ�����̂�
		bool isUpper = (addPos.z > pastData->thisPos.z);	// ��Ɉړ�����̂�

		// �������A��������Ȃ����1�O�̂�
		data.type = isLineX ? eRailType::AxisXLine : isLineZ ? eRailType::AxisZLine : pastData->type;

		// 1�O�̃��[���ƃ^�C�v��������������ݒ�
		if (data.type != pastData->type)
		{
			// ���[���̃A���O����ݒ�
			SetPastRailDataAngle(data, *pastData);

			// ���[���̃^�C�v��ݒ�
			SetPastRailDataType(data, *pastData);

			// �J�[�u���[����ǉ�
			AddInstanceCurveRail(*pastData);
		}

		// �}�b�v�ɒǉ�
		m_railDataMap.emplace(ROWCOL2LINE(row, col), data);
	}

	// �J�[�u�ɕς�������[���̃^�C�v��ݒ�
	void RailManager::SetPastRailDataType(RailData& current, RailData& past)
	{
		// ���݂̃��[���ƑO�X��̃��[���Ƃ̑��΍��W�ƃ��[����ݒu���������ɉ����đO��̃��[���̃^�C�v��ύX����
		if (LEFT2UNDER(current, past))  past.type = eRailType::Left2Under;
		if (LEFT2UPPER(current, past))  past.type = eRailType::Left2Upper;
		if (RIGHT2UNDER(current, past)) past.type = eRailType::Right2Under;
		if (RIGHT2UPPER(current, past)) past.type = eRailType::Right2Upper;
		if (UNDER2LEFT(current, past))  past.type = eRailType::Left2Under;
		if (UNDER2RIGHT(current, past)) past.type = eRailType::Right2Under;
		if (UPPER2LEFT(current, past))  past.type = eRailType::Left2Upper;
		if (UPPER2RIGHT(current, past)) past.type = eRailType::Right2Upper;
	}

	// �J�[�u�ɕς�������[���̃A���O����ݒ�
	void RailManager::SetPastRailDataAngle(RailData& current, RailData& past)
	{
		// ���݂̃��[���ƑO�X��̃��[���Ƃ̑��΍��W�ƃ��[����ݒu���������ɉ����đO��̃��[���̃A���O����ύX����
		if (LEFT2UNDER(current, past))  past.angle = eRailAngle::Right;
		if (LEFT2UPPER(current, past))  past.angle = eRailAngle::Left;
		if (RIGHT2UNDER(current, past)) past.angle = eRailAngle::Left;
		if (RIGHT2UPPER(current, past)) past.angle = eRailAngle::Right;
		if (UNDER2LEFT(current, past))  past.angle = eRailAngle::Left;
		if (UNDER2RIGHT(current, past)) past.angle = eRailAngle::Right;
		if (UPPER2LEFT(current, past))  past.angle = eRailAngle::Right;
		if (UPPER2RIGHT(current, past)) past.angle = eRailAngle::Left;
	}

	// ��[���[���̏�������
	void RailManager::SetRailID(size_t row, size_t col) const
	{
		// csv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// �v�f�����͈͓��ŁA��[���[���Ȃ�ʏ�̃��[���ɂ���
		for (const auto& elem : CSVElementCheck::GetElemsCheck(row, col, stageMap))
		{
			// �z��͈̔͊O�Ȃ疳��
			if (!elem.isRange) continue;

			// ��������
			int& num = stageMap.at(elem.row).at(elem.col);
			num = num == UnSTAGE_ID(eStageID::DeRail) ? UnSTAGE_ID(eStageID::Rail) : num;
		}
	}

	// �K�C�h�̒ǉ�
	void RailManager::SetGuideID(size_t row, size_t col)
	{
		m_guidePoints.clear(); // ������
		m_pastDeRailPos = ROWCOL2POS(row, col);

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
				// �ݒu�������[���������Ȃ�
				if (CheckStraightRail(data, elem.dir))
				{
					AddGuideID(elem.row, elem.col);
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
		if (!WithInElemRange(point.x, point.y, m_guideMap)) return false;

		// �K�C�hID���ǂ���
		return eStageID::GuideRail == STAGE_ID(m_guideMap.at(point.x).at(point.y));
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

	// ���[���������ɕ���ł��邩
	bool RailManager::CheckStraightRail(RailData data, eNextElemDir dir)
	{
		return (data.type == eRailType::AxisZLine && OR(dir, eNextElemDir::DirFlont, eNextElemDir::DirBack))
			|| (data.type == eRailType::AxisXLine && OR(dir, eNextElemDir::DirLeft, eNextElemDir::DirRight));
	}

	// ���[�����S�[�����[���ƌq��������
	void RailManager::CheckConnectionGoalRail(size_t row, size_t col)
	{
		// ���[���f�[�^�̎擾
		string line = ROWCOL2LINE(row, col);
		RailData data = m_railDataMap.at(line);

		// �K�C�h�t��csv�}�b�v����ݒu�ʒu�̏㉺���E���擾
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			if (!elem.isRange) continue;
			if (eStageID::GoalRail != STAGE_ID(m_guideMap.at(elem.row).at(elem.col))) continue;

			// �ݒu�������[���������Ȃ�
			if (CheckStraightRail(data, elem.dir) && data.type == eRailType::AxisXLine)
			{
				m_pastLine = line;
				AddRailDataMap(elem.row, elem.col);
				m_isConnectionGoal = true;
				return;
			}

			if (!CheckStraightRail(data, elem.dir) && data.type == eRailType::AxisZLine)
			{
				m_pastLine = line;
				AddRailDataMap(elem.row, elem.col);
				m_isConnectionGoal = true;
				return;
			}
		}
	}
}