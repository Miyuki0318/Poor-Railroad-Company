/*!
@file Crossing.h
@brief ���؂Ɠ��؊Ǘ��N���X
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief ���H�ɐݒu���铥��
	*/
	class Crossing : public TemplateObject
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g
		const string m_csvLine;	// csv��row��col��LINE

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Crossing(const shared_ptr<Stage>& stagePtr,
			const string& csvLine
		) :
			TemplateObject(stagePtr),
			m_csvLine(csvLine)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Crossing() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���f���ƃg�����X�t�H�[���̍����s��ݒ�֐�
		@param �ݒ肷��s��
		*/
		void SetModelMatrix(const Mat4x4& matrix)
		{
			if (!m_ptrDraw) return;

			m_ptrDraw->SetMeshToTransformMatrix(matrix);
		}

		/*!
		@brief ���؂��J������Ԃ̃A�j���[�V�����ɂ���֐�
		*/
		void SetOpen()
		{
			if (!m_ptrDraw) return;
			if (m_ptrDraw->GetCurrentAnimation() == L"OPEN") return;

			m_ptrDraw->ChangeCurrentAnimation(L"OPEN");
		}

		/*!
		@brief ���؂�������Ԃ̃A�j���[�V�����ɂ���֐�
		*/
		void SetCross()
		{
			if (!m_ptrDraw) return;
			if (m_ptrDraw->GetCurrentAnimation() == L"CROSS") return;

			m_ptrDraw->ChangeCurrentAnimation(L"CROSS");
		}
	};

	/*!
	@brief ���؊Ǘ��I�u�W�F�N�g
	*/
	class CrossingManager : public GameObject
	{
		// ���؃I�u�W�F�N�g�z��
		map<string, weak_ptr<Crossing>> m_crossingMap;

		// ��Ԃ̃|�C���^
		weak_ptr<TemplateObject> m_trainPtr;

		const Mat4x4 m_modelMat;	// ���f���p�̍s��
		const float m_updateRange;	// �X�V�͈�(��Ԃ���̑��΋���)
		const float m_openRange;	// �J�͈�

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		CrossingManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_updateRange(15.0f),
			m_openRange(2.0f),
			m_modelMat((Mat4x4)XMMatrixAffineTransformation(
				Vec3(0.25f),
				Vec3(0.0f),
				(Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f)),
				Vec3(0.0f, 0.05f, 0.0f)
			))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CrossingManager() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���ؒǉ��֐�
		@param �I���|�C���g
		*/
		void AddCrossing(const Point2D<size_t>& point);

		/*!
		@brief ���݂̃|�C���g�̈ʒu�����[���ɂȂ��Ă��邩�擾�֐�
		@param �I���|�C���g
		@return ��v���Ă���true
		*/
		bool GetIsRailPoint(const Point2D<size_t>& point) const;
	};
}