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
	using namespace Input;
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
	}

	// ���W�̍X�V����
	void SelectIndicator::UpdatePosition()
	{	
		Vec3 position;

		// �אڂ���ꏊ�ɔz�u
		UpdateCursolPosition(position);

		// �v���C���[�̍��W+�����x�N�g���ō��W�X�V
		SetPosition(position);
	}

	// �I�����Ă���|�C���g�̍X�V
	void SelectIndicator::UpdateSelectPoint()
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

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

	// �אړ_�ɃC���f�B�P�[�^�[���ړ�
	void SelectIndicator::UpdateCursolPosition(Vec3& position)
	{
		// �v���C���[�̉�]�pY���̒�����90�̔{���Ɉ�ԋ߂��̂ɐݒ�
		const auto& player = dynamic_pointer_cast<GamePlayer>(m_player.lock());

		// �v���C���[�̍��W���l�̌ܓ�����
		Vec3 pos = player->GetPosition();
		pos.x = round(pos.x);
		pos.z = round(pos.z);
		pos.y = m_position.y;

		// RB�{�^�����͂��������
		if (!GetButtonRB())
		{
			// �R���g���[���[����
			Vec3 cntlVec = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);
			if (cntlVec.length() > 0.0f)
			{
				m_cursolPosition += cntlVec * DELTA_TIME * 4.0f;
				m_cursolPosition.clamp(Vec3(-1.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 1.0f));
				if (cntlVec.x <= 0.1f && cntlVec.x >= -0.1f) m_cursolPosition.x = 0.0f;
				if (cntlVec.z <= 0.1f && cntlVec.z >= -0.1f) m_cursolPosition.z = 0.0f;
			}
		}

		Vec3 cursol = m_cursolPosition;
		cursol.x = round(cursol.x);
		cursol.z = round(cursol.z);

		// ���W���㏑��
		position = pos + cursol;
	}

	// �̎�I�u�W�F�N�g��I�����Ă��邩
	bool SelectIndicator::IsGatheringPoint() const
	{
		// �̎�I�u�W�F�N�g�}�l�[�W���[�̎擾
		const auto& gatheringManager = GetStage()->GetSharedGameObject<GatheringManager>(L"GatheringManager", false);
		if (!gatheringManager) return false;

		return gatheringManager->GetIsGatheringPoint(m_selectPoint);
	}

	// �K�C�h���[����I�����Ă��邩
	bool SelectIndicator::IsGuideRailPoint() const
	{
		// ���[���}�l�[�W���[�̎擾
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager", false);
		if (!railManager) return false;

		return railManager->GetIsGuidePoint(m_selectPoint);
	}

	// �����I�����Ă��邩
	bool SelectIndicator::IsWaterPoint() const
	{
		// �؂̑���}�l�[�W���[���擾
		const auto& bridgeManager = GetStage()->GetSharedGameObject<BridgeManager>(L"BridgeManager", false);
		if (!bridgeManager) return false;

		return bridgeManager->GetIsWaterPoint(m_selectPoint);
	}

	// �������[����I�����Ă��邩
	bool SelectIndicator::IsStraightRailPoint() const
	{
		// �I���|�C���g�����؂�ݒu�ł��郌�[���̈ʒu�ƈ�v���Ă��邩
		const auto& crossingManager = GetStage()->GetSharedGameObject<CrossingManager>(L"CrossingManager", false);
		if (!crossingManager) return false;

		return crossingManager->GetIsRailPoint(m_selectPoint);
	}

	// �̎施��
	int SelectIndicator::GatheringOrder() const
	{
		// �̎�I�u�W�F�N�g�}�l�[�W���[�̎擾
		const auto& gatheringManager = GetStage()->GetSharedGameObject<GatheringManager>(L"GatheringManager", false);
		if (!gatheringManager) return 0;

		// �I���|�C���g���K�C�h�̈ʒu�ƈ�v���Ă��邩
		int id = 0;
		if (gatheringManager->GetIsGatheringPoint(m_selectPoint))
		{
			// ��v���Ă���}�l�[�W���[�Ƀ��[���ǉ������𑗂�
			id = gatheringManager->Gathering(m_selectPoint);
		}

		return id;
	}

	// ���[���ݒu����
	bool SelectIndicator::RailedOrder() const
	{
		// ���[���}�l�[�W���[�̎擾
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager", false);
		if (!railManager) return false;

		// �I���|�C���g���K�C�h�̈ʒu�ƈ�v���Ă��邩
		bool posshible = railManager->GetIsGuidePoint(m_selectPoint);
		if (posshible)
		{
			// ��v���Ă���}�l�[�W���[�Ƀ��[���ǉ������𑗂�
			railManager->AddRail(m_selectPoint);
		}

		return posshible;
	}

	// ���[���ݒu����
	bool SelectIndicator::BridgeOrder() const
	{
		// �I���|�C���g���K�C�h�̈ʒu�ƈ�v���Ă��邩
		const auto& bridgeManager = GetStage()->GetSharedGameObject<BridgeManager>(L"BridgeManager", false);
		if (!bridgeManager) return false;

		bool posshible = bridgeManager->GetIsWaterPoint(m_selectPoint);

		// ��v���Ă���}�l�[�W���[�Ƀ��[���ǉ������𑗂�
		if (posshible)
		{
			bridgeManager->AddBridge(m_selectPoint);
		}

		return posshible;
	}

	// ���ؐݒu����
	bool SelectIndicator::CrossingOrder() const
	{
		// �I���|�C���g���K�C�h�̈ʒu�ƈ�v���Ă��邩
		const auto& crossingManager = GetStage()->GetSharedGameObject<CrossingManager>(L"CrossingManager", false);
		if (!crossingManager) return false;

		bool posshible = crossingManager->GetIsRailPoint(m_selectPoint);

		// ��v���Ă���}�l�[�W���[�ɓ��ؒǉ������𑗂�
		if (posshible)
		{
			crossingManager->AddCrossing(m_selectPoint);
		}

		return posshible;
	}
}