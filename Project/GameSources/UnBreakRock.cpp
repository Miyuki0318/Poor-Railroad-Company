/*!
@file UnBreakRock.cpp
@brief �j��ł��Ȃ���`��I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

const float DEFF_SCALE = 0.28f; // �f�t�H���g�̃X�P�[��

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;
	using namespace UnBreakRockData;

	// �������̏���
	void InstanceRock::OnCreate()
	{
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(UnBreakRockMeshMap.at(m_meshType));
		m_ptrDraw->SetTextureResource(L"UN_BREAK_ROCK_TX");
		m_ptrDraw->SetOwnShadowActive(true);
	}

	// �������̏���
	void UnBreakRock::OnCreate()
	{
		// �p�����̐������̏���
		TemplateObject::OnCreate();

		// �X�e�[�W�̎擾
		const shared_ptr<BaseStage>& stagePtr = GetTypeStage<BaseStage>();
		
		// ���b�V�����̃C���X�^���X�`��p�I�u�W�F�N�g�𐶐�
		m_instanceMap.emplace(eRockMeshType::Mesh1, stagePtr->AddGameObject<InstanceRock>(eRockMeshType::Mesh1));
		m_instanceMap.emplace(eRockMeshType::Mesh2, stagePtr->AddGameObject<InstanceRock>(eRockMeshType::Mesh2));
		m_instanceMap.emplace(eRockMeshType::Mesh3, stagePtr->AddGameObject<InstanceRock>(eRockMeshType::Mesh3));

		// �X�e�[�W�}�b�vcsv���琶��
		const auto& stageMap = stagePtr->GetStageMap();
		for (int row = 0; row < stageMap.size(); row++)
		{
			for (int col = 0; col < stageMap.at(row).size(); col++)
			{
				// �󂹂Ȃ��₶��Ȃ��Ȃ疳��
				if (eStageID::UnBreakRock != STAGE_ID(stageMap.at(row).at(col))) continue;

				// ���[�e�[�V����
				Quat quatRot;
				float rot = RangeRand(2, -1) * XM_PIDIV2;
				quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, rot, 0.0f));
				m_mtxRotation.rotation(quatRot);

				// �X�P�[��
				float scaleY = DEFF_SCALE * RangeRand(2.5f, 1.0f);
				m_mtxScale.scale(Vec3(DEFF_SCALE, scaleY, DEFF_SCALE));
				
				// �|�W�V����
				m_mtxPosition.translation(Vec3(float(col), 0.5f, -float(row)));

				// �ǉ�����p�̍s��
				Mat4x4 matrix = m_mtxScale * m_mtxRotation * m_mtxPosition;

				// ���b�V���^�C�v�������_���ɐݒ�
				eRockMeshType type = static_cast<eRockMeshType>(RangeRand(2, 0));
				m_instanceMap.at(type).lock()->AddMatrix(matrix);
			}
		}
	}
}