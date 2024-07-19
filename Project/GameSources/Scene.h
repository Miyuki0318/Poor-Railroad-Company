/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

#define MONEY_LIMIT 99999

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

	// Player Level
	enum class ePL : char
	{
		Level1,
		Level2,
		Level3,
		Level4,
		Level5,
	};

	class Scene : public SceneBase{
	private:
		eMapLevel m_mapLevel = eMapLevel::FirstMap;
		map<eMapLevel, string> m_stagePathMap;
		map<eMapLevel, eMapLevel> m_nextLevel;

		int m_money;

		// 各種レベル
		ePL m_statusLevel;
		ePL m_limitChoicesLevel;
		ePL m_startGearLevel;

	public:
		const wstring m_objTagName = L"SELECT";
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase()
		{
			m_stagePathMap.emplace(eMapLevel::FirstMap, "Lv1");
			m_stagePathMap.emplace(eMapLevel::SecondMap, "Lv2");
			m_stagePathMap.emplace(eMapLevel::ThirdMap, "Third");
			m_stagePathMap.emplace(eMapLevel::FourthMap, "Fourth");
			m_stagePathMap.emplace(eMapLevel::FifthMap, "Fifth");

			m_nextLevel.emplace(eMapLevel::FirstMap, eMapLevel::SecondMap);
			m_nextLevel.emplace(eMapLevel::SecondMap, eMapLevel::ThirdMap);
			m_nextLevel.emplace(eMapLevel::ThirdMap, eMapLevel::FourthMap);
			m_nextLevel.emplace(eMapLevel::FourthMap, eMapLevel::FifthMap);
			m_nextLevel.emplace(eMapLevel::FifthMap, eMapLevel::FirstMap);

			m_money = 5000;

			m_statusLevel = ePL::Level1;
			m_limitChoicesLevel = ePL::Level1;
			m_startGearLevel = ePL::Level1;
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
		@brief	マップ難易度を次のステージにしてファイル名を返す関数
		@return 次のステージのパス
		*/
		string ToNextStage()
		{
			m_mapLevel = m_nextLevel.at(m_mapLevel);
			return m_stagePathMap.at(m_mapLevel);
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
		int GetMoney() const
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

		/*!
		@brief 採掘/移動速度強化レベルの設定
		@param[in] 設定する採掘/移動速度強化レベルの値
		*/
		void SetStatusLevel(ePL level)
		{
			m_statusLevel = level;
		}

		/*!
		@brief 採掘/移動速度強化レベルの取得
		@return 取得する採掘/移動速度強化レベルの値
		*/
		ePL GetStatusLevel() const
		{
			return m_statusLevel;
		}

		/*!
		@brief 資材所持上限レベルの設定
		@param[in] 設定する資材所持上限レベルの値
		*/
		void SetLimitChoicesLevel(ePL level)
		{
			m_limitChoicesLevel = level;
		}

		/*!
		@brief 資材所持上限レベルの取得
		@return 取得する資材所持上限レベルの値
		*/
		ePL GetLimitChoicesLevel() const
		{
			return m_limitChoicesLevel;
		}

		/*!
		@brief 開始時所持ギアレベルの設定
		@param[in] 設定する開始時所持ギアレベルの値
		*/
		void SetStartGearLevel(ePL level)
		{
			m_startGearLevel = level;
		}

		/*!
		@brief 開始時所持ギアレベルの取得
		@return 取得する開始時所持ギアレベルの値
		*/
		ePL GetStartGearLevel() const
		{
			return m_startGearLevel;
		}
	};
}