/*!
@file GatheringObject.h
@brief �̎�ł���I�u�W�F�N�g
@author �����
*/

#pragma once
#include "BaseStage.h"

namespace basecross
{
	/*!
	@brief �C���X�^���X�`�悷��̎�I�u�W�F�N�g
	*/
	class InstanceGathers : public GameObject
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		InstanceGathers(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~InstanceGathers() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���b�V���f�[�^��ݒ肷��֐�
		@param ���b�V���̃��\�[�X�L�[
		@param �e�N�X�`���̃��\�[�X�L�[
		@param �}���`���b�V�����̐^�U
		*/
		void SetMeshData(const wstring& meshKey, const wstring& texKey, bool isMulti = false);

		/*!
		@brief �C���X�^���X�`��̍s��z��ɍs���ǉ�����֐�
		@param �ǉ�����s��
		*/
		void AddMatrix(const Mat4x4& matrix);

		/*!
		@brief �C���X�^���X�`��̍s��z����N���A����֐�
		*/
		void AllClearMatrix();

		/*!
		@brief �C���X�^���X�`��̍s��z����擾����֐�
		@return vector<Mat4x4>&
		*/
		vector<Mat4x4>& GetMatrixVec();
	};

	/*!
	@brief �C���X�^���X�`�悷��̎�I�u�W�F�N�g�̊Ǘ��I�u�W�F�N�g
	*/
	class GatheringManager : public GameObject
	{
		// �C���X�^���X�`��I�u�W�F�N�g�z��
		map<eStageID, weak_ptr<InstanceGathers>> m_gathersMap;

		const Vec3 m_fullScale;	// �t���T�C�Y�̃X�P�[��
		const Vec3 m_helfScale;	// �n�[�t�T�C�Y�̃X�P�[��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GatheringManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_fullScale(Vec3(1.0f)),
			m_helfScale(Vec3(0.7f))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GatheringManager() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �̎�I�u�W�F�N�g���Đ�������֐�
		*/
		void ResetGathering();

		/*!
		@brief �̎揈���֐�
		@param �I���|�C���g
		*/
		int Gathering(const Point2D<size_t>& point);

		/*!
		@brief ���݂̃|�C���g�̈ʒu���̎�I�u�W�F�N�g���ǂ����擾�֐�
		@param �I���|�C���g
		@return ��v���Ă���true
		*/
		bool GetIsGatheringPoint(const Point2D<size_t>& point) const;

	private:

		/*!
		@brief �S�̎�I�u�W�F�N�g�̃��b�V���f�[�^�ݒ�֐�
		*/
		void SetGathersMeshData();

		/*!
		@brief �n�[�t�T�C�Y�̍̎�I�u�W�F�N�g�̎掞�̊֐�
		@param �I���|�C���g
		*/
		void HelfSizeGathering(const Point2D<size_t>& point);

		/*!
		@brief �t���T�C�Y�̍̎�I�u�W�F�N�g�̎掞�̊֐�
		@param ���̈ʒu�̍s��
		@return �n�[�t�T�C�Y�Ő��������s��
		*/
		Mat4x4 FullSizeGathering(Mat4x4& matrix);
	};
}