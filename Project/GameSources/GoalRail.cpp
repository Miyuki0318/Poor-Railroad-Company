/*!
@file Rail.cpp
@brief �S�[���n�_�I�u�W�F�N�g�̎���
@author �����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void GoalRail::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_BLACK);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// �^�O�̐ݒ�
		AddTag(L"Rail");

		// Station�I�u�W�F�N�g�̐���
		const auto ptrTrans = GetComponent<Transform>();
		Vec3 pos = ptrTrans->GetPosition();
		GetStage()->AddGameObject<Station>(Vec3(pos.x, pos.y, pos.z + 1.0f));
	}

	void GoalRail::OnUpdate()
	{
		if (m_setFlg) {
			// ���g�̃R���W�������擾
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroup���擾
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup���m�̏Փ˔�����I�t
			ptrColl->SetExcludeCollisionGroup(group);
			// �t���O��false�ɕύX
			m_setFlg = false;
		}

		Debug::Log(L"�Q�[���̏�Ԃ�", GetTypeStage<GameStage>()->GetGameProgress());

	}

	void GoalRail::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		// Train�Ƃ̏Փ˂��`�F�b�N
		if (Other->FindTag(L"Train")) {
			// �S�[����̏����𑗂�
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameClear);
		}
	}
}