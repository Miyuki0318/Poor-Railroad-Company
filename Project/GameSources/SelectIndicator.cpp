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
		// �v���C���[�̃|�C���^������΁A���W�̍X�V���s��
		if (m_player.lock()) UpdatePosition();

		// �I�����Ă���|�C���g(csv��ł�)���X�V
		UpdateSelectPoint();

		// �f�o�b�N�p������
		wstring str;
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		eStageID id = static_cast<eStageID>(stageMap.at(m_selectPoint.x).at(m_selectPoint.y));
		switch (id)
		{
		case eStageID::Rail:
		case eStageID::DeRail:
			str += L"���[��";
			break;

		case eStageID::GoalRail:
			str += L"�S�[��";
			break;

		case eStageID::Stone:
			str += L"��";
			break;

		case eStageID::Tree:
			str += L"��";
			break;

		default:
			str += L"��";
			break;
		}

		Debug::Log(str + L"��I��");
	}

	// ���W�̍X�V����
	void SelectIndicator::UpdatePosition()
	{
		// �v���C���[�̉�]�pY���̒�����90�̔{���Ɉ�ԋ߂��̂ɐݒ�
		const auto& player = dynamic_pointer_cast<GamePlayer>(m_player.lock());
		float rotY = DegToRad(GetClosest(RadToDeg(player->GetPastRotTarget()), m_rotArray));

		// �����x�N�g�����`(�����_�ȉ��l�̌ܓ�)
		Vec3 velo = Vec3(cosf(rotY), 0.0f, -sinf(rotY));
		velo.x = round(velo.x);
		velo.z = round(velo.z);

		// �v���C���[�̍��W���l�̌ܓ�����
		Vec3 pos = player->GetPosition();
		pos.x = round(pos.x);
		pos.z = round(pos.z);
		pos.y = m_position.y;

		// �v���C���[�̍��W+�����x�N�g���ō��W�X�V
		SetPosition(pos + velo);
	}

	// �I�����Ă���|�C���g�̍X�V
	void SelectIndicator::UpdateSelectPoint()
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// �T�C�Y�Ɨ�ƍs
		Vec3 pos = GetPosition();
		size_t row, col;
		row = ROW(pos.z);
		col = COL(pos.x);

		// �z��͈͓̔����̃G���[�`�F�b�N
		if (!WithInElemRange(row, col, stageMap))
		{
			return;
		}

		// �X�V
		m_selectPoint = Point2D<size_t>(row, col);
	}

	// �̌@����
	set<wstring> SelectIndicator::MiningOrder() const
	{
		// �̌@�\�I�u�W�F�N�g�z��̎擾
		const auto& miningVec = GetStage()->GetSharedObjectGroup(L"MiningObject")->GetGroupVector();

		// �z��̐����[�v
		for (const auto& weakObj : miningVec)
		{
			// �G���[�`�F�b�N
			if (!weakObj.lock()) continue;
			if (!weakObj.lock()->IsUpdateActive()) continue;

			// �^�L���X�g
			const auto& miningObj = dynamic_pointer_cast<MiningObject>(weakObj.lock());
			if (!miningObj) continue;

			// CSV��̍��W����v������̌@�����𑗂�A�^�O�Z�b�g��Ԃ�
			if (m_selectPoint == miningObj->GetCSVPos())
			{
				miningObj->OnMining();
				return miningObj->GetTagSet();
			}
		}

		// �Ώۂ̃I�u�W�F�N�g�����������̂�null�̃^�O�Z�b�g��Ԃ�
		set<wstring> null;
		return null;
	}

	// ���[���ݒu����
	bool SelectIndicator::RailedOrder() const
	{
		// �I���|�C���g���K�C�h�̈ʒu�ƈ�v���Ă��邩
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager");
		bool posshible = railManager->GetIsGuidePoint(m_selectPoint);

		// ��v���Ă���}�l�[�W���[�Ƀ��[���ǉ������𑗂�
		if (posshible)
		{
			railManager->AddRail(m_selectPoint);
		}

		return posshible;
	}
}