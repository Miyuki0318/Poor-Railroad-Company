/*!
@file TemplateObject.h
@brief �I�u�W�F�N�g�̌p����
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief �e���v���[�g�I�u�W�F�N�g
	*/
	class TemplateObject : public GameObject
	{
	protected:

		shared_ptr<Transform> m_ptrTrans; // �g�����X�t�H�[���R���|�[�l���g

		Vec3 m_position; // �|�W�V����
		Vec3 m_rotation; // ���[�e�[�V����
		Vec3 m_scale;    // �X�P�[��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		TemplateObject(const shared_ptr<Stage>& stagePtr
		) :
			GameObject(stagePtr)
		{
			m_position.zero();
			m_rotation.zero();
			m_scale.zero();
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param ���[�e�[�V����
		@param �X�P�[��
		*/
		TemplateObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			GameObject(stagePtr),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TemplateObject() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �|�W�V�����ݒ�֐�
		@param Vec3�^���W
		*/
		virtual void SetPosition(const Vec3& position)
		{
			m_position = position;
			m_ptrTrans->SetPosition(m_position);
		}

		/*!
		@brief �|�W�V�����ݒ�֐�
		@param Vec2�^���W
		*/
		virtual void SetPosition(const Vec2& position)
		{
			SetPosition(Vec3(position));
		}

		/*!
		@brief �|�W�V�����ݒ�֐�
		@param float3�^���W
		*/
		virtual void SetPosition(const float& x, const float& y, const float& z)
		{
			SetPosition(Vec3(x, y, z));
		}

		/*!
		@brief �|�W�V�����ݒ�֐�
		@param float2�^���W
		*/
		virtual void SetPosition(const float& x, const float& y)
		{
			SetPosition(Vec2(x, y));
		}

		/*!
		@brief �|�W�V�����擾�֐�
		@return Vec3�^���W
		*/
		virtual Vec3 GetPosition() const
		{
			return m_ptrTrans->GetPosition();
		}

		/*!
		@brief ���[�e�[�V�����ݒ�֐�
		@param Vec3�^Radian
		*/
		virtual void SetRotation(const Vec3& rotation)
		{
			m_rotation = rotation;
			m_ptrTrans->SetRotation(m_rotation);
		}

		/*!
		@brief ���[�e�[�V�����ݒ�֐�
		@param Vec3�^Degree
		*/
		virtual void SetDegreeAngle(const Vec3& degree)
		{
			SetRotation(Utility::DegToRad(degree));
		}

		/*!
		@brief ���[�e�[�V�����擾�֐�
		@return Vec3�^Radian
		*/
		virtual Vec3 GetRotation() const
		{
			return m_rotation;
		}

		/*!
		@brief ���[�e�[�V�����擾�֐�
		@return Vec3�^Degree
		*/
		virtual Vec3 GetDegreeAngle() const
		{
			return Utility::RadToDeg(m_rotation);
		}

		/*!
		@brief �X�P�[���ݒ�֐�
		@param Vec3�^Scale
		*/
		virtual void SetScale(const Vec3& scale)
		{
			m_scale = scale;
			m_ptrTrans->SetScale(m_scale);
		}

		/*!
		@brief �X�P�[���ݒ�֐�
		@param Vec2�^Scale
		*/
		virtual void SetScale(const Vec2& scale)
		{
			SetScale(Vec3(scale.x, scale.y, 1.0f));
		}

		/*!
		@brief �X�P�[���ݒ�֐�
		@param float3�^Scale
		*/
		virtual void SetScale(const float& x, const float& y, const float& z)
		{
			SetScale(Vec3(x, y, z));
		}

		/*!
		@brief �X�P�[���ݒ�֐�
		@param float2�^Scale
		*/
		virtual void SetScale(const float& x, const float& y)
		{
			SetScale(Vec2(x, y));
		}

		/*!
		@brief �X�P�[���ݒ�֐�
		@param float�^Scale
		*/
		virtual void SetScale(const float& scale)
		{
			SetScale(Vec3(scale));
		}

		/*!
		@brief �X�P�[���擾�֐�
		@return Vec3�^Scale
		*/
		virtual Vec3 GetScale() const
		{
			return m_scale;
		}

		/*!
		@brief �g�����X�t�H�[���ȈՐݒ�
		*/
		virtual void SetTransParam()
		{
			SetPosition(m_position);
			SetRotation(m_rotation);
			SetScale(m_scale);
		}

		///*!
		//@brief �X�e�[�W�X�e�[�g�ݒ�֐�
		//@param eStageState
		//@param �G���[���O���o�����̐^�U(�f�t�H���g��true)
		//*/
		//template <typename T>
		//void SetStageState(const typename T::eStageState& state, bool exceptionActive = true)
		//{
		//	GetTypeStage<T>(exceptionActive)->SetStageState(state);
		//}

		///*!
		//@brief �X�e�[�W�X�e�[�g�擾
		//@param �G���[���O���o�����̐^�U(�f�t�H���g��true)
		//@return eStageState
		//*/
		//template <typename T>
		//const typename T::eStageState& GetStageState(bool exceptionActive = true)
		//{
		//	return GetTypeStage<T>(exceptionActive)->GetStageState();
		//}

		/*!
		@brief SE�Đ��֐�(�r���ŏI���ł����)
		*/
		virtual shared_ptr<SoundItem> StartSE(const wstring& seKey, float volume);

		/*!
		@brief SE�I���֐�
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief �^�C�}�[�֐�
		@param �ݒ莞��(s)
		@param ���Z�b�g�^�U(�f�t�H���g��false)
		@return �^�C�}�[���I�����Ă��邩�̐^�U
		*/
		virtual bool SetTimer(float time, bool reset = false);

		/*!
		@brief �^�C�}�[�̎��Ԏ擾�֐�
		@return �o�ߎ���
		*/
		virtual float GetTime(float time);

		///*!
		//@brief �w��̍��W�ɃI�u�W�F�N�g�����邩�̊m�F�֐�
		//@return ���݂��Ă����true
		//*/
		//virtual bool ObjectCheck(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& checkPos)
		//{
		//	bool check = false;

		//	for (const auto& ptr : groupVec)
		//	{
		//		if (!ptr.lock()) continue;

		//		// �^�L���X�g
		//		const auto& block = dynamic_pointer_cast<TemplateObject>(ptr.lock());
		//		if (!block) continue;

		//		// ���W����v���Ă��邩
		//		if (block->GetPosition() == checkPos)
		//		{
		//			check = true;
		//		}
		//	}

		//	return check;
		//}

		///*!
		//@brief �w��̍��W�ɃA�N�e�B�u�ȃu���b�N�����邩�̊m�F�֐�
		//@return ���݂��Ă����true
		//*/
		//virtual bool BlockCheck(const Vec3& checkPos)
		//{
		//	// �A�N�e�B�u�O���[�v�̎擾
		//	const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Active")->GetGroupVector();

		//	return ObjectCheck(blockVec, checkPos);
		//}

		///*!
		//@brief �I�u�W�F�N�g�ɏォ��Փ˂��Ă��邩
		//@param �Փˍ��W
		//@param �Փ˂����I�u�W�F�N�g�̍��W
		//@param �Փ˂����I�u�W�F�N�g�̔����̃X�P�[��
		//@return �Փ˂��Ă��邩�̐^�U
		//*/
		//virtual bool CollHitUpper(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		//{
		//	return hitPos.y > hitObjPos.y && ((hitPos.y - hitObjPos.y) >= helfScale.y);
		//}

		///*!
		//@brief �I�u�W�F�N�g�ɉ�����Փ˂��Ă��邩
		//@param �Փˍ��W
		//@param �Փ˂����I�u�W�F�N�g�̍��W
		//@param �Փ˂����I�u�W�F�N�g�̔����̃X�P�[��
		//@return �Փ˂��Ă��邩�̐^�U
		//*/
		//virtual bool CollHitUnder(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		//{
		//	return hitPos.y < hitObjPos.y && ((hitPos.y - hitObjPos.y) <= -helfScale.y);
		//}

		///*!
		//@brief �I�u�W�F�N�g�ɍ�����Փ˂��Ă��邩
		//@param �Փˍ��W
		//@param �Փ˂����I�u�W�F�N�g�̍��W
		//@param �Փ˂����I�u�W�F�N�g�̔����̃X�P�[��
		//@return �Փ˂��Ă��邩�̐^�U
		//*/
		//virtual bool CollHitLeft(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		//{
		//	return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
		//		&& ((hitPos.x - hitObjPos.x) < helfScale.x);
		//}

		///*!
		//@brief �I�u�W�F�N�g�ɉE����Փ˂��Ă��邩
		//@param �Փˍ��W
		//@param �Փ˂����I�u�W�F�N�g�̍��W
		//@param �Փ˂����I�u�W�F�N�g�̔����̃X�P�[��
		//@return �Փ˂��Ă��邩�̐^�U
		//*/
		//virtual bool CollHitRight(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		//{
		//	return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
		//		&& ((hitPos.x - hitObjPos.x) > -helfScale.x);
		//}
	};
}