/*!
@file GamePlayer.h
@brief ゲーム中のプレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "Player.h"
#include "SelectIndicator.h"
#include "CraftManager.h"
#include "PlayerState.h"

namespace basecross
{
	// プレイヤーの状態
	enum class ePlayerStatus : uint16_t
	{
		IsIdle,		// 待機状態
		IsMove,		// 移動状態
		IsRotate,	// 回転状態
		IsMining,	// 採掘状態
		IsHaveWood,	// 木を所持中
		IsHaveStone,// 石を所持中
		IsCrafting,	// クラフト中
		IsCraftQTE,	// クラフトQTE中
		IsHaveRail,	// 線路所持中
	};

	// プレイヤーの状態ステートクラス(名前のみ宣言)
	class PlayerIdleState;		// 待機状態
	class PlayerMovingState;	// 移動状態
	class PlayerMiningState;	// 採掘状態
	class PlayerCraftingState;	// クラフト状態

	/*!
	@brief ゲーム中のプレイヤー
	*/
	class GamePlayer : public Player
	{
		weak_ptr<SelectIndicator> m_indicator; // セレクトインディケーター
		unique_ptr<CraftManager> m_craft; // クラフトマネージャー

		map<wstring, eItemType> m_miningMap; // 採掘対象と取得アイテムタイプ
		Bool16_t<ePlayerStatus> m_status; // フラグ管理クラス

		// ステートマシン
		unique_ptr<PlayerStateMachine> m_playerState;

		// フレンド化(ステートマシンからメンバ関数を呼び出すため)
		friend PlayerIdleState;
		friend PlayerMovingState;
		friend PlayerMiningState;
		friend PlayerCraftingState;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GamePlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{
			m_status = 0; // 状態フラグは0で初期化

			// 採掘オブジェクトのタグと採掘時に加算するアイテムのタイプ
			m_miningMap.insert(make_pair(L"TREE", eItemType::Wood));	// タグか木ならアイテムタイプは木材
			m_miningMap.insert(make_pair(L"ROCK", eItemType::Stone)); // タグが岩ならアイテムタイプは石材

			// 移動不可なIDの登録
			m_impassableSet.insert(eStageID::Rail);
			m_impassableSet.insert(eStageID::DeRail);
			m_impassableSet.insert(eStageID::GoalRail);
			m_impassableSet.insert(eStageID::Tree);
			m_impassableSet.insert(eStageID::Stone);
			m_impassableSet.insert(eStageID::UnBreakRock);
			m_impassableSet.insert(eStageID::Water);

			m_impassableSet.insert(eStageID::Air);
			m_impassableSet.insert(eStageID::UnGrass);
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

	private:

		/*!
		@brief コンポーネントの生成関数
		*/
		void CreateComponent() override;

		/*!
		@brief プレイヤーに付加する機能生成関数
		*/
		void CreatePlayerFeatures();

		/*!
		@brief インディケーターへの取得と呼び出し関数
		*/
		void IndicatorOrder();

		/*!
		@brief 採掘命令関数
		@param インディケーターのポインタ
		@return 採掘できたか
		*/
		bool MiningOrder(const shared_ptr<SelectIndicator>& indicator);

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
		@brief 採掘時に呼び出される関数
		@param 採掘されるオブジェクトのタグ
		*/
		void MiningProcces(const set<wstring>& tagSet);

		/*!
		@brief クラフト画面切り替え関数
		*/
		void SwitchCraftWindow();

		/*!
		@brief アイテム状態の更新関数
		*/
		void UpdateStatus();

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove() override;

		/*!
		@brief State変更関数
		@param 新しいステートのポインタ
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_craft->AddItemCount(type, addNum);
		}

	public:

		/*!
		@brief アイテム数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		int GetItemCount(eItemType type)
		{
			return m_craft->GetItemCount(type);
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
		@brief クラフトできるの取得関数
		@param クラフトアイテムenum
		@return クラフトできるかの真偽
		*/
		bool GetCraftPosshible() const
		{
			bool rail = m_craft->CraftOrder(eCraftItem::Rail);
			bool bridge = m_craft->CraftOrder(eCraftItem::WoodBridge);

			return rail || bridge;
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