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
		Waiting,		// �ҋ@
		Walking,		// �ړ�
		Harvesting,		// �̎�
		CraftStart,		// �N���t�g�J�n
		Crafting,		// �N���t�g��
		CraftFinish,	// �N���t�g�I��
		QTESucces,		// QTE����
		QTEFailed,		// QTE���s
		GameSucces,		// �Q�[���N���A
		GameFailed,		// �Q�[�����s
	};

	// Player Status Type
	enum class ePST : char
	{
		WalkSpeed,
		GatherSpeed,
		StoneMax,
		WoodMax,
		GearMax,
		RailMax,
		BridgeMax,
		CrossingMax,
		StartGear,
	};

	// Player Level
	enum class ePL : char
	{
		Level1,
		Level2,
		Level3,
		Level4,
		Level5,
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
		const float m_rotSpeed;  // ��]���x

		// �ʂ�Ȃ��X�e�[�WID���X�g
		set<eStageID> m_impassableSet; 

		// �A�j���[�V�����}�b�v
		map<ePAKey, AnimationMap> m_animationMap;

		// ����SE�L�[�}�b�v
		map<eStageID, wstring> m_walkSEKeyMap;

		// �΂߂̃O���b�h���X�g
		const vector<pair<int, int>> m_obliqueGridArray;
		
		// �v���C���[�̃X�e�[�^�X
		map<ePST, map<ePL, float>> m_playerData;
		ePL m_playerLevel;

		// ���f���ƃg�����X�t�H�[�������s��
		Mat4x4 m_modelMat;

		// ����SE�̃|�C���^
		weak_ptr<SoundItem> m_walkSoundItem;

		Vec3 m_rotTarget;	// ��]��
		Vec3 m_currentRot;  // �O��̉�]��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_rotSpeed(0.5f),
			m_radius(1.0f),
			m_maxAcsel(1.0f),
			m_maxMove(2.0f),
			m_obliqueGridArray{
				make_pair(1, 1),	// �E�O
				make_pair(1, -1),	// ���O
				make_pair(-1, 1),	// �E��
				make_pair(-1, -1)	// ����
			}
		{
			m_acsel = 0.0f;
			m_moveValue = 0.0f;
			m_rotTarget.zero(); // ��]���0.0f�ŏ�����
			m_currentRot.zero(); // ��]���0.0f�ŏ�����
			m_playerLevel = ePL::Level1;

			// ���f���ƃg�����X�t�H�[���̍����s���ݒ�
			m_modelMat.affineTransformation(
				Vec3(0.5f),						// �X�P�[��
				Vec3(0.0f),						// ��]��
				Vec3(0.0f, -XM_PIDIV2, 0.0f),	// ���[�e�[�V����
				Vec3(0.0f, -1.05f, 0.0f)		// �|�W�V����
			);

			// �A�j���[�V�����L�[
			m_animationMap.emplace(ePAKey::Waiting, AnimationMap(L"WAIT", 24, 0.75f, true));	// �ҋ@
			m_animationMap.emplace(ePAKey::Walking, AnimationMap(L"WALK", 12, 0.75f, true));	// �ړ�
			m_animationMap.emplace(ePAKey::Harvesting, AnimationMap(L"HARVESTING", 24, 1.0f));	// ����
			m_animationMap.emplace(ePAKey::CraftStart, AnimationMap(L"C_START", 10, 1.0f));		// �N���t�g�J�n
			m_animationMap.emplace(ePAKey::Crafting, AnimationMap(L"C_NOW", 15, 1.0f, true));	// �N���t�g��
			m_animationMap.emplace(ePAKey::CraftFinish, AnimationMap(L"C_END", 10, 1.0f));		// �N���t�g�I��
			m_animationMap.emplace(ePAKey::QTESucces, AnimationMap(L"SUCCES", 24, 1.0f));		// QTE����
			m_animationMap.emplace(ePAKey::QTEFailed, AnimationMap(L"FAILED", 24, 1.0f));		// QTE���s
			m_animationMap.emplace(ePAKey::GameSucces, AnimationMap(L"SUCCES", 24, 1.0f, true));// �Q�[��������
			m_animationMap.emplace(ePAKey::GameFailed, AnimationMap(L"FAILED", 24, 0.5f));		// �Q�[�����s��

			// ����SE�̃L�[
			m_walkSEKeyMap.emplace(eStageID::Grass, L"WALK_GRASS_SE");	// ���n�̎���SE
			m_walkSEKeyMap.emplace(eStageID::Sand, L"WALK_SAND_SE");	// ���n�̎���SE
			m_walkSEKeyMap.emplace(eStageID::Rock, L"WALK_ROCK_SE");	// �Βn�̎���SE

			// �ړ��s��ID�̓o�^
			m_impassableSet.insert(eStageID::Rail);
			m_impassableSet.insert(eStageID::DeRail);
			m_impassableSet.insert(eStageID::GoalRail);
			m_impassableSet.insert(eStageID::StartRail);
			m_impassableSet.insert(eStageID::DrawRail);
			m_impassableSet.insert(eStageID::Tree1);
			m_impassableSet.insert(eStageID::Tree2);
			m_impassableSet.insert(eStageID::Stone1);
			m_impassableSet.insert(eStageID::Stone2);
			m_impassableSet.insert(eStageID::Stone3);
			m_impassableSet.insert(eStageID::UnBreakRock);
			m_impassableSet.insert(eStageID::CrossingCross);
			m_impassableSet.insert(eStageID::Water);
			m_impassableSet.insert(eStageID::Air);
			m_impassableSet.insert(eStageID::UnGrass);

			// �v���C���[�̃��x�����̃X�e�[�^�X��ݒ�
			vector<map<ePL, float>> tempMap(8);
			auto status = CSVLoader::ReadDataToFloat(CSVLoader::LoadFile("PlayerLevel"));
			for (size_t i = 0; i < tempMap.size(); i++)
			{
				tempMap.at(i).emplace(ePL::Level1, status.at(i).at(1));
				tempMap.at(i).emplace(ePL::Level2, status.at(i).at(2));
				tempMap.at(i).emplace(ePL::Level3, status.at(i).at(3));
				tempMap.at(i).emplace(ePL::Level4, status.at(i).at(4));
				tempMap.at(i).emplace(ePL::Level5, status.at(i).at(5));

				m_playerData.emplace(static_cast<ePST>(i), tempMap.at(i));
			}
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

		/*!
		@brief ����������SE�J�n�֐�
		*/
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