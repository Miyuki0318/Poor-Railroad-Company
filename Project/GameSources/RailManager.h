/*!
@file RailManager.h
@brief レール管理クラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

#define ROW(posZ) size_t(-posZ)	// 行
#define COL(posX) size_t(posX) // 列
#define LINE(row, col) to_string(row) + "-" + to_string(col)	// row-col
#define LINE2POS(row, col) Vec3(float(col), 1.0f, -float(row))	// rowとcolから座標に変換
#define POS2LINE(pos) LINE(ROW(pos.z), COL(pos.x))	// 座標からrow-col文字列に変換

namespace basecross
{
	// CSVチェック用向きenum
	enum class eNextElemDir
	{
		DirFlont,	// 前
		DirBack,	// 後
		DirLeft,	// 左
		DirRight,	// 右
	};

	// CSVのチェック用構造体
	struct CSVElementCheck
	{
		eNextElemDir dir = eNextElemDir::DirFlont;
		size_t row = 0;	// 行
		size_t col = 0; // 列
		bool isRange = false; // 配列の範囲内かの真偽

		/*!
		@brief コンストラクタ
		@param row
		@param col
		@param isRange
		*/
		CSVElementCheck(eNextElemDir railDir, size_t elemRow, size_t elemCol, bool range) :
			dir(railDir),
			row(elemRow),
			col(elemCol),
			isRange(range)
		{
		}

		/*!
		@brief ポイントの前後左右が配列の範囲内かを取得する関数
		@param row
		@param col
		@param csv配列
		*/
		static vector<CSVElementCheck> GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap)
		{
			vector<CSVElementCheck> elems; // 前後左右の結果保存用配列
			if (csvMap.empty()) return elems; // 参照する配列が空なら空を返す

			elems = {
				{eNextElemDir::DirFlont ,row - 1, col, Utility::WithInElemRange(row - 1, csvMap.size())},
				{eNextElemDir::DirBack ,row + 1, col, Utility::WithInElemRange(row + 1, csvMap.size())},
				{eNextElemDir::DirLeft ,row, col - 1, Utility::WithInElemRange(col - 1, csvMap.at(row).size())},
				{eNextElemDir::DirRight ,row, col + 1, Utility::WithInElemRange(col - 1, csvMap.at(row).size())},
			};

			return elems;
		}
	};

	// レールのタイプ
	enum class eRailType
	{
		AxisXLine,		// 左右の直線
		AxisZLine,		// 上下の直線
		Right2Under,	// 右から下へ
		Right2Upper,	// 右から上へ
		Left2Under,		// 左から下へ
		Left2Upper,		// 左から上へ
	};

	// レールの行く方向
	enum class eRailAngle
	{
		Straight,	// 直線
		Right,		// 右に曲がる
		Left,		// 左に曲がる
	};

	// レールの情報構造体
	struct RailData
	{
		Vec3 thisPos;		// 自身の座標
		Vec3 pastPos;		// 一個前の座標
		Vec3 futurePos;		// 一個先の座標
		eRailType type;		// レールのタイプ
		eRailAngle angle;	// レールの行く方向

		RailData() 
		{
			thisPos.zero();
			pastPos.zero();
			type = eRailType::AxisXLine;
			angle = eRailAngle::Straight;
		}

		RailData(const Vec3& tPos, const Vec3& pPos) :
			thisPos(tPos),
			pastPos(pPos)
		{
			type = eRailType::AxisXLine;
			angle = eRailAngle::Straight;
		}
	};

	/*!
	@brief LINEマクロで保持されたRowCol文字列をsize_tに代入する関数
	@param row格納変数
	@param col格納変数
	@param line文字列
	*/
	void GetLineStringToRowCol(size_t& row, size_t& col, string line);

	/*!
	@brief レール管理用クラス
	*/
	class RailManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // 描画コンポーネント
		vector<Point2D<size_t>> m_guidePoints; // ガイドがあるポイント
		vector<vector<int>> m_guideMap; // ガイド付きのステージcsv
		Mat4x4 m_mtxScale;		// インスタンス描画用のスケール
		Mat4x4 m_mtxRotation;	// インスタンス描画用のローテーション
		size_t m_railNum;		// レールの設置数
		map<string, RailData> m_railDataMap;
		string m_pastLine;
		Vec3 m_pastDeRailPos;
		
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_railNum = 0;

			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(0.675f));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~RailManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief レール追加関数
		@param 選択ポイント
		*/
		void AddRail(const Point2D<size_t>& point);

		/*!
		@brief 現在のポイントの位置がガイドになっているか取得関数
		@param 選択ポイント
		*/
		bool GetIsGuidePoint(const Point2D<size_t>& point) const;

		/*!
		@brief ガイドを再計算する関数
		*/
		void GuideRecalculation();

		/*!
		@brief ガイドのポイント取得関数
		@return m_guidePoints
		*/
		const vector<Point2D<size_t>>& GetGuidePoints() const
		{
			return m_guidePoints;
		}

		/*!
		@brief レールの座標配列取得関数
		@return m_railPositions
		*/
		const map<string, RailData>& GetRailDataMap() const
		{
			return m_railDataMap;
		}

		/*!
		@brief ガイドつきcsv取得関数
		@return m_guideMap
		*/
		const vector<vector<int>>& GetGuideMap() const
		{
			return m_guideMap;
		}

		/*!
		@brief レールの設置数取得関数名
		@return m_railNum
		*/
		size_t GetAddRailNum() const
		{
			return m_railNum;
		}

	private:

		/*!
		@brief インスタンス描画のレール追加関数
		@param row
		@param col
		*/
		void AddInstanceRail(size_t row, size_t col);

		/*!
		@brief レールデータ追加関数
		@param row
		@param col
		*/
		void AddRailDataMap(size_t row, size_t col);

		/*!
		@brief CSVをレールIDに書き換える関数
		@param row
		@param col
		*/
		void SetRailID(size_t row, size_t col) const;

		/*!
		@brief CSVをガイドIDに書き換える関数
		@param row
		@param col
		*/
		void SetGuideID(size_t row, size_t col);

		/*!
		@brief CSVをガイドIDに追加する関数
		@param 追加するrow
		@param 追加するcol
		*/
		void AddGuideID(size_t row, size_t col);
	};
}