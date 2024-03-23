/*!
@file CraftWindow.cpp
@brief �N���t�g�E�B���h�E
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void CraftWindow::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// ��\����
		SetDrawActive(false);
	}

	// ���t���[���X�V����
	void CraftWindow::OnUpdate()
	{
		// �O��܂ł̏�Ԃƈ������E�B���h�E�̍X�V
		if (m_enable(eWindowEnable::IsEnable) != m_enable(eWindowEnable::IsPastEnable))
		{
			UpdateWindow();
		}
	}

	// �E�B���h�E�̍X�V
	void CraftWindow::UpdateWindow()
	{
		// �\����Ԃ�ϐ���
		const bool& isEnable = m_enable(eWindowEnable::IsEnable);

		// �W�J������ǉ�
		m_showRatio += (isEnable ? DELTA_TIME : -DELTA_TIME) * m_showSpeed;
		m_showRatio = min(m_showRatio, 1.0f);
		m_showRatio = max(m_showRatio, 0.0f);

		// �X�P�[����Lerp�Őݒ�
		Vec2 scale = Utility::Lerp(Vec2(0.0f), m_showScale, m_showRatio);
		SetScale(scale);

		// �n�_or�I�_�ɂȂ����̂Ȃ��Ԃ𑵂���
		if (m_showRatio == 1.0f || m_showRatio == 0.0f)
		{
			SetDrawActive(isEnable);
			m_enable.Set(eWindowEnable::IsPastEnable) = isEnable;
		}
	}

	// �\���؂�ւ�
	void CraftWindow::SetDrawEnable(bool b)
	{
		m_enable.Set(eWindowEnable::IsEnable) = b;
	}

	// �ʒu�ݒ�
	void CraftWindow::SetWindowRect(eWindowRect rect)
	{
		// ���_�f�[�^�̃��Z�b�g
		Utility::SimpleVertices(m_vertex.vertices);

		// ���Z�ʗp�ϐ�
		const float HELF = 0.5f;
		Vec3 addPos;

		// �����ɉ����Đݒ�
		switch (rect)
		{
		case eWindowRect::UpperRight: // �E��Ȃ�
			addPos = Vec2(HELF, HELF);
			break;

		case eWindowRect::UpperLeft: // ����Ȃ�
			addPos = Vec2(-HELF, HELF);
			break;

		case eWindowRect::UnderRight: // �E���Ȃ�
			addPos = Vec2(HELF, -HELF);
			break;
		
		case eWindowRect::UnderLeft: // �����Ȃ�
			addPos = Vec2(-HELF, -HELF);
			break;
		
		default:
			break;
		}

		// ���_�f�[�^�̍��W�����Z
		for (auto& v : m_vertex.vertices)
		{
			v.position += addPos;
		}

		// ���_�f�[�^�̍X�V
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}
}