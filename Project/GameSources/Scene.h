/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{
	// マップの難易度
	enum class eMapLevel
	{
		FirstMap,
		SecondMap,
		ThirdMap,
		FourthMap,
		FifthMap
	};

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	private:
		eMapLevel m_mapLevel = eMapLevel::FirstMap;
		map<eMapLevel, string> m_stagePathMap;

		int m_money;
	public:
		const wstring m_objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase()
		{
			m_stagePathMap.emplace(eMapLevel::FirstMap, "First");
			m_stagePathMap.emplace(eMapLevel::SecondMap, "Second");
			m_stagePathMap.emplace(eMapLevel::ThirdMap, "Third");
			m_stagePathMap.emplace(eMapLevel::FourthMap, "Fourth");
			m_stagePathMap.emplace(eMapLevel::FifthMap, "Fifth");

			m_money = 0;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの読み込み
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CreateResourses();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シングルモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);

		/*!
		@brief	マップの難易度を取得するゲット関数
		*/
		eMapLevel GetMapLevel() const
		{
			return m_mapLevel;
		}

		/*!
		@brief	マップの難易度を設定するセット関数
		*/
		void SetMapLevel(eMapLevel level)
		{
			m_mapLevel = level;
		}


		/*!
		@brief	タグ名取得
		*/
		wstring GetTagName()
		{
			return m_objTagName;
		}

		/*!
		@brief お金の取得
		@return m_money
		*/
		int GetMoney()
		{
			return m_money;
		}

		/*!
		@brief お金の設定
		@param[in] 設定するお金の値
		*/
		void SetMoney(int money)
		{
			m_money = money;
		}
	};
}