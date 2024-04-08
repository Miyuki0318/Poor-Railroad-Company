/*!
@file RailManager.h
@brief ���[���Ǘ��N���X
@author ���V���M
*/

#pragma once
#include "stdafx.h"

#define ROW(posZ) size_t(-posZ)	// �s
#define COL(posX) size_t(posX) // ��

namespace basecross
{
	// CSV�̃`�F�b�N�p�\����
	struct CSVElementCheck
	{
		size_t row = 0;	// �s
		size_t col = 0; // ��
		bool isRange = false; // �z��͈͓̔����̐^�U

		/*!
		@brief �R���X�g���N�^
		@param row
		@param col
		@param isRange
		*/
		CSVElementCheck(size_t elemRow, size_t elemCol, bool range) :
			row(elemRow),
			col(elemCol),
			isRange(range)
		{
		}
	};

	/*!
	@brief ���[���Ǘ��p�N���X
	*/
	class RailManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // �`��R���|�[�l���g
		vector<Point2D<size_t>> m_guidePoints; // �K�C�h������|�C���g
		vector<vector<int>> m_guideMap; // �K�C�h�t���̃X�e�[�Wcsv
		Mat4x4 m_mtxScale;		// �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotation;	// �C���X�^���X�`��p�̃��[�e�[�V����
		
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_mtxScale.scale(Vec3(1.0f, 0.2f, 1.0f));
			m_mtxRotation.rotation(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~RailManager() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���[���ǉ��֐�
		@param �I���|�C���g
		*/
		void AddRail(const Point2D<size_t>& point);

		/*!
		@brief ���݂̃|�C���g�̈ʒu���K�C�h�ɂȂ��Ă��邩�擾�֐�
		@param �I���|�C���g
		*/
		bool GetIsGuidePoint(const Point2D<size_t>& point) const;

		/*!
		@brief �K�C�h�̃|�C���g�擾�֐�
		@return m_guidePoints
		*/
		vector<Point2D<size_t>> GetGuidePoints() const
		{
			return m_guidePoints;
		}

	private:

		/*!
		@brief �C���X�^���X�`��̃��[���ǉ��֐�
		@param row
		@param col
		*/
		void AddInstanceRail(size_t row, size_t col)
		{
			// �g�����X�t�H�[���s��̐ݒ�
			Mat4x4 matrix, mtxPosition;
			mtxPosition.translation(Vec3(float(col), 1.0f, -float(row)));

			// �s��̐ݒ�ƒǉ�
			matrix = m_mtxScale * m_mtxRotation * mtxPosition;
			m_ptrDraw->AddMatrix(matrix);
		}

		/*!
		@brief CSV�����[��ID�ɏ���������֐�
		@param row
		@param col
		*/
		void SetRailID(size_t row, size_t col) const;

		/*!
		@brief CSV���K�C�hID�ɏ���������֐�
		@param row
		@param col
		*/
		void SetGuideID(size_t row, size_t col);

		/*!
		@brief �|�C���g�̑O�㍶�E���z��͈͓̔������擾����֐�
		@param row
		@param col
		@param csv�z��
		*/
		vector<CSVElementCheck> GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap) const;
	};
}