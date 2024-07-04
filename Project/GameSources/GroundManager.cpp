/*!
@file GroundManager.cpp
@brief �n�ʊǗ��I�u�W�F�N�g�N���X
@author ���V���M
*/

#include "stdafx.h"
#include "OzawaUtility.h"
#include "GroundManager.h"

#define END_INDEX 10 // ��؂�܂ł̐�

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void InstanceGround::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetTextureResource(m_groundTexture);
		m_ptrDraw->SetOwnShadowActive(true);
	}

	// �������̏���
	void GroundManager::OnCreate()
	{
		// �I�u�W�F�N�g�̐���
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �n�ʃ}�b�v
		auto& groundMap = GetTypeStage<BaseStage>()->GetGroundMap();
		map<int, weak_ptr<InstanceGround>> grass, unGrass, sand, water, rock;
		for (int i = 0; i < groundMap.front().size(); i += END_INDEX)
		{
			// ��؂萔���ǉ�
			grass.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_GRASS_TX"));
			unGrass.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_GRASS_TX"));
			sand.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_SAND_TX"));
			water.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_WATER_TX"));
			rock.emplace(i, stagePtr->AddGameObject<InstanceGround>(L"G_ROCK_TX"));
		}

		// �e�^�C�v�Ő��������}�b�v��StageID���L�[�ɓ񎟌��}�b�v��
		m_groundMap.emplace(eStageID::Grass, grass);		// ���n
		m_groundMap.emplace(eStageID::UnGrass, unGrass);	// �ʉߕs�̑��n
		m_groundMap.emplace(eStageID::Sand, sand);			// ���n
		m_groundMap.emplace(eStageID::Watering, water);		// ����
		m_groundMap.emplace(eStageID::Rock, rock);			// ��n

		// �n�ʂ̐���
		CreateInstanceGround();
	}

	// ���t���[���X�V����
	void GroundManager::OnUpdate()
	{
		// �J�����̒����_���WX��
		int atX = int(GetStage()->GetView()->GetTargetCamera()->GetAt().x);

		// �J�����̒����_X������v�����āA��܂��ɉ�ʓ��ł���Ε\������
		for (auto& groud : m_groundMap)
		{
			for (auto& type : groud.second)
			{
				// �J�n�C���f�b�N�X����ʓ��ł��邩
				type.second.lock()->SetDrawActive(GetBetween(type.first, atX + m_windowRange, atX - m_windowRange));
			}
		}
	}

	// �n�ʐ�������
	void GroundManager::CreateInstanceGround()
	{
		// �n�ʃ}�b�v���擾
		auto& groundMap = GetTypeStage<BaseStage>()->GetGroundMap();

		// ��d���[�v
		for (int row = 0; row < groundMap.size(); row++)
		{
			for (int col = 0; col < groundMap.at(row).size(); col++)
			{
				// ���W�̐ݒ�
				Vec3 addPos = Vec3(float(col), m_defPosY, -float(row));

				// �g�����X�t�H�[���s��̐ݒ�
				Mat4x4 matrix, mtxPosition;
				mtxPosition.translation(addPos);

				// �s��̐ݒ�ƒǉ�
				matrix = m_mtxScale * m_mtxRotation * mtxPosition;

				// ID�ɉ����čs���ǉ�
				eStageID id = STAGE_ID(groundMap.at(row).at(col));
				for (auto& groud : m_groundMap)
				{
					// eStageID����v���Ȃ��Ȃ疳��
					if (groud.first != id) continue;

					// �^�C�v���Ƀ��[�v
					for (auto& type : groud.second)
					{
						// col���C���f�b�N�X����C���f�b�N�X+��؂鐔�͈͓̔���
						if (type.first <= col && type.first + END_INDEX > col)
						{
							type.second.lock()->AddMatrix(matrix);
						}
					}
				}
			}
		}
	}
	
	// �n�ʕ`��̏�����
	void GroundManager::ClearInstanceGround()
	{
		// �S����Instance�`��s����폜
		for (auto& groud : m_groundMap)
		{
			for (auto& type : groud.second)
			{
				type.second.lock()->ClearMatrix();
			}
		}
	}
}