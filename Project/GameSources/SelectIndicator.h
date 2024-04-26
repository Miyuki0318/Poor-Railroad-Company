/*!
@file SelectIndicator.h
@brief �Z���N�g�C���f�B�P�[�^�[
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �I�����`��
	*/
	class SelectIndicator : public TemplateObject
	{
		VertexData m_vertex; // ���_�f�[�^
		shared_ptr<PCTStaticDraw> m_ptrDraw; // �`��R���|�[�l���g
		weak_ptr<TemplateObject> m_player; // �v���C���[
		Point2D<size_t> m_selectPoint; // �I�����Ă���|�C���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�|�C���^
		*/
		SelectIndicator(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<TemplateObject>& playerPtr
		) :
			TemplateObject(stagePtr, Vec3(0.0f, 1.5f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_player(playerPtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~SelectIndicator() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �Z���N�^�[�̈ʒu�ɍ̌@�I�u�W�F�N�g������΍̌@�������s���֐�
		@return �̌@�\�I�u�W�F�N�g�^�O�Z�b�g��Ԃ�
		*/
		set<wstring> MiningOrder() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�Ƀ��[����ݒu�\�Ȃ�ݒu�������s���֐�
		@return �ݒu�ł��Ă�����true
		*/
		bool RailedOrder() const;

		/*!
		@brief �Z���N�^�[�̈ʒu���擾����֐�
		@return m_selectPoint
		*/
		const Point2D<size_t>& GetSelectPoint() const
		{
			return m_selectPoint;
		}

	private:

		/*!
		@brief ���W�̍X�V�����֐�
		*/
		void UpdatePosition();

		/*!
		@brief �I���|�C���g�̍X�V�����֐�
		*/
		void UpdateSelectPoint();
	};
}