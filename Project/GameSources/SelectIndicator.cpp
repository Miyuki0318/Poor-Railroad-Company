/*!
@file SelectIndicator.cpp
@brief �Z���N�g�C���f�B�P�[�^�[
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void SelectIndicator::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();

		// ���_�f�[�^�̐ݒ�
		const float HELF = 0.55f;
		vector<Vec3> point = {
			Vec3(HELF, 0.0f, HELF),
			Vec3(HELF, 0.0f, -HELF),
			Vec3(-HELF, 0.0f, -HELF),
			Vec3(-HELF, 0.0f, HELF),
			Vec3(HELF, 0.0f, HELF),
		};

		// ���{���^�̒��_�f�[�^���쐬
		RibonVerticesIndices(point, m_vertex, Vec3(0.0f, 0.0f, 0.5f), HELF, 1);

		// ���_�f�[�^�̐����[�v
		for (auto& vertices : m_vertex.vertices)
		{
			// ���WY�����㑤�ł���΁A�����F�ɂ���
			if (vertices.position.y > 0.0f)
			{
				vertices.color = COL_ALPHA;
			}
		}

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PCTStaticDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);

		// �����F�̕`����\��
		SetAlphaActive(true);
	}

	// ���t���[���X�V����
	void SelectIndicator::OnUpdate()
	{
		// �v���C���[�̃|�C���^�����邩
		if (m_player.lock())
		{
			const auto& player = m_player.lock();

			// �v���C���[�̉�]�pY���̒�����90�̔{���Ɉ�ԋ߂��̂ɐݒ�
			float rotY = GetClosest(player->GetRotation().y, 0.0f, XM_PIDIV2, XM_PI, -XM_PIDIV2, -XM_PI);
			
			// �����x�N�g�����`
			Vec3 velo = Vec3(cosf(rotY), 0.0f, -sinf(rotY));
			velo.x = round(velo.x);
			velo.z = round(velo.z);

			// �v���C���[�̍��W���l�̌ܓ�����
			Vec3 pos = player->GetPosition();
			pos.x = round(pos.x);
			pos.z = round(pos.z);
			pos.y = 1.5f;

			// �v���C���[�̍��W+�����x�N�g���ō��W�X�V
			SetPosition(pos + velo);
		}
	}

	// �̌@�\�I�u�W�F�N�g���A�̌@�\�I�u�W�F�N�g�|�C���^�̎擾
	shared_ptr<TemplateObject> SelectIndicator::GetMiningPossible() const
	{
		// ���g�̍��W���擾
		Vec3 indiPos = GetPosition();

		// �̌@�\�I�u�W�F�N�g�z��̎擾
		const auto& miningVec = GetStage()->GetSharedObjectGroup(L"Mining")->GetGroupVector();

		// �z��̐����[�v
		for (const auto& weakObj : miningVec)
		{
			// �G���[�`�F�b�N
			if (!weakObj.lock()) continue;
			if (!weakObj.lock()->IsUpdateActive()) continue;

			// �^�L���X�g
			const auto& miningObj = dynamic_pointer_cast<TemplateObject>(weakObj.lock());
			if (!miningObj) continue;

			// ���W����v������|�C���^��Ԃ�
			if (indiPos == miningObj->GetPosition())
			{
				return miningObj;
			}
		}

		// �\�I�u�W�F�N�g�����������̂�nullptr��Ԃ�
		return nullptr;
	}

	// ���[���ݒu�ł��邩�A�ł��Ȃ��ꍇ�͒u����Ă��郌�[�����擾
	bool SelectIndicator::GetRailedPossible(const Vec3& checkPos) const
	{
		bool achiev = false; // �ݒu�ł��邩�̐^�U

		// �̌@�\�I�u�W�F�N�g�z��̎擾
		const auto& railsVec = GetStage()->GetSharedObjectGroup(L"Rails")->GetGroupVector();

		// �z��̐����[�v
		for (const auto& weakObj : railsVec)
		{
			// �G���[�`�F�b�N
			if (!weakObj.lock()) continue;
			if (!weakObj.lock()->IsUpdateActive()) continue;

			// �^�L���X�g
			const auto& railObj = dynamic_pointer_cast<TemplateObject>(weakObj.lock());
			if (!railObj) continue;
		
			// ���W�̎擾
			Vec3 railPos = railObj->GetPosition();

			// �������W�Ƀ��[��������Ȃ�ݒu�s��
			if (railPos == checkPos) return false;

			// ���[���̍��E�O��̍��W�ƈ�v���Ă���ݒu�\
			vector<Vec3> vec = { FRONT_VEC, BACK_VEC, LEFT_VEC, RIGHT_VEC };
			for (const auto& v : vec)
			{
				if (checkPos == railPos + v)
				{
					achiev = true;
				}
			}
		}

		// ��v���Ȃ�������ݒu�s��
		return achiev;
	}
}