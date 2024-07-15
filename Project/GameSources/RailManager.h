/*!
@file RailManager.h
@brief ���[���Ǘ��N���X
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "OzawaUtility.h"

#define ROW(posZ) size_t(-posZ)	// �s
#define COL(posX) size_t(posX) // ��
#define ROWCOL2LINE(row, col) to_string(row) + "-" + to_string(col)	// row-col
#define ROWCOL2POS(row, col) Vec3(float(col), 1.0f, -float(row))	// row��col������W�ɕϊ�
#define POS2LINE(pos) ROWCOL2LINE(ROW(pos.z), COL(pos.x))	// ���W����row-col������ɕϊ�

namespace basecross
{
	// CSV�`�F�b�N�p����enum
	enum class eNextElemDir
	{
		DirFlont,	// �O
		DirBack,	// ��
		DirLeft,	// ��
		DirRight,	// �E
	};

	// CSV�̃`�F�b�N�p�\����
	struct CSVElementCheck
	{
		eNextElemDir dir = eNextElemDir::DirFlont;
		size_t row = 0;	// �s
		size_t col = 0; // ��
		bool isRange = false; // �z��͈͓̔����̐^�U

		/*!
		@brief �R���X�g���N�^
		@param row
		@param col
		@param isRange
		*/
		CSVElementCheck(eNextElemDir railDir, size_t elemRow, size_t elemCol, bool range) :
			dir(railDir),
			row(elemRow),
			col(elemCol),
			isRange(range)
		{
		}

		/*!
		@brief �|�C���g�̑O�㍶�E���z��͈͓̔������擾����֐�
		@param row
		@param col
		@param csv�z��
		*/
		static vector<CSVElementCheck> GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap)
		{
			vector<CSVElementCheck> elems; // �O�㍶�E�̌��ʕۑ��p�z��
			if (csvMap.empty()) return elems; // �Q�Ƃ���z�񂪋�Ȃ���Ԃ�

			elems = {
				{eNextElemDir::DirFlont ,row - 1, col, Utility::WithInElemRange(row - 1, csvMap.size())},
				{eNextElemDir::DirBack ,row + 1, col, Utility::WithInElemRange(row + 1, csvMap.size())},
				{eNextElemDir::DirLeft ,row, col - 1, Utility::WithInElemRange(col - 1, csvMap.at(row).size())},
				{eNextElemDir::DirRight ,row, col + 1, Utility::WithInElemRange(col - 1, csvMap.at(row).size())},
			};

			return elems;
		}
	};

	// ���[���̃^�C�v
	enum class eRailType
	{
		AxisXLine,		// ���E�̒���
		AxisZLine,		// �㉺�̒���
		Right2Under,	// �E���牺��
		Right2Upper,	// �E������
		Left2Under,		// �����牺��
		Left2Upper,		// ��������
	};

	// ���[���̍s������
	enum class eRailAngle
	{
		Straight,	// ����
		Right,		// �E�ɋȂ���
		Left,		// ���ɋȂ���
	};

	// ���[���̏��\����
	struct RailData
	{
		Vec3 thisPos;		// ���g�̍��W
		Vec3 pastPos;		// ��O�̍��W
		Vec3 futurePos;		// ���̍��W
		eRailType type;		// ���[���̃^�C�v
		eRailAngle angle;	// ���[���̍s������

		RailData() 
		{
			thisPos.zero();
			pastPos.zero();
			type = eRailType::AxisXLine;
			angle = eRailAngle::Straight;
		}

		RailData(const Vec3& tPos, const Vec3& pPos) :
			thisPos(tPos),
			pastPos(pPos)
		{
			type = eRailType::AxisXLine;
			angle = eRailAngle::Straight;
		}
	};

	/*!
	@brief LINE�}�N���ŕێ����ꂽRowCol�������size_t�ɑ������֐�
	@param row�i�[�ϐ�
	@param col�i�[�ϐ�
	@param line������
	*/
	void GetLineStringToRowCol(size_t& row, size_t& col, string line);

	class InstanceRail : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // �`��R���|�[�l���g
		const wstring m_meshKey;

	public:

		InstanceRail(const shared_ptr<Stage>& stagePtr,
			const wstring& meshKey
		) :
			GameObject(stagePtr),
			m_meshKey(meshKey)
		{
		}

		virtual ~InstanceRail() {}

		void OnCreate() override;

		void AddRail(Mat4x4 matrix);

		void ResetRail();

		vector<Mat4x4>& GetMatrix();
	};

	/*!
	@brief ���[���Ǘ��p�N���X
	*/
	class RailManager : public GameObject
	{
		vector<Point2D<size_t>> m_guidePoints;	// �K�C�h������|�C���g
		vector<vector<int>> m_guideMap;			// �K�C�h�t���̃X�e�[�Wcsv
		map<string, RailData> m_railDataMap;	// ���[���f�[�^�}�b�v
		map<eRailType, float> m_railAngleMap;	// ���[���̃A���O���}�b�v

		// �C���X�^���X�`��̃��[��
		map<eRailAngle, weak_ptr<InstanceRail>> m_instanceRail;

		string m_pastLine;		 // �Ō�ɐݒu�������[����LINE
		size_t m_railNum;		 // ���[���̐ݒu��
		Vec3 m_startRailPos;	 // �ŏ��ɐݒu�����J�n���[��
		Vec3 m_pastDeRailPos;	 // �Ō�ɐݒu������[���[��
		bool m_isConnectionGoal; // �S�[�����[���܂Ōq��������
		
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_railNum = 0;
			m_isConnectionGoal = false;

			m_railAngleMap.emplace(eRailType::AxisXLine, 0.0f);
			m_railAngleMap.emplace(eRailType::AxisZLine, XM_PIDIV2);
			m_railAngleMap.emplace(eRailType::Left2Under, XM_PI);
			m_railAngleMap.emplace(eRailType::Left2Upper, -XM_PIDIV2);
			m_railAngleMap.emplace(eRailType::Right2Under, XM_PIDIV2);
			m_railAngleMap.emplace(eRailType::Right2Upper, 0.0f);
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
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���[���`�搶��
		*/
		void ResetInstanceRail();

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
		vector<Point2D<size_t>>& GetGuidePoints()
		{
			return m_guidePoints;
		}

		/*!
		@brief ���[���̍��W�z��擾�֐�
		@return m_railPositions
		*/
		const map<string, RailData>& GetRailDataMap() const
		{
			return m_railDataMap;
		}

		/*!
		@brief �K�C�h��csv�擾�֐�
		@return m_guideMap
		*/
		const vector<vector<int>>& GetGuideMap() const
		{
			return m_guideMap;
		}

		/*!
		@brief ���[���̐ݒu���擾�֐���
		@return m_railNum
		*/
		size_t GetAddRailNum() const
		{
			return m_railNum;
		}

		/*!
		@brief �J�n���[���̍��W�擾�֐�
		@return m_startRailPos
		*/
		Vec3 GetStartRailPos() const
		{
			return m_startRailPos;
		}

		/*!
		@brief �Ō�ɒu�������[���̍��W�擾�֐�
		@return m_pastDeRailPos
		*/
		Vec3 GetPastRailPos() const
		{
			return m_pastDeRailPos;
		}

		/*!
		@brief �S�[�����[���ƌq���������̊m�F�֐�
		@return bool
		*/
		bool IsConnectionGoalRail() const
		{
			return m_isConnectionGoal;
		}

	private:

		/*!
		@brief �C���X�^���X�`��̃��[���ǉ��֐�
		@param row
		@param col
		*/
		void AddInstanceRail(size_t row, size_t col, eRailAngle angle = eRailAngle::Straight);

		/*!
		@brief �C���X�^���X�`��̃J�[�u���[���ǉ��֐�
		@param �O��̃��[���f�[�^
		*/
		void AddInstanceCurveRail(RailData& pastData);

		/*!
		@brief ���[���f�[�^�ǉ��֐�
		@param row
		@param col
		*/
		void AddRailDataMap(size_t row, size_t col);

		/*!
		@brief �}�b�v�Ƀ��[��ID��ǉ�����֐�
		@param �I���|�C���g
		*/
		void SetMapID(const Point2D<size_t>& point);

		/*!
		@brief CSV�����[��ID�ɏ���������֐�
		@param row
		@param col
		*/
		void SetRailID(size_t row, size_t col) const;

		/*!
		@brief ���[���f�[�^�̊p�x��ݒ肷��֐�
		@param ���݂̃��[���f�[�^
		@param ��O�̃��[���f�[�^
		*/
		void SetPastRailDataAngle(RailData& current, RailData& past);

		/*!
		@brief ���[���f�[�^�̎�ނ�ݒ肷��֐�
		@param ���݂̃��[���f�[�^
		@param ��O�̃��[���f�[�^
		*/
		void SetPastRailDataType(RailData& current, RailData& past);

		/*!
		@brief CSV���K�C�hID�ɏ���������֐�
		@param row
		@param col
		*/
		void SetGuideID(size_t row, size_t col);

		/*!
		@brief CSV���K�C�hID�ɒǉ�����֐�
		@param �ǉ�����row
		@param �ǉ�����col
		*/
		void AddGuideID(size_t row, size_t col);

		/*!
		@brief ���[���������Ɍq�����Ă���̂���Ԃ��֐�
		@param ���[���f�[�^
		@param elem.dir
		*/
		bool CheckStraightRail(RailData data, eNextElemDir dir);

		/*!
		@brief �S�[�����[���ƌq���������̊m�F�֐�
		@param �ݒu�������[����row
		@param �ݒu�������[����col
		*/
		void CheckConnectionGoalRail(size_t row, size_t col);
	};
}