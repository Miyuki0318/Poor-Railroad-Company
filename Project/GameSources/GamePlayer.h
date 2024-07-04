/*!
@file GamePlayer.h
@brief ゲーム中のプレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "Player.h"
#include "ItemFly.h"
#include "GamePlayerStateMachine.h"
#include "GameStage.h"

namespace basecross
{
	// プレイヤーの状態ステートクラス(名前のみ宣言)
	class GamePlayerIdleState;		// 待機状態
	class GamePlayerMovingState;	// 移動状態
	class GamePlayerGatheringState;	// 採掘状態
	class GamePlayerCraftingState;	// クラフト状態
	class GamePlayerSuccesState;	// クラフト状態
	class GamePlayerFailedState;	// クラフト状態

	/*!
	@brief ゲーム中のプレイヤー
	*/
	class GamePlayer : public Player
	{
		weak_ptr<FlyItemManager> m_itemFly;	// アイテムが飛んでいくエフェクト
		unique_ptr<CraftManager> m_craft;	// クラフトマネージャー

		// ステートマシン
		unique_ptr<GamePlayerStateMachine> m_playerState;

		// ステータスに応じたアイテムタイプ用マップ
		map<eItemType, ePST> m_itemLimitMap;

		Vec3 m_startPosition;			// 開始時の座標
		Vec3 m_goalStagingPosition;		// ゴール演出時の座標
		Vec3 m_goalPosition;			// ゴール時の座標

		// フレンド化(ステートマシンからメンバ関数を呼び出すため)
		friend GamePlayerIdleState;
		friend GamePlayerMovingState;
		friend GamePlayerGatheringState;
		friend GamePlayerCraftingState;
		friend GamePlayerSuccesState;
		friend GamePlayerFailedState;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GamePlayer(const shared_ptr<Stage>& stagePtr, 
			const Vec3& start, const Vec3& goal) :
			Player(stagePtr),
			m_startPosition(start),
			m_goalStagingPosition(goal)
		{
			m_itemLimitMap.emplace(eItemType::Wood, ePST::WoodMax);
			m_itemLimitMap.emplace(eItemType::Stone, ePST::StoneMax);
			m_itemLimitMap.emplace(eItemType::Gear, ePST::GearMax);
			m_itemLimitMap.emplace(eItemType::Rail, ePST::RailMax);
			m_itemLimitMap.emplace(eItemType::WoodBridge, ePST::BridgeMax);
			m_itemLimitMap.emplace(eItemType::Crossing, ePST::CrossingMax);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GamePlayer()
		{
			// 機能の解放
			m_craft.reset();
			m_playerState.reset();
		}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief リセット処理関数
		*/
		void ResetPlayer(const Vec3& start, const Vec3& goal);

	private:

		/*!
		@brief プレイヤーに付加する機能生成関数
		*/
		void CreatePlayerFeatures() override;

		/*!
		@brief インディケーターへの取得と呼び出し関数
		*/
		void IndicatorOrder() override;

		/*!
		@brief レール追加命令関数
		@param インディケーターのポインタ
		@return 設置できたか
		*/
		bool AddRailOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief 木の足場追加命令関数
		@param インディケーターのポインタ
		@return 設置できたか
		*/
		bool AddBridgeOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief 踏切追加命令関数
		@param インディケーターのポインタ
		@return 設置できたか
		*/
		bool AddCrossingOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief クラフト画面切り替え関数
		*/
		void SwitchCraftWindow();

		/*!
		@brief アイテム状態の更新関数
		*/
		void UpdateStatus();

		/*!
		@brief アイテム数の更新関数
		*/
		void UpdateItemCount();

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove() override;

		/*!
		@brief クラフト状態初期化関数
		*/
		void ResetCrafting();

		/*!
		@brief State変更関数
		@param 新しいステートのポインタ
		*/
		void SetState(const shared_ptr<GamePlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}

	public:

		/*!
		@brief クラフトマネージャーの取得関数
		@param m_craft.get()
		*/
		const CraftManager* GetCraftManager() const
		{
			return m_craft.get();
		}

		/*!
		@brief ゲーム結果時用のステート設定関数
		@param eGameProgress
		*/
		void SetGameResult(eGameProgress result);

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void AddItemCount(eItemType type, int addNum = 1) override
		{
			m_craft->AddItemCount(type, addNum);
		}

		/*!
		@brief アイテム数追加しフライエフェクトを出す関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void AddItemCountFly(eItemType type, int addNum = 1)
		{
			AddItemCount(type, addNum);
			m_itemFly.lock()->StartFly(type);
		}

		/*!
		@brief アイテム数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		int GetItemCount(eItemType type) override
		{
			return m_craft->GetItemCount(type);
		}

		/*!
		@brief アイテム上限数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		int GetLimitCount(eItemType type) override
		{
			if (m_itemLimitMap.find(type) == m_itemLimitMap.end()) return 99;
			return (int)m_playerData.at(m_itemLimitMap.at(type)).at(m_backPackLevel);
		}

		/*!
		@brief クラフトできるの取得関数
		@param クラフトアイテムenum
		@return クラフトできるかの真偽
		*/
		bool GetCraftPosshible() const override
		{
			bool rail = m_craft->CraftOrder(eCraftItem::Rail);
			bool bridge = m_craft->CraftOrder(eCraftItem::WoodBridge);
			bool crossing = m_craft->CraftOrder(eCraftItem::Crossing);

			return rail || bridge || crossing;
		}

		/*!
		@brief コントローラーで向けた方向取得関数
		@return atan2f(m_rotTarget.z, m_rotTarget.x)
		*/
		float GetPastRotTarget() const
		{
			return atan2f(m_rotTarget.z, m_rotTarget.x);
		}
	};
}