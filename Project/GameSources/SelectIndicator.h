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
		shared_ptr<PCStaticDraw> m_ptrDraw; // �`��R���|�[�l���g
		weak_ptr<TemplateObject> m_player; // �v���C���[
		Point2D<size_t> m_selectPoint; // �I�����Ă���|�C���g
		Vec3 m_cursolPosition; // �J�[�\���̍��W

		const float m_deadZone; // ����臒l

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
			m_player(playerPtr),
			m_deadZone(0.1f)
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
		@brief �Z���N�^�[�̈ʒu�ɍ̎�\�I�u�W�F�N�g���邩�m�F����֐�
		@return ���݂���Ȃ�true
		*/
		bool IsGatheringPoint() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�ɃK�C�h���[�����邩�m�F����֐�
		@return ���݂���Ȃ�true
		*/
		bool IsGuideRailPoint() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�����ꂩ�ǂ����m�F����֐�
		@return ����Ȃ�true
		*/
		bool IsWaterPoint() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�ɒ������[�������邩�m�F����֐�
		@return ���݂���Ȃ�true
		*/
		bool IsStraightRailPoint() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�ɍ̎�\�I�u�W�F�N�g����Ȃ�̎揈�����s���֐�
		@return �̎�ł��Ă�����true
		*/
		int GatheringOrder() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�Ƀ��[����ݒu�\�Ȃ�ݒu�������s���֐�
		@return �ݒu�ł��Ă�����true
		*/
		bool RailedOrder() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�Ƀ��[����ݒu�\�Ȃ�ݒu�������s���֐�
		@return �ݒu�ł��Ă�����true
		*/
		bool BridgeOrder() const;

		/*!
		@brief �Z���N�^�[�̈ʒu�ɓ��؂�ݒu�\�Ȃ�ݒu�������s���֐�
		@return �ݒu�ł��Ă�����true
		*/
		bool CrossingOrder() const;

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
		@brief �v���C���[�ɗאڂ���ꏊ�ɍ��W��ݒ肷��֐�
		@param ���݂̍��W
		*/
		void UpdateCursolPosition(Vec3& position);

		/*!
		@brief �I���|�C���g�̍X�V�����֐�
		*/
		void UpdateSelectPoint();
	};
}