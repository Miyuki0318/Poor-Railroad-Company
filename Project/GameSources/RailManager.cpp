#include "stdafx.h"
#include "Project.h"

#define RAIL_ID static_cast<size_t>(basecross::eStageID::Rail)
#define DERAIL_ID static_cast<size_t>(basecross::eStageID::DeRail)
#define GUIDE_ID static_cast<size_t>(basecross::eStageID::GuideRail)

const float stageDiff = 7.0f;
const Vec3 scale = Vec3(1.0f, 0.2f, 1.0f);

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

		// ���[�e�[�V�����p�̃N�H�[�^�j�I��
		Quat quat;
		quat.rotationRollPitchYawFromVector(Vec3(0.0f));

		// ��d���[�v
		for (int i = 0; i < stageMap.size(); i++)
		{
			for (int j = 0; j < stageMap.at(i).size(); j++)
			{
				// ���[��ID�Ɛ�[���[��ID�ȊO�͖���
				if (!Utility::GetBetween(stageMap.at(i).at(j), RAIL_ID, DERAIL_ID)) continue;

				// x��z���W
				float x, z;
				x = j - stageDiff;
				z = -i + stageDiff;

				// �s��̐錾
				Mat4x4 matrix, mtxT, mtxR, mtxS;

				// �N�H�[�^�j�I�����烍�[�e�[�V������ݒ�
				mtxR.rotation(quat);
				mtxS.scale(scale); // �X�P�[���̐ݒ�
				mtxT.translation(Vec3(x, 1.0f, z)); // ���W�̐ݒ�

				// �s��̐ݒ�ƒǉ�
				matrix = mtxS * mtxR * mtxT;
				m_ptrDraw->AddMatrix(matrix);

				// ��[���[���Ȃ�K�C�hID��ݒ�
				if (stageMap.at(i).at(j) == DERAIL_ID)
				{
					SetGuideID(i, j);
				}
			}
		}
	}

	// ���[���̒ǉ�
	void RailManager::AddRail(const Vec3& addPos)
	{
		// �X�e�[�Wcsv�̎擾
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// ��ƍs�����W���狁�߂�
		size_t i, j;
		j = size_t(addPos.x + stageDiff);
		i = size_t(addPos.z + stageDiff);

		// ���[�e�[�V�����p
		Quat quat;
		quat.rotationRollPitchYawFromVector(Vec3(0.0f));

		// �s��̐錾
		Mat4x4 matrix, mtxT, mtxR, mtxS;

		// �N�H�[�^�j�I�����烍�[�e�[�V������ݒ�
		mtxR.rotation(quat);
		mtxS.scale(scale); // �X�P�[���̐ݒ�
		mtxT.translation(Vec3(addPos.x, 1.0f, addPos.z)); // ���W�̐ݒ�

		// �s��̐ݒ�ƒǉ�
		matrix = mtxS * mtxR * mtxT;
		m_ptrDraw->AddMatrix(matrix);

		// csv�̏�������
		stageMap.at(i).at(j) = DERAIL_ID;
		m_guideMap = stageMap;
		SetGuideID(i, j);
		SetRailID(i, j);
	}

	// csv�̏�������
	void RailManager::SetRailID(size_t i, size_t j) const
	{
		// csv�̎擾
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// �ݒu�ʒu�̑O�㍶�E�����[��ID���K�C�hID�ɏ�������
		vector<CSVElementCheck> elems = {
			{i - 1, j, GetBetween(i - 1, 0, stageMap.size())},
			{i + 1, j, GetBetween(i + 1, 0, stageMap.size())},
			{i, j - 1, GetBetween(j - 1, 0, stageMap.at(i).size())},
			{i, j + 1, GetBetween(i + 1, 0, stageMap.at(i).size())},
		};

		// �v�f�������[�v
		for (const auto& elem : elems)
		{
			// �v�f�����͈͓��Ȃ�
			if (elem.isRange)
			{
				// ��[���[���Ȃ�ʏ�̃��[���ɁA���������Ȃ�K�C�h�ɁA�K�C�h�Ȃ牽�������ɁA����ȊO�Ȃ炻�̂܂�
				int& num = stageMap.at(elem.row).at(elem.col);
				num = num == DERAIL_ID ? RAIL_ID : num;
			}
		}
	}

	void RailManager::SetGuideID(size_t i, size_t j)
	{
		// �ݒu�ʒu�̑O�㍶�E�����[��ID���K�C�hID�ɏ�������
		vector<CSVElementCheck> elems = {
			{i - 1, j, GetBetween(i - 1, 0, m_guideMap.size())},
			{i + 1, j, GetBetween(i + 1, 0, m_guideMap.size())},
			{i, j - 1, GetBetween(j - 1, 0, m_guideMap.at(i).size())},
			{i, j + 1, GetBetween(i + 1, 0, m_guideMap.at(i).size())},
		};

		// �v�f�������[�v
		for (const auto& elem : elems)
		{
			// �v�f�����͈͓��Ȃ�
			if (elem.isRange)
			{
				// ��[���[���Ȃ�ʏ�̃��[���ɁA���������Ȃ�K�C�h�ɁA�K�C�h�Ȃ牽�������ɁA����ȊO�Ȃ炻�̂܂�
				int& num = m_guideMap.at(elem.row).at(elem.col);
				num = num == 0 ? GUIDE_ID : num;
			}
		}
	}
}