/*!
@file CraftingQTE.cpp
@brief �N���t�g����QTE
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void CraftingQTE::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �t���[����QTE�|�C���g�𐶐�
		m_barFlame = stagePtr->AddGameObject<CraftUI>(L"C_QTE_FLAME_TX", m_drawScale, m_drawSpeed);
		m_qtePoint = stagePtr->AddGameObject<CraftUI>(L"WHITE_TX", Vec2(40.0f, 10.0f), m_drawSpeed);
		m_qtePoint.lock()->SetDiffuseColor(Col4(1.0f, 1.0f, 0.0f, 0.75f));
	}

	// ���t���[���X�V����
	void CraftingQTE::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		CraftUI::OnUpdate();

		// QTE���Ȃ�QTE�̍X�V���������s
		if (m_qteEnable)
		{
			UpdateQTE();
		}
	}

	// �E�B���h�E�̍X�V
	void CraftingQTE::DrawWindow()
	{
		// �p�����̃E�B���h�E�̍X�V���������s
		CraftUI::DrawWindow();

		// Y���̃X�P�[����0�̏�Ԃɂ���
		SetScale(0.0f, m_scale.y);
	}

	// �\���؂�ւ�
	void CraftingQTE::SetDrawEnable(bool enable, Vec3 windowPos)
	{
		// �p�����̕\���ؑւ����������s
		CraftUI::SetDrawEnable(enable);

		// �t���[����QTE�|�C���g�ɂ��\���ؑւ��̏����𑗂�
		m_barFlame.lock()->SetDrawEnable(enable);
		m_qtePoint.lock()->SetDrawEnable(enable);

		// ���W��ݒ�
		Vec3 pos = GetPosition() + Vec3(0.0f, m_posDiff + m_drawScale.y, 0.0f);
		SetPosition(Vec3(pos.x, pos.y, 0.2f));
		m_barFlame.lock()->SetPosition(Vec3(pos.x, pos.y, 0.0f));
		m_qtePoint.lock()->SetPosition(Vec3(pos.x + (m_drawScale.x * (m_qteRatio - 0.1f)), pos.y, 0.1f));
	}
	
	// �`��ύX�ݒ�
	void CraftingQTE::SetVerticesRect(eVerticesRect rect)
	{
		// �p�����̕`��ύX�ݒ菈���𑗂�
		Sprite::SetVerticesRect(rect);

		// �t���[����QTE�|�C���g�ɂ��`��ύX�ݒ�̏����𑗂�
		m_barFlame.lock()->SetVerticesRect(rect);
		m_qtePoint.lock()->SetVerticesRect(rect);
	}

	// QTE�̍X�V����
	void CraftingQTE::UpdateQTE()
	{
		// �������f���^�^�C���ƃo�[�̑��x�ŉ��Z
		m_barRatio += DELTA_TIME / m_barSpeed;
		m_barRatio = min(m_barRatio, 1.0f);
		m_barRatio = max(m_barRatio, 0.0f);

		// �X�P�[��X���������g����Lerp�ŋ��߂�
		float scaleX = Utility::Lerp(0.0f, m_drawScale.x, m_barRatio);
		SetScale(scaleX, m_drawScale.y);

		// �n�_or�I�_�ɂȂ����̂Ȃ�QTE���I��
		if (m_barRatio == 1.0f)
		{
			m_barRatio = 0.0f;
			m_qteEnable = false;
		}
	}

	// QTE�̊J�n����
	void CraftingQTE::StartQTE(const Vec3& windowPos)
	{
		// �J�n���̏��������s��
		m_barRatio = 0.0f;
		m_qteEnable = true;
	}

	// QTE�̒�~����
	bool CraftingQTE::StopQTE()
	{
		// QTE���~
		m_qteEnable = false;

		// Y���̃X�P�[����0�̏�Ԃɂ���
		SetScale(0.0f, m_scale.y);

		// QTE�͈̔͂�ݒ�
		const float upper = m_qteRatio + 0.1f;
		const float under = m_qteRatio - 0.1f;

		// �o�[�̊�����QTE�͈͓̔�����Ԃ�
		return Utility::GetBetween(m_barRatio, upper, under);
	}
}