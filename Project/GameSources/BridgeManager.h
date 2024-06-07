/*!
@file BridgeManager.h
@brief �؂̑���Ǘ��N���X
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief �؂̑���Ǘ��p�N���X
	*/
	class BridgeManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // �`��R���|�[�l���g

		const Mat4x4 m_mtxScale;	// �s��v�Z�Ɏg���X�P�[��
		const Mat4x4 m_mtxRotation;	// �s��v�Z�Ɏg�����[�e�[�V����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		BridgeManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_mtxScale((Mat4x4)XMMatrixScalingFromVector(Vec3(0.35f))),
			m_mtxRotation((Mat4x4)XMMatrixRotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f)))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~BridgeManager() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �������֐�
		*/
		void ResetBridge();

		/*!
		@brief ���[���ǉ��֐�
		@param �I���|�C���g
		*/
		void AddBridge(const Point2D<size_t>& point);

		/*!
		@brief ���݂̃|�C���g�̈ʒu������ɂȂ��Ă��邩�擾�֐�
		@param �I���|�C���g
		@return ��v���Ă���true
		*/
		bool GetIsWaterPoint(const Point2D<size_t>& point) const;
	};
}