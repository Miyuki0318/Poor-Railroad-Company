/*!
@file Player.h
@brief プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"
#include "SelectIndicator.h"
#include "CraftManager.h"

namespace basecross
{
	/*!
	@brief プレイヤーの状態
	*/
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

	/*!
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
		weak_ptr<SelectIndicator> m_indicator; // セレクトインディケーター
		shared_ptr<CraftManager> m_craft;      // クラフトマネージャー

		shared_ptr<PNTStaticDraw> m_ptrDraw;  // 描画コンポーネント
		shared_ptr<CollisionObb> m_ptrColl;   // コリジョンOBBコンポーネント
		Bool8_t<ePlayerStatus> m_status;	  // フラグ管理クラス

		const float m_speed; // 速度

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
		@brief Aボタン入力時に呼び出される関数
		*/
		void OnPushA();

		/*!
		@brief クラフト時に呼び出される関数
		*/
		void OnCraft();

		/*!
		@brief 採掘時に呼び出される関数
		@param 採掘されるオブジェクトのポインタ
		*/
		void OnMining(const shared_ptr<TemplateObject>& miningObj);

		/*!
		@brief レール設置時に呼び出される関数
		@param レールを設置する座標
		*/
		void OnRailed(const Vec3& railPosition);

		/*!
		@brief 採掘中の更新関数
		*/
		void UpdateMining();

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
		bool GetStatus(ePlayerStatus status)
		{
			return m_status(status);
		}
	};
}