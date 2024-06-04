/*!
@file GatheringObject.cpp
@brief �̎�ł���I�u�W�F�N�g
@author �����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void InstanceGathers::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
	}

	// ���b�V���f�[�^��o�^
	void InstanceGathers::SetMeshData(const wstring& meshKey, const wstring& texKey)
	{
		m_ptrDraw->SetMeshResource(meshKey);
		m_ptrDraw->SetTextureResource(texKey);
	}

	// �s��ɒǉ�
	void InstanceGathers::AddMatrix(const Mat4x4& matrix)
	{
		m_ptrDraw->AddMatrix(matrix);
	}

	// �s���S�폜
	void InstanceGathers::AllClearMatrix()
	{
		m_ptrDraw->ClearMatrixVec();
	}

	// �s��z����擾
	vector<Mat4x4>& InstanceGathers::GetMatrixVec()
	{
		return m_ptrDraw->GetMatrixVec();
	}

	// �������̏���
	void GatheringManager::OnCreate()
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �e��ID�̃C���X�^���X�`��p�I�u�W�F�N�g�𐶐�����map�ɕێ�
		m_gathersMap.emplace(eStageID::Stone1, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Stone2, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Stone3, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Tree1, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Tree2, stagePtr->AddGameObject<InstanceGathers>());

		// ���b�V���ݒ�𑗂�
		SetGathersMeshData();

		// �J�n���̍̎�I�u�W�F�N�g���������𑗂�
		ResetGathering();
	}

	// ���b�V���L�[�ƍ����s��̐ݒ�
	void GatheringManager::SetGathersMeshData()
	{
		// �e��ID�ɉ����Đݒ�
		m_gathersMap.at(eStageID::Stone1).lock()->SetMeshData(L"ROCK1", L"ROCK_SM_TX");
		m_gathersMap.at(eStageID::Stone2).lock()->SetMeshData(L"ROCK2", L"ROCK_SM_TX");
		m_gathersMap.at(eStageID::Stone3).lock()->SetMeshData(L"ROCK3", L"ROCK_SM_TX");
		m_gathersMap.at(eStageID::Tree1).lock()->SetMeshData(L"TREE1", L"TREE_SM_TX");
		m_gathersMap.at(eStageID::Tree2).lock()->SetMeshData(L"TREE2", L"TREE_SM_TX");
	}

	// ��������
	void GatheringManager::ResetGathering()
	{
		// �s���S�ď�����
		for (auto& gathers : m_gathersMap)
		{
			gathers.second.lock()->AllClearMatrix();
		}

		// �X�e�[�Wcsv���擾��2�d���[�v
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		for (int row = 0; row < stageMap.size(); row++)
		{
			for (int col = 0; col < stageMap.at(row).size(); col++)
			{
				// ID�ɉ����čs���ǉ�
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (!GetBetween(id, eStageID::Stone1, eStageID::Tree2)) continue;
				if (m_gathersMap.find(id) == m_gathersMap.end()) continue;

				// �g�����X�t�H�[���s��̐ݒ�
				Mat4x4 matrix, mtxPosition, mtxRotation, mtxScale;
				mtxPosition.translation(ROWCOL2POS(row, col));
				mtxRotation.rotation(Quat().rotationRollPitchYawFromVector(Vec3(0.0f, RangeRand(2, -1) * XM_PIDIV2, 0.0f)));
				mtxScale.scale(m_fullScale);
				
				// �s��̐ݒ�ƒǉ�
				matrix = mtxScale * mtxRotation * mtxPosition;

				// �s���ǉ�
				m_gathersMap.at(id).lock()->AddMatrix(matrix);
			}
		}
	}

	// �̏W����
	int GatheringManager::Gathering(const Point2D<size_t>& point)
	{
		// �X�e�[�W��csv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return 0;

		// �X�e�[�WID�ɕϊ�
		int num = stageMap.at(point.x).at(point.y);
		eStageID id = STAGE_ID(num);
		if (m_gathersMap.find(id) == m_gathersMap.end()) return 0;

		// �s��̃X�P�[����ύX���邩�s�񂻂̂��̂����O���邩
		vector<Mat4x4> tempMat;
		auto& matrixVec = m_gathersMap.at(id).lock()->GetMatrixVec();
		for (auto& matrix : matrixVec)
		{
			// ���W����v������
			if (matrix.getTranslation() == ROWCOL2POS(point.x, point.y))
			{
				Vec3 scale = matrix.scaleInMatrix().round(1); // �X�P�[��

				// �X�P�[�����n�[�t�X�P�[���Ȃ�CSV����������
				if (scale == m_helfScale) HelfSizeGathering(point);

				// �X�P�[�����t���X�P�[���Ȃ�n�[�t�X�P�[���ɏ��������Ēǉ�
				if (scale == m_fullScale) tempMat.push_back(FullSizeGathering(matrix));
				
				continue;
			}
			// �ǉ�
			tempMat.push_back(matrix);
		}
		// �s��z���u��������ID��Ԃ�
		matrixVec.swap(tempMat);
		return num;
	}

	// �n�[�t�T�C�Y�ł̍̎�
	void GatheringManager::HelfSizeGathering(const Point2D<size_t>& point)
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();

		// �X�e�[�W��csv�̏�������
		auto& stageMap = stagePtr->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return;
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::None);

		// ���[���}�l�[�W���[�ɃK�C�h�̍Đ����֐��𑗂�
		stagePtr->GetSharedGameObject<RailManager>(L"RailManager")->GuideRecalculation();
	}

	// �t���T�C�Y�ł̍̎�
	Mat4x4 GatheringManager::FullSizeGathering(Mat4x4& matrix)
	{
		// �n�[�t�T�C�Y�X�P�[���ōs����쐬���Ԃ�
		Mat4x4 mat, tMat, rMat, sMat;
		tMat.translation(matrix.getTranslation());
		rMat.rotation(matrix.quatInMatrix());
		sMat.scale(m_helfScale);
		mat = sMat * rMat * tMat;
		return mat;
	}

	// �w��̍��W�ɍ̏W�I�u�W�F�N�g�����邩
	bool GatheringManager::GetIsGatheringPoint(const Point2D<size_t>& point) const
	{
		// �z��͈͓̔����̃G���[�`�F�b�N
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// �̏W�I�u�W�F�N�g��ID��
		eStageID id = STAGE_ID(stageMap.at(point.x).at(point.y));
		return GetBetween(id, eStageID::Stone1, eStageID::Tree2);
	}
}