/*!
@file Player.h
@brief �v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "BaseStage.h"
#include "TemplateObject.h"
#include "SelectIndicator.h"
#include "CraftManager.h"
#include "Scene.h"

#define ANIME_FPS 30.0f

namespace basecross
{
	// Player Animation Key
	enum class ePAK : char
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

	// �v���C���[�̏��
	enum class ePlayerStatus : uint16_t
	{
		IsIdle,			// �ҋ@���
		IsMove,			// �ړ����
		IsRotate,		// ��]���
		IsGathering,	// �̌@���
		IsHaveWood,		// �؂�������
		IsHaveStone,	// �΂�������
		IsCrafting,		// �N���t�g��
		IsCraftQTE,		// �N���t�gQTE��
		IsHaveRail,		// ���H������
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

		weak_ptr<SelectIndicator> m_indicator; // �Z���N�g�C���f�B�P�[�^�[

		float m_acsel;			 // �����x
		float m_moveValue;		 // �^����(�ړ��Ɖ�])
		const float m_maxAcsel;	 // �ő�����x
		const float m_maxMove;	 // �ő�^����
		const float m_radius;	 // �Փ˔���p���a
		const float m_rotSpeed;  // ��]���x

		// �ʂ�Ȃ��X�e�[�WID���X�g
		set<eStageID> m_impassableSet; 

		// �A�j���[�V�����}�b�v
		map<ePAK, AnimationMap> m_animationMap;

		pair<eItemType, bool> m_addItem; // �ǉ������A�C�e���̃^�C�v
		map<eStageID, pair<eItemType, wstring>> m_gatherMap; // �̎�ΏۂƎ擾�A�C�e���^�C�v

		// ����SE�L�[�}�b�v
		map<eStageID, wstring> m_walkSEKeyMap;

		// �΂߂̃O���b�h���X�g
		const vector<pair<int, int>> m_obliqueGridArray;
		
		Bool16_t<ePlayerStatus> m_status; // �t���O�Ǘ��N���X

		// �v���C���[�̃X�e�[�^�X
		map<ePST, map<ePL, float>> m_playerData;
		ePL m_statusLevel;
		ePL m_limitChoicesLevel;
		ePL m_startGearLevel;

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
			TemplateObject(stagePtr, Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, XM_PIDIV2, 0.0f), Vec3(1.0f)),
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
			m_status = 0;
			m_acsel = 0.0f;
			m_moveValue = 0.0f;
			m_rotTarget = Vec3(0.0f, XM_PIDIV2, 0.0f); // ��]���0.0f�ŏ�����
			m_currentRot = Vec3(0.0f, XM_PIDIV2, 0.0f); // ��]���0.0f�ŏ�����
			m_statusLevel = ePL::Level1;
			m_limitChoicesLevel = ePL::Level1;
			m_startGearLevel = ePL::Level1;

			// ���f���ƃg�����X�t�H�[���̍����s���ݒ�
			m_modelMat.affineTransformation(
				Vec3(0.5f),						// �X�P�[��
				Vec3(0.0f),						// ��]��
				Vec3(0.0f, -XM_PIDIV2, 0.0f),	// ���[�e�[�V����
				Vec3(0.0f, -1.05f, 0.0f)		// �|�W�V����
			);

			// ��芸�����؂ŏ�����
			m_addItem = make_pair(eItemType::Wood, false); 

			// �̎�I�u�W�F�N�g��ID�ƍ̎掞�Ɉ����f�[�^
			m_gatherMap.emplace(eStageID::Stone1, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Stone2, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Stone3, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Tree1, make_pair(eItemType::Wood, L"TREE"));
			m_gatherMap.emplace(eStageID::Tree2, make_pair(eItemType::Wood, L"TREE"));

			// �A�j���[�V�����L�[
			m_animationMap.emplace(ePAK::Waiting, AnimationMap(L"WAIT", 24, 0.75f, true));		// �ҋ@
			m_animationMap.emplace(ePAK::Walking, AnimationMap(L"WALK", 12, 0.75f, true));		// �ړ�
			m_animationMap.emplace(ePAK::Harvesting, AnimationMap(L"HARVESTING", 24, 1.0f));	// ����
			m_animationMap.emplace(ePAK::CraftStart, AnimationMap(L"C_START", 10, 1.0f));		// �N���t�g�J�n
			m_animationMap.emplace(ePAK::Crafting, AnimationMap(L"C_NOW", 15, 1.0f, true));		// �N���t�g��
			m_animationMap.emplace(ePAK::CraftFinish, AnimationMap(L"C_END", 10, 1.0f));		// �N���t�g�I��
			m_animationMap.emplace(ePAK::QTESucces, AnimationMap(L"SUCCES", 24, 1.0f));			// QTE����
			m_animationMap.emplace(ePAK::QTEFailed, AnimationMap(L"FAILED", 24, 1.0f));			// QTE���s
			m_animationMap.emplace(ePAK::GameSucces, AnimationMap(L"SUCCES", 24, 1.0f, true));	// �Q�[��������
			m_animationMap.emplace(ePAK::GameFailed, AnimationMap(L"FAILED", 24, 0.5f));		// �Q�[�����s��

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
			vector<map<ePL, float>> tempMap(9);
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

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		virtual void AddItemCount(eItemType type, int addNum = 1) {};

		/*!
		@brief �A�C�e�����擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		virtual int GetItemCount(eItemType type) 
		{
			return 0;
		};

		/*!
		@brief �A�C�e��������擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		virtual int GetLimitCount(eItemType type)
		{
			return 99;
		}

		/*!
		@brief �N���t�g�ł���̎擾�֐�
		@param �N���t�g�A�C�e��enum
		@return �N���t�g�ł��邩�̐^�U
		*/
		virtual bool GetCraftPosshible() const
		{
			return false;
		}

		/*!
		@brief ��Ԏ擾�֐�
		@param �v���C���[�̏��enum
		@return ���̏�ԂɂȂ��Ă��邩�̐^�U
		*/
		bool GetStatus(ePlayerStatus status) const
		{
			return m_status(status);
		}

	protected:

		/*!
		@brief �R���|�[�l���g�̐����֐�
		*/
		virtual void CreateComponent();

		/*!
		@brief �v���C���[�ɕt������@�\�����֐�
		*/
		virtual void CreatePlayerFeatures() = 0;

		/*!
		@brief �C���f�B�P�[�^�[�ւ̎擾�ƌĂяo���֐�
		*/
		virtual void IndicatorOrder() = 0;

		/*!
		@brief �̌@���ߊ֐�
		@param �C���f�B�P�[�^�[�̃|�C���^
		@return �̌@�ł�����
		*/
		bool GatheringOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief �̌@���ɌĂяo�����֐�
		@param �̌@�����I�u�W�F�N�g�̃^�O
		*/
		void GatheringProcces(int stageID);

		/*!
		@brief �A�j���[�V�������b�V���̍X�V
		@param animationKey
		*/
		virtual void SetAnimationMesh(ePAK animation, float start = 0.0f);

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
		virtual bool IsAnimation(ePAK animation);

		/*!
		@brief �w�肵���A�j���[�V�������I�����Ă��邩�̃`�F�b�N
		@param animationKey
		@return �I�����Ă���true
		*/
		virtual bool IsAnimeEnd(ePAK animation);

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
		@brief �A�N�V�������ɃC���f�B�P�[�^�[�̕��։�]�ݒ肷��֐�
		*/
		void SetRotateIndicatorAngle();

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