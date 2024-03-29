/*!
@file Player.h
@brief プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"
#include "SelectIndicator.h"
#include "CraftManager.h"
#include "PlayerState.h"

namespace basecross
{
	// プレイヤーの状態
	enum class ePlayerStatus : uint8_t
	{
		IsIdle,		// 待機状態
		IsMove,		// 移動状態
		IsMining,	// 採掘状態
		IsHaveWood,	// 木を所持中
		IsHaveStone,// 石を所持中
		IsCrafting,	// クラフト中
		IsCraftQTE,	// クラフトQTE中
		IsHaveRail,	// 線路所持中
	};

	// プレイヤーの状態ステートクラス(名前用)
	class PlayerMovingState;	// 移動状態
	class PlayerMiningState;	// 採掘状態
	class PlayerCraftingState;	// クラフト状態

	/*!
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
		weak_ptr<SelectIndicator> m_indicator; // セレクトインディケーター
		unique_ptr<CraftManager> m_craft;      // クラフトマネージャー

		shared_ptr<PNTStaticDraw> m_ptrDraw;   // 描画コンポーネント
		shared_ptr<CollisionObb> m_ptrColl;    // コリジョンOBBコンポーネント
		Bool8_t<ePlayerStatus> m_status;	   // フラグ管理クラス

		// ステートマシン
		unique_ptr<PlayerStateMachine> m_stateMachine; 

		const float m_speed; // 速度

		// フレンド化(ステートマシンからメンバ関数を呼び出すため)
		friend PlayerMovingState;
		friend PlayerMiningState;
		friend PlayerCraftingState;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f, 1.5f, 1.0f)),
			m_speed(5.0f)
		{
			m_status = 0;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Player() 
		{
			m_craft.reset();
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
		@brief クラフト画面切り替え関数
		*/
		void SwitchCraftWindow();

		/*!
		@brief 採掘時に呼び出される関数
		@param 採掘されるオブジェクトのポインタ
		*/
		void MiningProcces(const shared_ptr<TemplateObject>& miningObj);

		/*!
		@brief レール設置時に呼び出される関数
		@param レールを設置する座標
		*/
		void AddRailed(const Vec3& railPosition);

		/*!
		@brief 採掘状態中の更新関数
		*/
		void MiningWaiting();

		/*!
		@brief クラフトQTEが終わっているかの確認関数
		*/
		void CheckedCraftQTE();

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove();

		/*!
		@brief アイテム状態の更新関数
		*/
		void UpdateItemStatus();

		/*!
		@brief コントローラー回転関数
		*/
		void ControllerRotation(const Vec3& stickValue);

		/*!
		@brief コントローラー移動関数
		*/
		void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief State変更関数
		@param 新しいステートのポインタ
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			m_stateMachine->SetState(newState);
		}
			
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
		@brief 状態取得関数
		@param プレイヤーの状態enum
		@return その状態になっているかの真偽
		*/
		bool GetStatus(ePlayerStatus status) const
		{
			return m_status(status);
		}
	};
}