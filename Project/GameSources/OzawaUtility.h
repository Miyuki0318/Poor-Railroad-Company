/*!
@file OzawaUtility.h
@brief ���[�e�B���e�B
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h") // BaseCross�̃��C�u�������C���N���[�h���Ă��邩�̃}�N��

namespace Utility
{
	/*!
	@brief ���`��Ԃ��s���֐�
	@param (start) ���`��Ԃ̎n�_�̒l
	@param (end) ���`��Ԃ̏I�_�̒l
	@param (time) ���`��Ԃ̊���
	@return �����ɉ����ĕ�Ԃ��ꂽ�l
	*/
	template <class T>
	inline T Lerp(const T& start, const T& end, float time)
	{
		time = time < 0.0f ? 0.0f : time;
		time = time > 1.0f ? 1.0f : time;
		return start + (end - start) * time;
	}

	/*!
	@brief �~�ʕ�ԗp�̐��`��Ԋ֐�
	@param (start) ���`��Ԃ̎n�_�̒l
	@param (end) ���`��Ԃ̏I�_�̒l
	@param (time) ���`��Ԃ̊���
	@return �����ɉ����ĕ�Ԃ��ꂽ�l
	*/
	template <class T>
	inline T SLerp(const T& start, const T& end, float time)
	{
		return start * (1.0f - time) + end * time;
	}

	/*!
	@brief �~�ʕ�Ԃ��s���֐�
	@param (pointA) ��Ԃ̎n�_�̒l
	@param (pointB) ��Ԃ̒��Ԃ̒l
	@param (pointC) ��Ԃ̏I�_�̒l
	@param (time) ��Ԃ̊���
	@return �����ɉ����ĕ�Ԃ��ꂽ�l
	*/
	template <class T>
	inline T BezierLerp(const T& pointA, const T& pointB, const T& pointC, float time)
	{
		T pA2B = SLerp(pointA, pointB, time);
		T pB2C = SLerp(pointB, pointC, time);
		return SLerp(pA2B, pB2C, time);
	}

	/*!
	@brief �͈͂��w�肵���T�C���J�[�u�𐶐�
	@param (time) �o�ߎ���
	@param (min) �J�[�u�̉����l
	@param (max) �J�[�u�̏���l
	@return �͈͓��Ő������ꂽ�T�C���J�[�u
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
	@brief �����_�ȉ��̌�����Ԃ��֐�
	@param (value)�����̌����𒲂ׂ����l
	@return �����_�ȉ��̌�����int�^�ŕԂ�
	*/
	template <class T>
	int GetLowerDigit(const T& value)
	{
		// float/double�^�ȊO�̏ꍇ��0��Ԃ��Ċ֐����I������
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double))
		{
			return 0;
		}

		// ������ŏ����_�𐸍�����ׁA������^�̕ϐ��ɕϊ�
		wstring str = to_wstring(value);

		// �v���p�J�E���^
		int integer = 0;
		int decimal = 0;
		int zeroCount = -1;
		bool errorCheck = false;

		// ������̒��������[�v
		for (int i = 0; i < str.size(); i++)
		{
			// �����_������������
			if (str[i] == '.')
			{
				// �z��̗v�f����ێ�����for�����~
				integer = i;
				break;
			}
		}

		// ������̒��������[�v
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

		// �����_��������Ȃ�������
		if (integer != 0)
		{
			// �����_�ȉ��̌�����Ԃ��J�E���^���v�Z����
			if (errorCheck)
			{
				return decimal = static_cast<int>(str.size()) - integer - zeroCount - 1;
			}

			return decimal = static_cast<int>(str.size()) - integer - 1;
		}

		// �����_�ȉ���Ԃ��J�E���^��0�ɂ���
		return decimal = 1;
	}

	/*!
	@brief �����̌�����Ԃ��֐�
	@param (value)�����̌����𒲂ׂ����l
	@return �����̂̌�����int�^�ŕԂ�
	*/
	template <class T>
	inline int GetUpperDigit(const T& value)
	{
		// float/double�^�ȊO�̏ꍇ��0��Ԃ��Ċ֐����I������
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double) && typeid(value) != typeid(int))
		{
			return 0;
		}

		// ������ŏ����_�𐸍�����ׁA������^�̕ϐ��ɕϊ�
		wstringstream stream;
		stream << value;
		wstring str = stream.str();

		// �v���p�J�E���^
		int integer = 0;

		// ������̒��������[�v
		for (int i = 0; i < str.size(); i++)
		{
			// �����_������������for�����~
			if (str[i] == '.' || (integer == 0 && str[i] == '0')) break;
			integer++;
		}

		// �����_�ȏ�̌�����Ԃ�
		return integer;
	}

	/*!
	@brief �͈͂��w�肵�������𐶐�(�����_�ȉ����Ή�)
	@param (max) �����̏���l
	@param (min) �����̉����l
	@param (digit) �����_�ȉ��̌����w��
	@return �͈͓��Ő������ꂽ����
	*/
	template <class T>
	T RangeRand(const T& min, const T& max, int digit)
	{
		T r = min + static_cast<T>(rand()) * static_cast<T>(max) / RAND_MAX;
		return static_cast<T>(basecross::bsm::floor(static_cast<float>(r), digit));
	}

	/*!
	@brief �͈͂��w�肵�������𐶐�(�����_�ȉ����Ή�)
	@param (max) �����̏���l
	@param (min) �����̉����l
	@return �͈͓��Ő������ꂽ����
	*/
	template <class T>
	T RangeRand(const T& max, const T& min)
	{
		// ���������^�̏ꍇ�͏����̌������v�Z����
		int maxDigit = GetLowerDigit(max);
		int minDigit = GetLowerDigit(min);
		int digit = maxDigit > minDigit ? maxDigit : minDigit;

		return RangeRand(min, max, digit);
	}

	/*!
	@brief �f�B�O���[�p�����W�A���p�ɕϊ�
	@param (deg) �f�B�O���[�p
	@return ���W�A���p�ɕϊ����ꂽ�f�B�O���[�p
	*/
	template <class T>
	T DegToRad(const T& deg)
	{
		return deg * (3.141592654f / 180.0f);
	}

	/*!
	@brief ���W�A���p���f�B�O���[�p�ɕϊ�
	@param (rad) ���W�A���p
	@return �f�B�O���[�p�ɕϊ����ꂽ���W�A���p
	*/
	template <class T>
	T RadToDeg(const T& rad)
	{
		return rad * (180.0f / 3.141592654f);
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(int value, int a, int b);

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(float value, float a, float b);

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	template<class Ty1, class Ty2, class Ty3>
	bool GetBetween(Ty1 value, Ty2 a, Ty3 b)
	{
		return GetBetween(static_cast<int>(value), static_cast<int>(a), static_cast<int>(b));
	}

	/*!
	@brief �񋓌^�̒l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	template <class T>
	bool GetBetween(T value, T a, T b)
	{
		// �񋓌^���ǂ����̃`�F�b�N
		if (is_enum<T>())
		{
			// �񋓌^�Ȃ̂�int�^�ɃL���X�g����
			int iv, ia, ib;
			iv = static_cast<int>(value);
			ia = static_cast<int>(a);
			ib = static_cast<int>(b);

			// int�^�ł�GetBetween()�����s����
			return GetBetween(iv, ia, ib);
		}
		else
		{
#if BASECROSS

			// �G���[���b�Z�[�W�𑗂�
			throw basecross::BaseException(
				L"�񋓌^�ȊO�͎w��ł��܂���",
				L"�񋓌^���w�肳��Ă邩�m�F���Ă�������",
				L"bool GetBetween(T value, T a, T b)"
			);
#endif
			return false;
		}
	}

	/*!
	@brief �C���f�b�N�X���z��͈͓̔�����Ԃ�
	@param (index)�@�m�F����l
	@param (size)�@�z���.size()
	@return �͈͓����̐^�U
	*/
	template<class T>
	bool WithInElemRange(T index, size_t size)
	{
		return (0 <= static_cast<size_t>(index) && static_cast<size_t>(index) < size);
	}

	/*!
	@brief �w�肵����ƍs���z��͈͓̔�����Ԃ�
	@param (row)�@�m�F�����
	@param (col)�@�m�F����s
	@param (elem)�@�m�F����z��
	@return �͈͓����̐^�U
	*/
	template<class Ty1, class Ty2, class Ty3>
	bool WithInElemRange(Ty1 row, Ty2 col, Ty3 elem)
	{
		return WithInElemRange(row, elem.size()) && WithInElemRange(col, elem.at(row).size());
	}

	/*!
	@brief enum�񋓎q��size_t�ɃL���X�g
	@return �L���X�g���ꂽenum
	*/
	template <typename Enum>
	const size_t toSize_t(Enum index)
	{
		// �񋓌^����Ȃ����
		if (!is_enum<Enum>())
		{
#if BASECROSS

			// �G���[���b�Z�[�W�𑗂�
			throw basecross::BaseException(
				L"enum�ȊO��size_t�ɃL���X�g�ł��܂���",
				L"enum���w�肵�Ă�������",
				L"toSize_t(Enum index)"
			);
#endif
			return 0;
		}

		// size_t�ɃL���X�g���ĕԂ�
		return static_cast<size_t>(index);
	}

	/*!
	@brief �������̒l�Ɉ�ԋ߂��l��Ԃ� (���Ԓl������ꍇ�͍ŒZ�ŏo���l���D�悳���)
	@param (num) ���̐�
	@param (a...) ��ׂ鐔(�ϒ�����)
	@return num�Ɉ�ԋ߂����l
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
	@brief �������̒l�Ɉ�ԋ߂��l��Ԃ� (���Ԓl������ꍇ�͍ŒZ�ŏo���l���D�悳���)
	@param (num) ���̐�
	@param (a...) ��ׂ鐔(�ϒ�����)
	@return num�Ɉ�ԋ߂����l
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
	@brief or���Z
	@param ��r��
	@param ��ׂ�l�P
	@param ��ׂ�l�Q
	@return �ǂ��炩�ƈ�v���Ă��邩
	*/
	template <class T>
	bool OR(T val, T a, T b)
	{
		return (val == a || val == b);
	}

	/*!
	@brief or���Z
	@param ��r��
	@param ��ׂ�l(�ϒ�����)
	@return �ǂꂩ�ƈ�v���Ă��邩
	*/
	template <class T, class... V>
	bool ORS(T val, V... nums)
	{
		// �ϒ������̐��������[�v������
		for (T num : { static_cast<T>(nums)... })
		{
			if (num == val) return true;
		}

		return false;
	}

	/*!
	@brief �A���t�@�x�b�g�啶����
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	string ToUpperString(string str);

	/*!
	@brief �A���t�@�x�b�g�啶����
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	wstring ToUpperString(wstring str);

	/*!
	@brief �A���t�@�x�b�g��������
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	string ToLowerString(string str);

	/*!
	@brief �A���t�@�x�b�g��������
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	wstring ToLowerString(wstring str);

#if BASECROSS

	/*!
	@brief ���F����
	 */
#define COL_NONE	Col4(0.0f)

	 /*!
	 @brief �L�F����
	 */
#define COL_ALPHA	Col4(1.0f,1.0f,1.0f,0.0f)

	 /*!
	 @brief ���F
	 */
#define COL_WHITE	Col4(1.0f)

	 /*!
	 @brief ���F
	 */
#define COL_BLACK	Col4(0.0f,0.0f,0.0f,1.0f)

	 /*!
	 @brief �ԐF
	 */
#define COL_RED		Col4(1.0f,0.0f,0.0f,1.0f)

	 /*!
	 @brief �ΐF
	 */
#define COL_GREAN	Col4(0.0f, 1.0f, 0.0f, 1.0f)

	 /*!
	 @brief �F
	 */
#define COL_BLUE	Col4(0.0f,0.0f,1.0f,1.0f)

	 /*!
	 @brief ���F
	 */
#define COL_YELOW	Col4(1.0f, 1.0f, 0.0f, 1.0f)

	 /*!
	 @brief �ΐF
	 */
#define COL_BG		Col4(0.0f, 1.0f, 1.0f, 1.0f)

	 /*!
	 @brief �D�F
	 */
#define COL_GRAY	Col4(0.5f, 0.5f, 0.5f, 1.0f)

	 /*!
	 @brief �f���^�^�C��
	 @return App::GetApp()->GetElapsedTime()
	 */
#define DELTA_TIME basecross::App::GetApp()->GetElapsedTime()

	 /*!
	 @brief �E�B���h�E�̕�
	 @return App::GetApp()->GetGameWidth()
	 */
#define WINDOW_WIDTH static_cast<float>(basecross::App::GetApp()->GetGameWidth())

	 /*!
	 @brief �E�B���h�E�̍���
	 @return App::GetApp()->GetGameHeight()
	 */
#define WINDOW_HEIGHT static_cast<float>(basecross::App::GetApp()->GetGameHeight())

	 /*!
	 @brief �E�B���h�E�̃T�C�Y
	 @return Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)
	 */
#define WINDOW_SIZE Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)

	 /*!
	 @brief This�|�C���^
	 @return static_cast<void*>(ObjectInterface::shared_from_this().get())
	 */
#define ThisPtr static_cast<void*>(shared_from_this().get())

	 /*!
	 @brief �����Vec3
	 @return Vec3(0.0f, 1.0f, 0.0f)
	 */
#define UP_VEC		Vec3(0.0f, 1.0f, 0.0f)

	 /*!
	 @brief ������Vec3
	 @return Vec3(0.0f, -1.0f, 0.0f)
	 */
#define DOWN_VEC	Vec3(0.0f, -1.0f, 0.0f)

	 /*!
	 @brief ������Vec3
	 @return Vec3(-1.0f, 0.0f, 0.0f)
	 */
#define LEFT_VEC	Vec3(-1.0f, 0.0f, 0.0f)

	 /*!
	 @brief �E����Vec3
	 @return Vec3(1.0f, 0.0f, 0.0f)
	 */
#define RIGHT_VEC	Vec3(1.0f, 0.0f, 0.0f)

	 /*!
	 @brief �O����Vec3
	 @return Vec3(0.0f, 0.0f, -1.0f)
	 */
#define FRONT_VEC	Vec3(0.0f, 0.0f, -1.0f)

	 /*!
	 @brief �����Vec3
	 @return Vec3(0.0f, 0.0f, 1.0f)
	 */
#define BACK_VEC	Vec3(0.0f, 0.0f, 1.0f)

	 /*!
	 @brief 2D���Wz�����ZVec3
	 @return Vec3(0.0f, 0.0f, 0.1f)
	 */
#define BACK_LAYER	Vec3(0.0f, 0.0f, 0.1f)

	/*!
	@brief �V���v���Ȓ��_�f�[�^���쐬
	@param (vertices) ���_�f�[�^
	*/
	void SimpleVertices(vector<basecross::VertexPositionColorTexture>& vertices, const Col4& color = COL_WHITE);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color);

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number);

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	@param (number) ����
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number);

	/*!
	@brief ���{���p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (axisVec) ���x�N�g��
	@param (weight) ���̃T�C�Y
	@param (texLoop) �e�N�X�`���̃��[�v��
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop);

	/*!
	@brief ���{���p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	@param (axisVec) ���x�N�g��
	@param (weight) ���̃T�C�Y
	@param (texLoop) �e�N�X�`���̃��[�v��
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop);

	/*!
	@brief �~�^�p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void CircleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int squareNum);

	/*!
	@brief �~�^�p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum);

	/*!
	@brief ���[���h���W���X�N���[�����W�ɕϊ�(BaseCross�̍��W�p�A����ȊO��DirectX�t���̊֐�����l)
	@param (viewPtr) view��shared_ptr
	@param (position) ���[���h���W
	@return �ϊ����ꂽ�X�N���[�����W
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position);

	/*!
	@brief �r���{�[�h���N�H�[�^�j�I���ō쐬���Ԃ�
	@param (Line)�@�J�����̒����_ - �J�����̈ʒu
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat GetBillboardQuat(const Vec3& Line);

	/*!
	@brief �r���{�[�h��Ԃ����]�������N�H�[�^�j�I���ō쐬���Ԃ�
	@param (cameraPos)�@�J�����̈ʒu
	@param (objPos)�@�I�u�W�F�N�g�̈ʒu
	@param (rotateVec)�@��]��(Vec3)
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat GetBillboardRotateQuat(const Vec3& Line, Vec3& rotateVec);

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
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

		// ���W�̃R�[�h�v�Z
		int ComputeOutCode(float x, float y);
	}

	/*!
	@brief ���̈ʒu�ƃ^�[�Q�b�g�Ő��������A��ʓ��̍��W�ɃN���b�s���O���ĕԂ�
	@param (center)�@���̈ʒu�̍��W(��{�͒��S0.0f,0.0f)
	@param (target)�@�^�[�Q�b�g�̍��W
	@return ��ʓ��ɕϊ����ꂽ���W
	*/
	Vec3 WindowClipLineVec(const Vec3& center, const Vec3& target);

#endif
}