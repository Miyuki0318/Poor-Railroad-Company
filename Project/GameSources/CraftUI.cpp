/*!
@file CraftUI.cpp
@brief �N���t�gUI�̌p�����I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ���t���[���X�V����
	void CraftUI::OnUpdate()
	{
		// �O��܂ł̏�Ԃƈ������E�B���h�E�̍X�V
		if (m_drawEnable(eWindowEnable::IsEnable) != m_drawEnable(eWindowEnable::IsPastEnable))
		{
			DrawWindow();
		}
	}

	// �E�B���h�E�̍X�V
	void CraftUI::DrawWindow()
	{
		// �\����Ԃ�ϐ���
		const bool isEnable = m_drawEnable(eWindowEnable::IsEnable);

		// �W�J������ǉ�
		m_drawRatio += (isEnable ? DELTA_TIME : -DELTA_TIME) / m_drawSpeed;
		m_drawRatio = min(m_drawRatio, 1.0f);
		m_drawRatio = max(m_drawRatio, 0.0f);

		// �X�P�[����Lerp�Őݒ�
		Vec2 scale = Utility::Lerp(Vec2(0.0f), m_drawScale, m_drawRatio);
		SetScale(scale);

		// �n�_or�I�_�ɂȂ����̂Ȃ��Ԃ𑵂���
		if (m_drawRatio == 1.0f || m_drawRatio == 0.0f)
		{
			m_drawEnable.Set(eWindowEnable::IsPastEnable) = isEnable;
		}
	}

	// �\���؂�ւ�
	void CraftUI::SetDrawEnable(bool enable)
	{
		// �\����Ԃ�ݒ�
		m_drawEnable.Set(eWindowEnable::IsEnable) = enable;

		// �v���C���[�̓���̍��W�̈ʒu�Ɉړ�
		const auto& stagePtr = GetStage();
		const auto& player = stagePtr->GetSharedGameObject<Player>(L"Player");
		Vec3 point = Utility::ConvertToWorldPosition(stagePtr->GetView(), player->GetPosition());
		point.z = 0.0f;

		// ���W���X�V
		SetPosition(point);
	}
}