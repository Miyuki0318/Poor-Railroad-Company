/*!
@file ItemFly.cpp
@brief �A�C�e���ǉ�����UI�ɔ��ł����G�t�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void FlySprite::OnCreate()
	{
		// �p�����̐������̏����𑗂�
		Sprite::OnCreate();

		// ��A�N�e�B�u��
		SetDrawActive(false);
		SetUpdateActive(false);
	}

	// �X�V����
	void FlySprite::OnUpdate()
	{
		// �Ȑ���Ԃō��W�����߂�
		Vec3 pos = BezierLerp(m_targetPointStart, m_targetPointMiddle, m_targetPointEnd, m_totalTime / m_flyTime);
		m_totalTime += DELTA_TIME;
		m_totalTime = min(m_totalTime, m_flyTime);

		// �o�ߎ��Ԃ����Ď��Ԉȏ�ɂȂ�����
		if (m_totalTime >= m_flyTime)
		{
			// ���W���I�����W�ɂ��A��A�N�e�B�u��
			pos = m_targetPointEnd;
			SetDrawActive(false);
			SetUpdateActive(false);
		}
		
		// ���W�̍X�V
		SetPosition(pos);
		SetRotation(Vec3(0.0f, 0.0f, m_totalTime));
	}

	// �J�n�ݒ�
	void FlySprite::StartDraw(const wstring& texWstr, const Vec3& start, const Vec3& middle, const Vec3& end)
	{
		// �����ݒ�
		SetTexture(texWstr); // �e�N�X�`����ݒ�
		m_targetPointStart = start; // �J�n���W
		m_targetPointMiddle = middle; // ���ԍ��W
		m_targetPointEnd = end; // �I�����W
		m_totalTime = 0.0f; // �o�ߎ��Ԃ�0��

		// �A�N�e�B�u��
		SetDrawActive(true);
		SetUpdateActive(true);
	}

	// �J�n�Ăяo��
	void FlyItemManager::StartFly(eItemType itemType)
	{
		// �G���[�`�F�b�N
		if (m_itemUIPosMap.find(itemType) == m_itemUIPosMap.end()) return;
		
		// �A�C�e���^�C�v�ƃX�e�[�W�̎擾
		const auto& item = m_itemUIPosMap.at(itemType);
		const auto& stagePtr = GetStage();
		
		// �v���C���[�̍��W��2D���W�ɕϊ�
		Vec3 playerPos = ConvertToWorldPosition(stagePtr->GetView(), m_playerPtr.lock()->GetPosition());
		playerPos.z = 0.0f;

		// ���ԍ��W��ݒ�
		Vec3 middlePos = Vec3(playerPos.x, item.first.y, 0.0f);

		// �X�v���C�g�z��ɔ�A�N�e�B�u������Ȃ炻�������
		for (auto& weakPtr : m_spriteVec)
		{
			// �G���[�`�F�b�N
			auto& sprite = weakPtr.lock();
			if (!sprite) continue;

			// ��A�N�e�B�u�Ȃ�J�n�����𑗂�I��
			if (!sprite->GetUpdateActive())
			{
				sprite->StartDraw(item.second, playerPos, middlePos, item.first);
				return;
			}
		}

		// ��A�N�e�B�u�����������̂Ő������ĊJ�n�����𑗂�
		auto& newSprite = stagePtr->AddGameObject<FlySprite>();
		newSprite->StartDraw(item.second, playerPos, middlePos, item.first);
		
		// �z��ɒǉ�
		m_spriteVec.push_back(newSprite);
	}
}