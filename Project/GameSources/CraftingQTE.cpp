/*!
@file CraftingQTE.cpp
@brief �N���t�g����QTE
@author ���V���M
*/

#include "stdafx.h"
#include "CraftingQTE.h"

namespace basecross
{
	// �������̏���
	void CraftingQTE::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �t���[����QTE�|�C���g�ƃ{�^�����͂𐶐�
		m_barFlame = stagePtr->AddGameObject<CraftUI>(L"C_QTE_FLAME_TX", m_drawScale, m_drawSpeed);
		m_qtePoint = stagePtr->AddGameObject<CraftUI>(L"WHITE_TX", Vec2(40.0f, 10.0f), m_drawSpeed);
		m_qteButton = stagePtr->AddGameObject<CraftUI>(L"BUTTON_A_TX", Vec2(50.0f), m_drawSpeed);
		
		// �F��ύX
		m_qtePoint.lock()->SetDiffuseColor(Col4(0.0f, 0.0f, 1.0f, 0.75f));
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
		CraftUI::SetEnable(enable);

		// �e��|�C���^
		auto& flame = m_barFlame.lock();
		auto& point = m_qtePoint.lock();
		auto& button = m_qteButton.lock();

		// �t���[����QTE�|�C���g�ɂ��\���ؑւ��̏����𑗂�
		flame->SetEnable(enable);
		point->SetEnable(enable);

		// ���W��ݒ�
		Vec3 diff = m_rectDiff.at(m_rectType);
		Vec3 pos = windowPos + Vec3(0.0f, (m_posDiff + m_drawScale.y) * diff.y, 0.0f);
		SetPosition(Vec3(pos.x, pos.y, 0.2f));
		flame->SetPosition(Vec3(pos.x, pos.y, 0.0f));
		point->SetPosition(Vec3(pos.x + (m_drawScale.x * (m_qteRatio - m_qteRange)) * diff.x, pos.y, 0.1f));
		button->SetPosition(point->GetPosition() + Vec3(point->m_drawScale.x / 2.0f * diff.x, button->m_drawScale.y / 1.25f * diff.y, 0.0f));
	}
	
	// �`��ύX�ݒ�
	void CraftingQTE::SetVerticesRect(eRectType rect)
	{
		// �p�����̕`��ύX�ݒ菈���𑗂�
		CraftUI::SetVerticesRect(rect);

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
		m_qteButton.lock()->SetEnable(true);
	}

	// QTE�̒�~����
	bool CraftingQTE::StopQTE()
	{
		// QTE���~
		m_qteEnable = false;
		m_qteButton.lock()->SetEnable(false);

		// Y���̃X�P�[����0�̏�Ԃɂ���
		SetScale(0.0f, m_scale.y);

		// QTE�͈̔͂�ݒ�
		const float upper = m_qteRatio + m_qteRange;
		const float under = m_qteRatio - m_qteRange;

		// �o�[�̊�����QTE�͈͓̔�����Ԃ�
		return Utility::GetBetween(m_barRatio, upper, under);
	}
}