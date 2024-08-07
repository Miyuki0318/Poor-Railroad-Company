/*!
@file OzawaUtility.h
@brief ユーティリティ
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h") // BaseCrossのライブラリをインクルードしているかのマクロ

namespace Utility
{
	/*!
	@brief 線形補間を行う関数
	@param (start) 線形補間の始点の値
	@param (end) 線形補間の終点の値
	@param (time) 線形補間の割合
	@return 割合に応じて補間された値
	*/
	template <class T>
	inline T Lerp(const T& start, const T& end, float time)
	{
		time = time < 0.0f ? 0.0f : time;
		time = time > 1.0f ? 1.0f : time;
		return start + (end - start) * time;
	}

	/*!
	@brief 円弧補間用の線形補間関数
	@param (start) 線形補間の始点の値
	@param (end) 線形補間の終点の値
	@param (time) 線形補間の割合
	@return 割合に応じて補間された値
	*/
	template <class T>
	inline T SLerp(const T& start, const T& end, float time)
	{
		return start * (1.0f - time) + end * time;
	}

	/*!
	@brief 円弧補間を行う関数
	@param (pointA) 補間の始点の値
	@param (pointB) 補間の中間の値
	@param (pointC) 補間の終点の値
	@param (time) 補間の割合
	@return 割合に応じて補間された値
	*/
	template <class T>
	inline T BezierLerp(const T& pointA, const T& pointB, const T& pointC, float time)
	{
		T pA2B = SLerp(pointA, pointB, time);
		T pB2C = SLerp(pointB, pointC, time);
		return SLerp(pA2B, pB2C, time);
	}

	/*!
	@brief 範囲を指定したサインカーブを生成
	@param (time) 経過時間
	@param (min) カーブの下限値
	@param (max) カーブの上限値
	@return 範囲内で生成されたサインカーブ
	*/
	template <class T>
	inline T SinCurve(const float& time, const T& min, const T& max)
	{
		T length = max - min;
		T helfLength = length / 2.0f;
		T curve = sin(time) * helfLength + helfLength;
		return curve + min;
	}

	/*!
	@brief 小数点以下の桁数を返す関数
	@param (value)小数の桁数を調べたい値
	@return 小数点以下の桁数をint型で返す
	*/
	template <class T>
	int GetLowerDigit(const T& value)
	{
		// float/double型以外の場合は0を返して関数を終了する
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double))
		{
			return 0;
		}

		// 文字列で小数点を精査する為、文字列型の変数に変換
		wstring str = to_wstring(value);

		// 計測用カウンタ
		int integer = 0;
		int decimal = 0;
		int zeroCount = -1;
		bool errorCheck = false;

		// 文字列の長さ文ループ
		for (int i = 0; i < str.size(); i++)
		{
			// 小数点が見つかったら
			if (str[i] == '.')
			{
				// 配列の要素数を保持してfor文を停止
				integer = i;
				break;
			}
		}

		// 文字列の長さ文ループ
		for (int i = integer; i < str.size(); i++)
		{
			if (str[i] == '0')
			{
				errorCheck = true;
				zeroCount++;
			}

			if (str[i] != '.' && str[i] != '0')
			{
				errorCheck = false;
				zeroCount = 0;
			}
		}

		// 小数点が見つからなかったら
		if (integer != 0)
		{
			// 小数点以下の桁数を返すカウンタを計算する
			if (errorCheck)
			{
				return decimal = static_cast<int>(str.size()) - integer - zeroCount - 1;
			}

			return decimal = static_cast<int>(str.size()) - integer - 1;
		}

		// 小数点以下を返すカウンタは0にする
		return decimal = 1;
	}

	/*!
	@brief 整数の桁数を返す関数
	@param (value)整数の桁数を調べたい値
	@return 整数のの桁数をint型で返す
	*/
	template <class T>
	inline int GetUpperDigit(const T& value)
	{
		// float/double型以外の場合は0を返して関数を終了する
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double) && typeid(value) != typeid(int))
		{
			return 0;
		}

		// 文字列で小数点を精査する為、文字列型の変数に変換
		wstringstream stream;
		stream << value;
		wstring str = stream.str();

		// 計測用カウンタ
		int integer = 0;

		// 文字列の長さ文ループ
		for (int i = 0; i < str.size(); i++)
		{
			// 小数点が見つかったらfor文を停止
			if (str[i] == '.' || (integer == 0 && str[i] == '0')) break;
			integer++;
		}

		// 小数点以上の桁数を返す
		return integer;
	}

	/*!
	@brief 範囲を指定した乱数を生成(小数点以下も対応)
	@param (max) 乱数の上限値
	@param (min) 乱数の下限値
	@param (digit) 小数点以下の桁数指定
	@return 範囲内で生成された乱数
	*/
	template <class T>
	T RangeRand(const T& min, const T& max, int digit)
	{
		T r = min + static_cast<T>(rand()) * static_cast<T>(max) / RAND_MAX;
		return static_cast<T>(basecross::bsm::floor(static_cast<float>(r), digit));
	}

	/*!
	@brief 範囲を指定した乱数を生成(小数点以下も対応)
	@param (max) 乱数の上限値
	@param (min) 乱数の下限値
	@return 範囲内で生成された乱数
	*/
	template <class T>
	T RangeRand(const T& max, const T& min)
	{
		// 浮動小数型の場合は小数の桁数を計算する
		int maxDigit = GetLowerDigit(max);
		int minDigit = GetLowerDigit(min);
		int digit = maxDigit > minDigit ? maxDigit : minDigit;

		return RangeRand(min, max, digit);
	}

	/*!
	@brief ディグリー角をラジアン角に変換
	@param (deg) ディグリー角
	@return ラジアン角に変換されたディグリー角
	*/
	template <class T>
	T DegToRad(const T& deg)
	{
		return deg * (3.141592654f / 180.0f);
	}

	/*!
	@brief ラジアン角をディグリー角に変換
	@param (rad) ラジアン角
	@return ディグリー角に変換されたラジアン角
	*/
	template <class T>
	T RadToDeg(const T& rad)
	{
		return rad * (180.0f / 3.141592654f);
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(int value, int a, int b);

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(float value, float a, float b);

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	template<class Ty1, class Ty2, class Ty3>
	bool GetBetween(Ty1 value, Ty2 a, Ty3 b)
	{
		return GetBetween(static_cast<int>(value), static_cast<int>(a), static_cast<int>(b));
	}

	/*!
	@brief 列挙型の値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	template <class T>
	bool GetBetween(T value, T a, T b)
	{
		// 列挙型かどうかのチェック
		if (is_enum<T>())
		{
			// 列挙型なのでint型にキャストする
			int iv, ia, ib;
			iv = static_cast<int>(value);
			ia = static_cast<int>(a);
			ib = static_cast<int>(b);

			// int型でのGetBetween()を実行する
			return GetBetween(iv, ia, ib);
		}
		else
		{
#if BASECROSS

			// エラーメッセージを送る
			throw basecross::BaseException(
				L"列挙型以外は指定できません",
				L"列挙型が指定されてるか確認してください",
				L"bool GetBetween(T value, T a, T b)"
			);
#endif
			return false;
		}
	}

	/*!
	@brief インデックスが配列の範囲内かを返す
	@param (index)　確認する値
	@param (size)　配列の.size()
	@return 範囲内かの真偽
	*/
	template<class T>
	bool WithInElemRange(T index, size_t size)
	{
		return (0 <= static_cast<size_t>(index) && static_cast<size_t>(index) < size);
	}

	/*!
	@brief 指定した列と行が配列の範囲内かを返す
	@param (row)　確認する列
	@param (col)　確認する行
	@param (elem)　確認する配列
	@return 範囲内かの真偽
	*/
	template<class Ty1, class Ty2, class Ty3>
	bool WithInElemRange(Ty1 row, Ty2 col, Ty3 elem)
	{
		return WithInElemRange(row, elem.size()) && WithInElemRange(col, elem.at(row).size());
	}

	/*!
	@brief enum列挙子をsize_tにキャスト
	@return キャストされたenum
	*/
	template <typename Enum>
	const size_t toSize_t(Enum index)
	{
		// 列挙型じゃなければ
		if (!is_enum<Enum>())
		{
#if BASECROSS

			// エラーメッセージを送る
			throw basecross::BaseException(
				L"enum以外をsize_tにキャストできません",
				L"enumを指定してください",
				L"toSize_t(Enum index)"
			);
#endif
			return 0;
		}

		// size_tにキャストして返す
		return static_cast<size_t>(index);
	}

	/*!
	@brief 第一引数の値に一番近い値を返す (中間値がある場合は最短で出た値が優先される)
	@param (num) 元の数
	@param (a...) 比べる数(可変長引数)
	@return numに一番近い数値
	*/
	template <class T>
	T GetClosest(const T& num, vector<T> nums)
	{
		size_t size = nums.size();
		size_t elem = 0;
		T ret = (numeric_limits<T>::max)();

		for (size_t i = 0; i < size; i++)
		{
			T value = num - nums.at(i);
			if (value < 0) value *= -1;

			if (ret > value)
			{
				ret = value;
				elem = i;
			}
		}

		return nums.at(elem);
	}

	/*!
	@brief 第一引数の値に一番近い値を返す (中間値がある場合は最短で出た値が優先される)
	@param (num) 元の数
	@param (a...) 比べる数(可変長引数)
	@return numに一番近い数値
	*/
	template <class Ty1, class... Ty2>
	Ty1 GetClosest(const Ty1& num, Ty2... a)
	{
		size_t size = sizeof...(a);
		size_t elem = 0;
		Ty1 ret = (numeric_limits<Ty1>::max)();
		vector<Ty1> nums = { static_cast<Ty1>(a)... };

		for (size_t i = 0; i < size; i++)
		{
			Ty1 value = num - nums.at(i);
			if (value < 0) value *= -1;

			if (ret > value)
			{
				ret = value;
				elem = i;
			}
		}

		return nums.at(elem);
	}

	/*!
	@brief or演算
	@param 比較元
	@param 比べる値１
	@param 比べる値２
	@return どちらかと一致しているか
	*/
	template <class T>
	bool OR(T val, T a, T b)
	{
		return (val == a || val == b);
	}

	/*!
	@brief or演算
	@param 比較元
	@param 比べる値(可変長引数)
	@return どれかと一致しているか
	*/
	template <class T, class... V>
	bool ORS(T val, V... nums)
	{
		// 可変長引数の数だけループさせる
		for (T num : { static_cast<T>(nums)... })
		{
			if (num == val) return true;
		}

		return false;
	}

	/*!
	@brief アルファベット大文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	string ToUpperString(string str);

	/*!
	@brief アルファベット大文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	wstring ToUpperString(wstring str);

	/*!
	@brief アルファベット小文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	string ToLowerString(string str);

	/*!
	@brief アルファベット小文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	wstring ToLowerString(wstring str);

#if BASECROSS

	/*!
	@brief 無色透明
	 */
#define COL_NONE	Col4(0.0f)

	 /*!
	 @brief 有色透明
	 */
#define COL_ALPHA	Col4(1.0f,1.0f,1.0f,0.0f)

	 /*!
	 @brief 白色
	 */
#define COL_WHITE	Col4(1.0f)

	 /*!
	 @brief 黒色
	 */
#define COL_BLACK	Col4(0.0f,0.0f,0.0f,1.0f)

	 /*!
	 @brief 赤色
	 */
#define COL_RED		Col4(1.0f,0.0f,0.0f,1.0f)

	 /*!
	 @brief 緑色
	 */
#define COL_GREAN	Col4(0.0f, 1.0f, 0.0f, 1.0f)

	 /*!
	 @brief 青色
	 */
#define COL_BLUE	Col4(0.0f,0.0f,1.0f,1.0f)

	 /*!
	 @brief 黄色
	 */
#define COL_YELOW	Col4(1.0f, 1.0f, 0.0f, 1.0f)

	 /*!
	 @brief 青緑色
	 */
#define COL_BG		Col4(0.0f, 1.0f, 1.0f, 1.0f)

	 /*!
	 @brief 灰色
	 */
#define COL_GRAY	Col4(0.5f, 0.5f, 0.5f, 1.0f)

	 /*!
	 @brief デルタタイム
	 @return App::GetApp()->GetElapsedTime()
	 */
#define DELTA_TIME basecross::App::GetApp()->GetElapsedTime()

	 /*!
	 @brief ウィンドウの幅
	 @return App::GetApp()->GetGameWidth()
	 */
#define WINDOW_WIDTH static_cast<float>(basecross::App::GetApp()->GetGameWidth())

	 /*!
	 @brief ウィンドウの高さ
	 @return App::GetApp()->GetGameHeight()
	 */
#define WINDOW_HEIGHT static_cast<float>(basecross::App::GetApp()->GetGameHeight())

	 /*!
	 @brief ウィンドウのサイズ
	 @return Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)
	 */
#define WINDOW_SIZE Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)

	 /*!
	 @brief Thisポインタ
	 @return static_cast<void*>(ObjectInterface::shared_from_this().get())
	 */
#define ThisPtr static_cast<void*>(shared_from_this().get())

	 /*!
	 @brief 上方向Vec3
	 @return Vec3(0.0f, 1.0f, 0.0f)
	 */
#define UP_VEC		Vec3(0.0f, 1.0f, 0.0f)

	 /*!
	 @brief 下方向Vec3
	 @return Vec3(0.0f, -1.0f, 0.0f)
	 */
#define DOWN_VEC	Vec3(0.0f, -1.0f, 0.0f)

	 /*!
	 @brief 左方向Vec3
	 @return Vec3(-1.0f, 0.0f, 0.0f)
	 */
#define LEFT_VEC	Vec3(-1.0f, 0.0f, 0.0f)

	 /*!
	 @brief 右方向Vec3
	 @return Vec3(1.0f, 0.0f, 0.0f)
	 */
#define RIGHT_VEC	Vec3(1.0f, 0.0f, 0.0f)

	 /*!
	 @brief 前方向Vec3
	 @return Vec3(0.0f, 0.0f, -1.0f)
	 */
#define FRONT_VEC	Vec3(0.0f, 0.0f, -1.0f)

	 /*!
	 @brief 後方向Vec3
	 @return Vec3(0.0f, 0.0f, 1.0f)
	 */
#define BACK_VEC	Vec3(0.0f, 0.0f, 1.0f)

	 /*!
	 @brief 2D座標z軸加算Vec3
	 @return Vec3(0.0f, 0.0f, 0.1f)
	 */
#define BACK_LAYER	Vec3(0.0f, 0.0f, 0.1f)

	/*!
	@brief シンプルな頂点データを作成
	@param (vertices) 頂点データ
	*/
	void SimpleVertices(vector<basecross::VertexPositionColorTexture>& vertices, const Col4& color = COL_WHITE);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color);

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number);

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (number) 数字
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number);

	/*!
	@brief リボン用頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (axisVec) 軸ベクトル
	@param (weight) 幅のサイズ
	@param (texLoop) テクスチャのループ数
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop);

	/*!
	@brief リボン用頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (axisVec) 軸ベクトル
	@param (weight) 幅のサイズ
	@param (texLoop) テクスチャのループ数
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop);

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int squareNum);

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum);

	/*!
	@brief ワールド座標をスクリーン座標に変換(BaseCrossの座標用、それ以外はDirectX付属の関数を備考)
	@param (viewPtr) viewのshared_ptr
	@param (position) ワールド座標
	@return 変換されたスクリーン座標
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position);

	/*!
	@brief ビルボードをクォータニオンで作成し返す
	@param (Line)　カメラの注視点 - カメラの位置
	@return 作成されたクォータニオン
	*/
	Quat GetBillboardQuat(const Vec3& Line);

	/*!
	@brief ビルボード状態から回転させたクォータニオンで作成し返す
	@param (cameraPos)　カメラの位置
	@param (objPos)　オブジェクトの位置
	@param (rotateVec)　回転量(Vec3)
	@return 作成されたクォータニオン
	*/
	Quat GetBillboardRotateQuat(const Vec3& Line, Vec3& rotateVec);

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b);
	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b);

	float rotYatan2f(const Vec3& A, const Vec3& B);

	namespace CohenClip 
	{
		const int INSIDE = 0; // 0000
		const int LEFT = 1;   // 0001
		const int RIGHT = 2;  // 0010
		const int BOTTOM = 4; // 0100
		const int TOP = 8;    // 1000

		// 座標のコード計算
		int ComputeOutCode(float x, float y);
	}

	/*!
	@brief 元の位置とターゲットで線分を作り、画面内の座標にクリッピングして返す
	@param (center)　元の位置の座標(基本は中心0.0f,0.0f)
	@param (target)　ターゲットの座標
	@return 画面内に変換された座標
	*/
	Vec3 WindowClipLineVec(const Vec3& center, const Vec3& target);

#endif
}