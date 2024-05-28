/*!
@file CraftManager.h
@brief クラフト管理用マネージャ
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"
#include "CraftUI.h"
#include "CraftWindow.h"
#include "CraftItemIcon.h"
#include "CraftingIcon.h"
#include "CraftingQTE.h"

namespace basecross
{
	// クラフトアイテムenum
	enum class eCraftItem : size_t
	{
		Rail,		// レール
		WoodBridge,	// 木の橋
	};

	// クラフトCSVパラメータ
	enum class eCraftParam : size_t
	{
		ItemType,	 // 作るアイテムタイプ
		WoodValue,	 // 木の要求数
		StoneValue,	 // 石の要求数
		SuccesValue, // QTE成功時の作成量
		FailedValue, // QTE失敗時の作成量
	};

	// アイテムカウンタ用enum
	enum class eItemType : size_t
	{
		Wood,		// 木
		Stone,		// 石
		Gear,		// 歯車
		Rail,		// レール
		WoodBridge,	// 木の橋
	};

	/*!
	@brief クラフト管理クラス(プレイヤーとクラフト関連クラスの仲介クラス)
	*/
	class CraftManager
	{
		const weak_ptr<TemplateObject> m_player;
		const weak_ptr<CraftingQTE> m_craftQTE; // クラフトQTE
		const weak_ptr<CraftWindow> m_window;	// クラフトウィンドウ

		const vector<vector<string>> m_racipe; // クラフトレシピ

		vector<int> m_itemCount; // アイテム数
		eCraftItem m_craftItem;	// 作成中のアイテム
		eItemType m_craftType;	// 作成中のアイテム

	public:

		/*!
		@brief コンストラクタ
		@param ウィンドウオブジェクトのポインタ
		@param クラフトQTEオブジェクトのポインタ
		*/
		CraftManager(const shared_ptr<TemplateObject>& playerPtr,
			const shared_ptr<CraftWindow>& windowPtr,
			const shared_ptr<CraftingQTE>& qtePtr) :
			m_player(playerPtr),
			m_window(windowPtr),
			m_craftQTE(qtePtr),
			m_racipe(CSVLoader::LoadFile("CraftRacipe"))
		{
			m_itemCount = {0, 0, 0, 0, 0};
			m_craftItem = eCraftItem::Rail;
			m_craftType = eItemType::Rail;
		}

		/*!
		@brief デストラクタ
		*/
		~CraftManager() {}

		/*!
		@brief アイテムクラフト関数
		@param クラフトするアイテム
		@return クラフト可能かの真偽
		*/
		bool CraftOrder(eCraftItem item);

		/*!
		@brief クラフト有効化関数
		@param 有効にするかの真偽
		*/
		void CraftingEnabled(bool enable);

		/*!
		@brief QTE開始関数
		*/
		void StartQTE(eCraftItem item, eItemType type);

		/*!
		@brief QTE停止とQTE結果取得関数
		@param クラフトアイテム
		*/
		bool StopQTE();

		/*!
		@brief ウィンドウ描画が完了したかの真偽取得関数
		@return 描画完了してたらtrue、それ以外はfalse
		*/
		bool GetShowCraftWindow() const
		{
			return m_window.lock()->GetShowWindow();
		}

		/*!
		@brief QTEが終了したかの真偽取得関数
		@return 終了してたらtrue、それ以外はfalse
		*/
		bool GetEndedQTE() const
		{
			return !m_craftQTE.lock()->GetEnableQTE();
		}

		/*!
		@brief アイテム数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		int GetItemCount(eItemType type) const
		{
			return m_itemCount.at(static_cast<size_t>(type));
		}

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_itemCount.at(static_cast<size_t>(type)) += addNum;
		}

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void UseItem(eItemType type, int useNum = 1)
		{
			m_itemCount.at(static_cast<size_t>(type)) -= useNum;
		}

		/*!
		@brief クラフト要求素材数取得関数
		@param クラフトアイテム
		@param 素材タイプ
		@return 素材要求数
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param)
		{
			return stoi(m_racipe.at(static_cast<size_t>(item)).at(static_cast<size_t>(param)));
		}

		/*!
		@brief クラフト可能かの真偽取得関数
		@param 木の要求数
		@param 石の要求数
		@return 可能かの真偽
		*/
		bool GetCraftPossible(int woodValue, int stoneValue) const
		{
			return woodValue <= GetItemCount(eItemType::Wood) && stoneValue <= GetItemCount(eItemType::Stone);
		}
	};
}