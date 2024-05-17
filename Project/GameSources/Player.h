/*!
@file Player.h
@brief �v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "BaseStage.h"
#include "TemplateObject.h"

namespace basecross
{
	// �v���C���[�̃A�j���[�V�����L�[�^�C�v
	enum class ePAKey : char
	{
		Waiting,			// �ҋ@
		Walking,			// �ړ�
		Harvesting,		// �̎�
		CraftStart,		// �N���t�g�J�n
		Crafting,		// �N���t�g��
		CraftFinish,	// �N���t�g�I��
		QTESucces,		// QTE����
		QTEFailed,		// QTE���s
	};

	/*!
	@brief �A�j���[�V�����f�[�^�\����
	*/
	struct AnimationMap
	{
		wstring animeKey;	// �A�j���[�V�����L�[
		UINT flameNum;		// �t���[����
		float animeSpeed;	// �A�j���[�V�����̑��x
		bool loopActive;	// ���[�v���邩�̐^�U

		/*!
		@brief �R���X�g���N�^
		@param �A�j���[�V�����L�[
		@param �J�n���̃t���[��
		@param �I�����̃t���[��
		@param ���[�v���邩(�f�t�H���g��false)
		*/
		AnimationMap(wstring aniKey, UINT frame, float speed, bool loop = false) :
			animeKey(aniKey),
			flameNum(frame),
			animeSpeed(speed),
			loopActive(loop)
		{
		}
	};

	/*!
	@brief �v���C���[
	*/
	class Player : public TemplateObject
	{
	protected:

		// �`��R���|�[�l���g
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; 
		shared_ptr<Shadowmap> m_ptrShadow; 

		float m_acsel;			 // �����x
		float m_moveValue;		 // �^����(�ړ��Ɖ�])
		const float m_maxAcsel;	 // �ő�����x
		const float m_maxMove;	 // �ő�^����
		const float m_radius;	 // �Փ˔���p���a
		const float m_moveSpeed; // �ړ����x
		const float m_rotSpeed;  // ��]���x

		// �ʂ�Ȃ��X�e�[�WID���X�g
		set<eStageID> m_impassableSet; 

		// �A�j���[�V�����}�b�v
		map<ePAKey, AnimationMap> m_animationMap;

		// ����SE�L�[�}�b�v
		map<eStageID, wstring> m_walkSEKeyMap;

		// �΂߂̃O���b�h���X�g
		vector<pair<int, int>> m_obliqueGridArray;

		// ����SE�̃|�C���^
		weak_ptr<SoundItem> m_walkSoundItem;

		Vec3 m_rotTarget;	// ��]��
		Vec3 m_currentRot;  // �O��̉�]��
		Mat4x4 m_modelMat;	// ���f���ƃg�����X�t�H�[�������s��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_rotSpeed(0.5f),  // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_radius(1.0f),
			m_maxAcsel(1.0f),
			m_maxMove(2.0f)
		{
			m_acsel = 0.0f;
			m_moveValue = 0.0f;
			m_rotTarget.zero(); // ��]���0.0f�ŏ�����
			m_currentRot.zero(); // ��]���0.0f�ŏ�����

			// �A�j���[�V�����L�[
			m_animationMap.emplace(ePAKey::Waiting, AnimationMap(L"WAIT", 24, 0.75f, true));	// �ҋ@
			m_animationMap.emplace(ePAKey::Walking, AnimationMap(L"WALK", 12, 0.75f, true));	// �ړ�
			m_animationMap.emplace(ePAKey::Harvesting, AnimationMap(L"HARVESTING", 24, 1.0f));	// ����
			m_animationMap.emplace(ePAKey::CraftStart, AnimationMap(L"C_START", 10, 1.0f));		// �N���t�g�J�n
			m_animationMap.emplace(ePAKey::Crafting, AnimationMap(L"C_NOW", 15, 1.0f, true));	// �N���t�g��
			m_animationMap.emplace(ePAKey::CraftFinish, AnimationMap(L"C_END", 10, 1.0f));		// �N���t�g�I��
			m_animationMap.emplace(ePAKey::QTESucces, AnimationMap(L"SUCCES", 24, 1.0f));		// QTE����
			m_animationMap.emplace(ePAKey::QTEFailed, AnimationMap(L"FAILED", 24, 1.0f));		// QTE���s

			// ����SE�̃L�[
			m_walkSEKeyMap.emplace(eStageID::Grass, L"WALK_GRASS_SE");	// ���n�̎���SE
			m_walkSEKeyMap.emplace(eStageID::Sand, L"WALK_SAND_SE");	// ���n�̎���SE
			m_walkSEKeyMap.emplace(eStageID::Rock, L"WALK_ROCK_SE");	// �Βn�̎���SE

			// �אڂ���O���b�h
			m_obliqueGridArray.push_back(make_pair(1, 1));		// �E�O
			m_obliqueGridArray.push_back(make_pair(1, -1));		// ���O
			m_obliqueGridArray.push_back(make_pair(-1, 1));		// �E��
			m_obliqueGridArray.push_back(make_pair(-1, -1));	// ����

			// �X�P�[�������AY��������2�{�ɂ���
			m_modelMat.affineTransformation(
				Vec3(0.5f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.05f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Player() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

	protected:

		/*!
		@brief �R���|�[�l���g�̐����֐�
		*/
		virtual void CreateComponent() = 0;

		/*!
		@brief �A�j���[�V�������b�V���̍X�V
		@param animationKey
		*/
		virtual void SetAnimationMesh(ePAKey animation, float start = 0.0f);

		/*!
		@brief �A�j���[�V�����̂�������
		@param DELTA_TIME�Ɋ|���鑬�x
		*/
		virtual void UpdateAnimation(float speedValue = 1.0f);

		/*!
		@brief �w�肵���A�j���[�V�������̃`�F�b�N
		@param animationKey
		@return ��v���Ă���true
		*/
		virtual bool IsAnimation(ePAKey animation);

		/*!
		@brief �w�肵���A�j���[�V�������I�����Ă��邩�̃`�F�b�N
		@param animationKey
		@return �I�����Ă���true
		*/
		virtual bool IsAnimeEnd(ePAKey animation);

		/*!
		@brief �ړ��X�V�֐�
		*/
		virtual void UpdateMove();

		/*!
		@brief ��]�X�V�֐�
		*/
		virtual void UpdateRotation();

		/*!
		@brief ��]��ݒ�֐�
		@param L�X�e�B�b�N���͗�
		*/
		virtual void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief �R���g���[���[�ړ��֐�
		@param L�X�e�B�b�N���͗�
		*/
		virtual void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief �ړ��o���Ȃ��O���b�h���܂�����֐�
		@param �|�W�V����
		*/
		virtual void GridHitResponse(Vec3& pos);

		virtual void StartWalkSoundEffect();

	private:

		/*!
		@brief �w�肵���s�񂪏Փ˔�������O���b�h���̐^�U�擾�֐�
		@param row
		@param col
		@return bool �Փ˔�������O���b�h�Ȃ�true,���Ȃ��O���b�h�Ȃ�false
		*/
		virtual bool GetIsImpassable(size_t row, size_t col);

		/*!
		@brief �O�����̒藝�ŃO���b�h�ƃv���C���[�̉����o������֐�
		@param pos
		@param gridX
		@param gridZ
		*/
		virtual void GridSquareTheorem(Vec3& pos, float gridX, float gridZ);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitLeftResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(�E)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitRightResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(�΂�)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitOliqueResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief �X�e�[�W�͈̔͊O�֍s���Ȃ��l�ɂ���֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridRangeResponse(Vec3& pos, const vector<vector<int>>& stageMap);
	};
}