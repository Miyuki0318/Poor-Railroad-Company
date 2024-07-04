/*!
@file GatherEffect.cpp
@brief ����@�\
@author ���V���M
*/

#include "stdafx.h"
#include "OzawaUtility.h"
#include "GatherTreasure.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void GatherEffect::OnCreate()
	{
		// �p�����̐������̏����𑗂�
		Billboard::OnCreate();

		// ��A�N�e�B�u��
		SetDrawActive(false);
		SetUpdateActive(false);
	}

	// ���t���[���X�V����
	void GatherEffect::OnUpdate()
	{
		// �T�C���J�[�u���Ɏg�����W�A��
		float rad = m_totalTime * XM_2PI;

		// �J�������擾
		const auto& camera = GetStage()->GetView()->GetTargetCamera();

		// �g�����X�t�H�[���̌v�Z
		Vec3 pos = SinCurve(rad, m_startPos, m_startPos + m_boundVal);
		Quat rot = GetBillboardRotateQuat(camera->GetAt() - camera->GetEye(), Vec3(0.0f, rad, 0.0f));
		Vec2 scale = Utility::Lerp(Vec2(0.0f), m_drawScale, m_totalTime / m_effectTime);

		// �o�ߎ��Ԃ����Z
		m_totalTime += DELTA_TIME;

		// �`�掞�Ԉȏ�ɂȂ������A�N�e�B�u��
		if (m_totalTime >= m_effectTime)
		{
			SetDrawActive(false);
			SetUpdateActive(false);
		}

		// �g�����X�t�H�[���̍X�V
		SetScale(scale);
		SetPosition(pos);
		SetQuaternion(rot);

		// �F�̍X�V����
		SetDiffuseColor(Col4(1.0f, 1.0f, 1.0f, m_totalTime / m_effectTime));
	}

	// �J�n����
	void GatherEffect::StartDraw(const Vec3& position)
	{
		// ���W��ݒ�
		SetPosition(position);
		m_startPos = position;
		
		// �o�ߎ��Ԃ�������
		m_totalTime = 0.0f;

		// �A�N�e�B�u��
		SetDrawActive(true);
		SetUpdateActive(true);

		// SE�̍Đ�
		StartSE(L"C_SUCCES_SE", 1.0f);
	}

	// �󔠂��o�邩�A�o����A�C�e���f�[�^��Ԃ�
	pair<eItemType, int> GatherTreasure::TreasureCheck(const Vec3& position)
	{
		// 1����̊m����݂���
		int random = rand() % 10;
		if (m_probability > random) return make_pair(eItemType::Rail, 0);

		// �A�C�e���^�C�v�Ɖ��Z����ݒ�
		int max = sizeof(m_items) / sizeof(eItemType);
		eItemType addType = m_items[rand() % max];
		int addVal = m_addVal.at(addType);
		auto ret = make_pair(addType, addVal);

		// ���W��ݒ�
		Vec3 effectPos = position + UP_VEC;

		// �X�v���C�g�z��ɔ�A�N�e�B�u������Ȃ炻�������
		for (auto& weakPtr : m_effectVec)
		{
			// �G���[�`�F�b�N
			auto& sprite = weakPtr.lock();
			if (!sprite) continue;

			// ��A�N�e�B�u�Ȃ�J�n�����𑗂�I��
			if (!sprite->GetUpdateActive())
			{
				sprite->StartDraw(effectPos);
				return ret;
			}
		}

		// ��A�N�e�B�u�����������̂Ő������ĊJ�n�����𑗂�
		auto& newSprite = GetStage()->AddGameObject<GatherEffect>();
		newSprite->StartDraw(effectPos);

		// �z��ɒǉ�
		m_effectVec.push_back(newSprite);
		return ret;
	}
}