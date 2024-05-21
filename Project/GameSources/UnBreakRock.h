/*!
@file UnBreakRock.h
@brief �j��ł��Ȃ���`��I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	// �󂹂Ȃ���̃��b�V���^�C�venum
	enum class eRockMeshType : int
	{
		Mesh1,	// ���b�V���^�C�v1
		Mesh2,	// ���b�V���^�C�v2
		Mesh3,	// ���b�V���^�C�v3
	};

	// �󂹂Ȃ���Ɏg���f�[�^�p
	namespace UnBreakRockData
	{
		// �󂹂Ȃ���̃��b�V���L�[�}�b�v
		static map<eRockMeshType, wstring> UnBreakRockMeshMap = {
			{eRockMeshType::Mesh1, L"UN_BREAK_ROCK1"},	// �^�C�v1
			{eRockMeshType::Mesh2, L"UN_BREAK_ROCK2"},	// �^�C�v2
			{eRockMeshType::Mesh3, L"UN_BREAK_ROCK3"},	// �^�C�v3
		};
	}

	/*!
	@brief �C���X�^���X�`�悷��󂹂Ȃ���
	*/
	class InstanceRock : public GameObject
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

		// ���b�V���̃L�[
		const eRockMeshType m_meshType;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �ݒ肷�郁�b�V���̃L�[
		*/
		InstanceRock(const shared_ptr<Stage>& stagePtr,
			const eRockMeshType& meshType
		) :
			GameObject(stagePtr),
			m_meshType(meshType)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~InstanceRock() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �n�ʕ`��ǉ��֐�
		@param �ǉ�����s��
		*/
		void AddMatrix(const Mat4x4 matrix)
		{
			if (!m_ptrDraw) return;
			m_ptrDraw->AddMatrix(matrix);
		}
	};

	/*!
	@brief �󂹂Ȃ���`�搶���I�u�W�F�N�g
	*/
	class UnBreakRock : public TemplateObject
	{
		Mat4x4 m_mtxScale;		// �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotation;	// �C���X�^���X�`��p�̃��[�e�[�V����
		Mat4x4 m_mtxPosition;	// �C���X�^���X�`��p�̃��[�e�[�V����

		// �C���X�^���X�`��}�b�v
		map<eRockMeshType, weak_ptr<InstanceRock>> m_instanceMap;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		UnBreakRock(const shared_ptr<Stage>& stagePtr
		) :
			TemplateObject(stagePtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~UnBreakRock() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;
	};
}