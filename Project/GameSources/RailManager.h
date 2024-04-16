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
		vector<Vec3> m_railPositions; // ���[���̍��W�z��
		Mat4x4 m_mtxScale;		// �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotation;	// �C���X�^���X�`��p�̃��[�e�[�V����
		size_t m_railNum;
		map<size_t, Vec3> m_railMap;
		
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_railNum = 0;

			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(0.675f));
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
		@brief �K�C�h���Čv�Z����֐�
		*/
		void GuideRecalculation();

		/*!
		@brief �K�C�h�̃|�C���g�擾�֐�
		@return m_guidePoints
		*/
		const vector<Point2D<size_t>>& GetGuidePoints() const
		{
			return m_guidePoints;
		}

		/*!
		@brief ���[���̍��W�z��擾�֐�
		@return m_railPositions
		*/
		const map<size_t, Vec3>& GetRailMap() const
		{
			return m_railMap;
		}

		/*!
		@brief �K�C�h��csv�擾�֐�
		@return m_guideMap
		*/
		const vector<vector<int>>& GetGuideMap() const
		{
			return m_guideMap;
		}

	private:

		/*!
		@brief �C���X�^���X�`��̃��[���ǉ��֐�
		@param row
		@param col
		*/
		void AddInstanceRail(size_t row, size_t col)
		{
			// ���W�̐ݒ�
			Vec3 addPos = Vec3(float(col), 1.0f, -float(row));

			// �g�����X�t�H�[���s��̐ݒ�
			Mat4x4 matrix, mtxPosition;
			mtxPosition.translation(addPos);

			// �s��̐ݒ�ƒǉ�
			matrix = m_mtxScale * m_mtxRotation * mtxPosition;
			m_ptrDraw->AddMatrix(matrix);
			m_railPositions.push_back(addPos);
			m_railMap.insert(make_pair(++m_railNum, addPos));
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