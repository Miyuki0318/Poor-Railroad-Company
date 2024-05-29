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

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		BridgeManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
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