/*!
@file Player.h
@brief プレイヤーオブジェクト
@author 小澤博貴
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
		Waiting,		// 待機
		Walking,		// 移動
		Harvesting,		// 採取
		CraftStart,		// クラフト開始
		Crafting,		// クラフト中
		CraftFinish,	// クラフト終了
		QTESucces,		// QTE成功
		QTEFailed,		// QTE失敗
		GameSucces,		// ゲームクリア
		GameFailed,		// ゲーム失敗
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

	// プレイヤーの状態
	enum class ePlayerStatus : uint16_t
	{
		IsIdle,			// 待機状態
		IsMove,			// 移動状態
		IsRotate,		// 回転状態
		IsGathering,	// 採掘状態
		IsHaveWood,		// 木を所持中
		IsHaveStone,	// 石を所持中
		IsCrafting,		// クラフト中
		IsCraftQTE,		// クラフトQTE中
		IsHaveRail,		// 線路所持中
	};

	/*!
	@brief アニメーションデータ構造体
	*/
	struct AnimationMap
	{
		wstring animeKey;	// アニメーションキー
		UINT flameNum;		// フレーム数
		float animeSpeed;	// アニメーションの速度
		bool loopActive;	// ループするかの真偽

		/*!
		@brief コンストラクタ
		@param アニメーションキー
		@param 開始時のフレーム
		@param 終了時のフレーム
		@param ループするか(デフォルトはfalse)
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
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
	protected:

		// 描画コンポーネント
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; 
		shared_ptr<Shadowmap> m_ptrShadow; 

		weak_ptr<SelectIndicator> m_indicator; // セレクトインディケーター

		float m_acsel;			 // 加速度
		float m_moveValue;		 // 運動量(移動と回転)
		const float m_maxAcsel;	 // 最大加速度
		const float m_maxMove;	 // 最大運動量
		const float m_radius;	 // 衝突判定用半径
		const float m_rotSpeed;  // 回転速度

		// 通れないステージIDリスト
		set<eStageID> m_impassableSet; 

		// アニメーションマップ
		map<ePAK, AnimationMap> m_animationMap;

		pair<eItemType, bool> m_addItem; // 追加したアイテムのタイプ
		map<eStageID, pair<eItemType, wstring>> m_gatherMap; // 採取対象と取得アイテムタイプ

		// 歩くSEキーマップ
		map<eStageID, wstring> m_walkSEKeyMap;

		// 斜めのグリッドリスト
		const vector<pair<int, int>> m_obliqueGridArray;
		
		Bool16_t<ePlayerStatus> m_status; // フラグ管理クラス

		// プレイヤーのステータス
		map<ePST, map<ePL, float>> m_playerData;
		ePL m_statusLevel;
		ePL m_limitChoicesLevel;
		ePL m_startGearLevel;

		// モデルとトランスフォーム差分行列
		Mat4x4 m_modelMat;

		// 歩くSEのポインタ
		weak_ptr<SoundItem> m_walkSoundItem;

		Vec3 m_rotTarget;	// 回転先
		Vec3 m_currentRot;  // 前回の回転軸

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, XM_PIDIV2, 0.0f), Vec3(1.0f)),
			m_rotSpeed(0.5f),
			m_radius(1.0f),
			m_maxAcsel(1.0f),
			m_maxMove(2.0f),
			m_obliqueGridArray{
				make_pair(1, 1),	// 右前
				make_pair(1, -1),	// 左前
				make_pair(-1, 1),	// 右奥
				make_pair(-1, -1)	// 左奥
			}
		{
			m_status = 0;
			m_acsel = 0.0f;
			m_moveValue = 0.0f;
			m_rotTarget = Vec3(0.0f, XM_PIDIV2, 0.0f); // 回転先は0.0fで初期化
			m_currentRot = Vec3(0.0f, XM_PIDIV2, 0.0f); // 回転先は0.0fで初期化
			m_statusLevel = ePL::Level1;
			m_limitChoicesLevel = ePL::Level1;
			m_startGearLevel = ePL::Level1;

			// モデルとトランスフォームの差分行列を設定
			m_modelMat.affineTransformation(
				Vec3(0.5f),						// スケール
				Vec3(0.0f),						// 回転軸
				Vec3(0.0f, -XM_PIDIV2, 0.0f),	// ローテーション
				Vec3(0.0f, -1.05f, 0.0f)		// ポジション
			);

			// 取り敢えず木で初期化
			m_addItem = make_pair(eItemType::Wood, false); 

			// 採取オブジェクトのIDと採取時に扱うデータ
			m_gatherMap.emplace(eStageID::Stone1, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Stone2, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Stone3, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Tree1, make_pair(eItemType::Wood, L"TREE"));
			m_gatherMap.emplace(eStageID::Tree2, make_pair(eItemType::Wood, L"TREE"));

			// アニメーションキー
			m_animationMap.emplace(ePAK::Waiting, AnimationMap(L"WAIT", 24, 0.75f, true));		// 待機
			m_animationMap.emplace(ePAK::Walking, AnimationMap(L"WALK", 12, 0.75f, true));		// 移動
			m_animationMap.emplace(ePAK::Harvesting, AnimationMap(L"HARVESTING", 24, 1.0f));	// 伐採
			m_animationMap.emplace(ePAK::CraftStart, AnimationMap(L"C_START", 10, 1.0f));		// クラフト開始
			m_animationMap.emplace(ePAK::Crafting, AnimationMap(L"C_NOW", 15, 1.0f, true));		// クラフト中
			m_animationMap.emplace(ePAK::CraftFinish, AnimationMap(L"C_END", 10, 1.0f));		// クラフト終了
			m_animationMap.emplace(ePAK::QTESucces, AnimationMap(L"SUCCES", 24, 1.0f));			// QTE成功
			m_animationMap.emplace(ePAK::QTEFailed, AnimationMap(L"FAILED", 24, 1.0f));			// QTE失敗
			m_animationMap.emplace(ePAK::GameSucces, AnimationMap(L"SUCCES", 24, 1.0f, true));	// ゲーム成功時
			m_animationMap.emplace(ePAK::GameFailed, AnimationMap(L"FAILED", 24, 0.25f));		// ゲーム失敗時

			// 歩くSEのキー
			m_walkSEKeyMap.emplace(eStageID::Grass, L"WALK_GRASS_SE");	// 草地の時のSE
			m_walkSEKeyMap.emplace(eStageID::Sand, L"WALK_SAND_SE");	// 砂地の時のSE
			m_walkSEKeyMap.emplace(eStageID::Rock, L"WALK_ROCK_SE");	// 石地の時のSE

			// 移動不可なIDの登録
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

			// プレイヤーのレベル事のステータスを設定
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
		@brief デストラクタ
		*/
		virtual ~Player() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		virtual void AddItemCount(eItemType type, int addNum = 1) {};

		/*!
		@brief アイテム数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		virtual int GetItemCount(eItemType type) 
		{
			return 0;
		};

		/*!
		@brief アイテム上限数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		virtual int GetLimitCount(eItemType type)
		{
			return 99;
		}

		/*!
		@brief クラフトできるの取得関数
		@param クラフトアイテムenum
		@return クラフトできるかの真偽
		*/
		virtual bool GetCraftPosshible() const
		{
			return false;
		}

		/*!
		@brief 状態取得関数
		@param プレイヤーの状態enum
		@return その状態になっているかの真偽
		*/
		bool GetStatus(ePlayerStatus status) const
		{
			return m_status(status);
		}

		/*!
		@brief コントローラーで向けた方向取得関数
		@return atan2f(m_rotTarget.z, m_rotTarget.x)
		*/
		float GetPastRotTarget() const
		{
			return atan2f(m_rotTarget.z, m_rotTarget.x);
		}

	protected:

		/*!
		@brief コンポーネントの生成関数
		*/
		virtual void CreateComponent();

		/*!
		@brief プレイヤーに付加する機能生成関数
		*/
		virtual void CreatePlayerFeatures() = 0;

		/*!
		@brief インディケーターへの取得と呼び出し関数
		*/
		virtual void IndicatorOrder() = 0;

		/*!
		@brief 採掘命令関数
		@param インディケーターのポインタ
		@return 採掘できたか
		*/
		bool GatheringOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief 採掘時に呼び出される関数
		@param 採掘されるオブジェクトのタグ
		*/
		void GatheringProcces(int stageID);

		/*!
		@brief アニメーションメッシュの更新
		@param animationKey
		*/
		virtual void SetAnimationMesh(ePAK animation, float start = 0.0f);

		/*!
		@brief アニメーションのこうしん
		@param DELTA_TIMEに掛ける速度
		*/
		virtual void UpdateAnimation(float speedValue = 1.0f);

		/*!
		@brief 指定したアニメーションかのチェック
		@param animationKey
		@return 一致してたらtrue
		*/
		virtual bool IsAnimation(ePAK animation);

		/*!
		@brief 指定したアニメーションが終了しているかのチェック
		@param animationKey
		@return 終了してたらtrue
		*/
		virtual bool IsAnimeEnd(ePAK animation);

		/*!
		@brief 移動更新関数
		*/
		virtual void UpdateMove();

		/*!
		@brief 回転更新関数
		*/
		virtual void UpdateRotation();

		/*!
		@brief 回転先設定関数
		@param Lスティック入力量
		*/
		virtual void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief アクション時にインディケーターの方へ回転設定する関数
		*/
		void SetRotateIndicatorAngle();

		/*!
		@brief コントローラー移動関数
		@param Lスティック入力量
		*/
		virtual void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief 移動出来ないグリッド埋まり解除関数
		@param ポジション
		*/
		virtual void GridHitResponse(Vec3& pos);

		/*!
		@brief 歩いた時のSE開始関数
		*/
		virtual void StartWalkSoundEffect();

	private:

		/*!
		@brief 指定した行列が衝突判定を取るグリッドかの真偽取得関数
		@param row
		@param col
		@return bool 衝突判定を取るグリッドならtrue,取らないグリッドならfalse
		*/
		virtual bool GetIsImpassable(size_t row, size_t col);

		/*!
		@brief 三平方の定理でグリッドとプレイヤーの押し出しする関数
		@param pos
		@param gridX
		@param gridZ
		*/
		virtual void GridSquareTheorem(Vec3& pos, float gridX, float gridZ);

		/*!
		@brief 移動出来ないグリッド(上)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(下)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(左)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitLeftResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(右)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitRightResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(斜め)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitOliqueResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief ステージの範囲外へ行かない様にする関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridRangeResponse(Vec3& pos, const vector<vector<int>>& stageMap);
	};
}