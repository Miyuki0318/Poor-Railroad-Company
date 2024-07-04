/*!
@file GoalGuide.cpp
@brief �S�[���w������\������K�C�hUI
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void GoalGuide::OnCreate()
	{
		// �p�����̐������̏���
		Sprite::OnCreate();

		// �X�v���C�g�̐����ƃS�[���w�̎擾
		const auto& stagePtr = GetStage();
		m_arrowSprite = stagePtr->AddGameObject<Sprite>(L"GOAL_ARROW_TX", Vec2(m_scale));
		m_goalStation = stagePtr->GetSharedGameObject<TemplateObject>(L"Station");

		// ���C���[�ݒ�
		m_arrowSprite.lock()->SetDrawLayer(1);
		m_goalStation.lock()->SetDrawLayer(1);
	}

	// ���t���[���X�V
	void GoalGuide::OnUpdate()
	{
		// �r���[���擾���A2D���W�ϊ����s��
		const auto& view = GetStage()->GetView();
		Vec3 stationPos = ConvertToWorldPosition(view, m_goalStation.lock()->GetPosition());
		stationPos.z = 0.0f;

		// ��ʓ��ɃN���b�s���O����
		Vec3 pos = WindowClipLineVec(Vec3(0.0f), stationPos);
		pos.z = 0.8f;

		// �E�B���h�E�T�C�Y����X�P�[���������������ŃN�����v����
		Vec3 windowMax = Vec3((WINDOW_SIZE / 2.0f) - Vec2(m_scale / 2.0f), 1.0f);
		pos.clamp(-windowMax, windowMax);		

		// ���̊p�x�����߂�
		float rad = atan2f(stationPos.y - pos.y, stationPos.x - pos.x);

		// ���W�̍X�V�Ȃ�
		SetPosition(pos);
		m_arrowSprite.lock()->SetPosition(pos + BACK_LAYER);
		m_arrowSprite.lock()->SetRotation(Vec3(0.0f, 0.0f, rad));

		// �S�[���w����ʓ��Ȃ��\��
		bool isDraw = !GetBetween(stationPos, -windowMax, windowMax);
		SetDrawActive(isDraw);
		m_arrowSprite.lock()->SetDrawActive(isDraw);
	}
}