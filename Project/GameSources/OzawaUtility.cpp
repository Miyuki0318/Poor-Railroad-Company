/*!
@file OzawaUtility.cpp
@brief ���[�e�B���e�B
@author ���V���M
*/

#include "stdafx.h"
#include "OzawaUtility.h"

namespace Utility
{
	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(int value, int a, int b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(float value, float a, float b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	/*!
	@brief �A���t�@�x�b�g�啶����
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	string ToUpperString(string str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}

	/*!
	@brief �A���t�@�x�b�g�啶����
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	wstring ToUpperString(wstring str)
	{
		transform(str.begin(), str.end(), str.begin(), ::towupper);
		return str;
	}

	/*!
	@brief �A���t�@�x�b�g��������
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	string ToLowerString(string str)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

	/*!
	@brief �A���t�@�x�b�g��������
	@param �ύX�O������
	@return �ύX�㕶����
	*/
	wstring ToLowerString(wstring str)
	{
		transform(str.begin(), str.end(), str.begin(), ::towlower);
		return str;
	}

#if BASECROSS

	/*!
	@brief �V���v���Ȓ��_�f�[�^���쐬
	@param (vertices) ���_�f�[�^
	*/
	void SimpleVertices(vector<basecross::VertexPositionColorTexture>& vertices, const Col4& color)
	{
		// �����̃T�C�Y
		const float HALF_SIZE = 0.5f;

		// ������
		vertices.clear();

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), color, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), color , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), color, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), color, Vec2(1.0f, 1.0f)},
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertices);

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertex.vertices);

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertices, color);

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertex.vertices, color);

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	@param (number) ����
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief ���{���^���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (axisVec) ���s���x�N�g��
	@param (weight) ��
	@param (texLoop) �e�N�X�`�����[�v
	@return �����o�������̐^�U
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop)
	{
		// �ߖڂ̐���0�Ȃ�I��
		const int& size = static_cast<int>(point.size()); 
		if (size == 0) return false;

		vertices.clear(); // ���_���W�̏�����

		// �ߖڂ̐����[�v
		for (int i = 0; i < size; i++)
		{
			// �e�N�X�`�������[�v���邩
			const int loop = texLoop > 0 ? texLoop : 1;

			int front = (i - 1) < 0 ? size - 1 : i - 1; // ��O
			int rear = (i + 1) % size; // ���

			// ��������O�ς����߂�
			Vec3 dir = point.at(front) - point.at(rear);
			Vec3 cross = dir.cross(axisVec).normalize();
		
			// ���������������č��W�𒸓_�Ƃ��Ēǉ�
			Vec3 pos = point.at(i) - cross * weight;
			basecross::VertexPositionColorTexture vertexLeft(pos, COL_WHITE, Vec2(0.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexLeft);

			// ���������Z�������W�𒸓_�Ƃ��Ēǉ�
			pos = point.at(i) + cross * weight;
			basecross::VertexPositionColorTexture vertexRight(pos, COL_WHITE, Vec2(1.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexRight);
		}

		// ���_�C���f�b�N�X�̌�
		const vector<uint16_t> baseIndices = {
			0, 1, 2,
			2, 1, 3
		};

		indices.clear(); // �C���f�b�N�X�̏�����

		// �T�C�Y������񏭂Ȃ������[�v
		for (int i = 0; i < size - 1; i++)
		{
			// ���T�C�Y�����[�v
			for (int j = 0; j < baseIndices.size(); j++)
			{
				// ���̃C���f�b�N�X��2�̔{�������Z�����Ēǉ�
				indices.push_back(baseIndices.at(j) + (2 * i));
			}
		}

		return true; // �����o�����̂�true��Ԃ�
	}

	/*!
	@brief ���{���^���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X
	@param (axisVec) ���s���x�N�g��
	@param (weight) ��
	@param (texLoop) �e�N�X�`�����[�v
	@return �����o�������̐^�U
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop)
	{
		// ���_�f�[�^�\���̂𕪉����Đ����֐������s
		return RibonVerticesIndices(point, vertex.vertices, vertex.indices, axisVec, weight, texLoop);
	}

	/*!
	@brief �~�^�p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void CircleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int squareNum)
	{
		const float HELF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);
		
		vertices.clear();
		indices.clear();

		basecross::VertexPositionColorTexture first(Vec3(0.0f), DEFAULT_COLOR, Vec2(HELF_SIZE));
		vertices.push_back(first);

		for (int i = 0; i < squareNum + 1; i++)
		{
			float rad = (XM_2PI / squareNum * i);
			float x = cosf(rad) * HELF_SIZE;
			float y = sinf(rad) * HELF_SIZE;
			Vec3 position = Vec3(x, y, 0.0f);
			Vec2 uvPos = Vec2(HELF_SIZE) + Vec2(x, -y);

			basecross::VertexPositionColorTexture v(position, DEFAULT_COLOR, uvPos);
			vertices.push_back(v);
		}

		for (int i = 0; i < squareNum; i++)
		{
			// 3�̔{������ɐݒ�
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
	}

	/*!
	@brief �~�^�p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum)
	{
		CircleVerticesIndices(vertex.vertices, vertex.indices, squareNum);
	}

	/*!
	@brief ���[���h���W���X�N���[�����W�ɕϊ�(BaseCross�̍��W�p�A����ȊO��DirectX�t���̊֐�����l)
	@param (viewPtr) view��shared_ptr
	@param (position) ���[���h���W
	@return �ϊ����ꂽ�X�N���[�����W
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position)
	{
		// �r���[����J�����ƃr���[�|�[�g���擾
		const auto& ptrCamera = viewPtr->GetTargetCamera();
		const auto& viewPort = viewPtr->GetTargetViewport();

		// �}�g���b�N�X�̒�`
		const Mat4x4 viewMat = ptrCamera->GetViewMatrix();
		const Mat4x4 projMat = ptrCamera->GetProjMatrix();
		Mat4x4 worldMat;
		worldMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f),
			Vec3(0.0f)
		);

		// 3D���W��2D���W�ɕϊ�(����0,0�)
		XMVECTOR xmWinPos = DirectX::XMVector3Project(
			position,
			viewPort.TopLeftX,
			viewPort.TopLeftY,
			viewPort.Width,
			viewPort.Height,
			viewPort.MinDepth,
			viewPort.MaxDepth,
			projMat,
			viewMat,
			worldMat
		);

		// BaseCross�p�ɕϊ�(����0,0�)
		Vec3 windowPos = Vec3(xmWinPos);
		windowPos.x -= static_cast<float>(viewPort.Width) / 2.0f;
		windowPos.y -= static_cast<float>(viewPort.Height) / 2.0f;
		windowPos.y *= -1.0f;

		// �ϊ����ꂽ���W
		return windowPos;
	}

	/*!
	@brief �r���{�[�h���N�H�[�^�j�I���ō쐬���Ԃ�
	@param (Line)�@�J�����̒����_ - �J�����̈ʒu
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat GetBillboardQuat(const Vec3& Line)
	{
		// �����̃R�s�[
		Vec3 line = Line;
		Vec3 defUp(UP_VEC);
		Vec2 lineVec(line.x, line.z);

		// �O�㍶�E�̒�����0.1f�ȉ��Ȃ��]����ύX
		if (lineVec.length() < 0.1f) defUp = BACK_VEC;
		line.normalize();

		// �}�g���b�N�X�̌v�Z
		Mat4x4 rotMatrix = (Mat4x4)XMMatrixLookAtLH(Vec3(0.0f), line, defUp);
		rotMatrix.inverse();

		// �N�H�[�^�j�I���ɕϊ����A���K��
		Quat qt = rotMatrix.quatInMatrix();
		qt.normalize();
		return qt;
	}

	/*!
	@brief �r���{�[�h��Ԃ����]�������N�H�[�^�j�I���ō쐬���Ԃ�
	@param (cameraPos)�@�J�����̈ʒu
	@param (objPos)�@�I�u�W�F�N�g�̈ʒu
	@param (rotateVec)�@��]��(Vec3)
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat GetBillboardRotateQuat(const Vec3& Line, Vec3& rotateVec)
	{
		// �����̌v�Z
		Vec3 line = Line;
		Vec3 defUp(UP_VEC);
		Vec2 lineVec(line.x, line.z);

		// �O�㍶�E�̒�����0.1f�ȉ��Ȃ��]����ύX
		if (lineVec.length() < 0.1f) defUp = BACK_VEC;
		line.normalize();

		// ��]�ʂ�360�x�𒴂��Ă���0�x�ɂ���
		if (rotateVec.x >= XM_2PI) rotateVec.x -= XM_2PI;
		if (rotateVec.y >= XM_2PI) rotateVec.y -= XM_2PI;
		if (rotateVec.z >= XM_2PI) rotateVec.z -= XM_2PI;

		// �}�g���b�N�X�̌v�Z
		Mat4x4 rotMatrix = (Mat4x4)XMMatrixLookAtLH(Vec3(0.0f), line, defUp);
		rotMatrix *= (Mat4x4)XMMatrixRotationRollPitchYawFromVector(rotateVec);
		rotMatrix.inverse();

		// ����]�������N�H�[�^�j�I����Ԃ�
		return rotMatrix.quatInMatrix();
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y);
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y) && GetBetween(value.z, a.z, b.z);
	}

	float rotYatan2f(const Vec3& A, const Vec3& B)
	{
		return atan2f(B.z - A.z, B.x - A.x);
	}

	namespace CohenClip
	{
		const float xmin = -960.0f;
		const float xmax = 960.0f;
		const float ymin = -540.0f;
		const float ymax = 540.0f;

		// ���W�̃R�[�h�v�Z
		int ComputeOutCode(float x, float y) 
		{
			int code = INSIDE;

			if (x < xmin) code |= LEFT;
			else if (x > xmax) code |= RIGHT;

			if (y < ymin) code |= BOTTOM;				
			else if (y > ymax) code |= TOP;
				
			return code;
		}
	}

	Vec3 WindowClipLineVec(const Vec3& center, const Vec3& target)
	{
		float x0 = center.x;
		float y0 = center.y;
		float x1 = target.x;
		float y1 = target.y;

		int outcode0 = CohenClip::ComputeOutCode(center.x, center.y);
		int outcode1 = CohenClip::ComputeOutCode(target.x, target.y);
		bool accept = false;

		while (true) 
		{
			if (!(outcode0 | outcode1)) 
			{
				// ���[�_���E�B���h�E���ɂ���ꍇ
				accept = true;
				break;
			}
			else if (outcode0 & outcode1)
			{
				// ���[�_���E�B���h�E�̊O���̓����̈�ɂ���ꍇ
				break;
			}
			else 
			{
				// �N���b�s���O����K�v������ꍇ
				Vec3 clipVec;
				int outcodeOut = outcode0 ? outcode0 : outcode1;
				
				// �㑤�Ɍ���
				if (outcodeOut & CohenClip::TOP) 
				{           
					clipVec.x = x0 + (x1 - x0) * (CohenClip::ymax - y0) / (y1 - y0);
					clipVec.y = CohenClip::ymax;
				} 
				else if (outcodeOut & CohenClip::BOTTOM) // �����Ɍ���
				{
					clipVec.x = x0 + (x1 - x0) * (CohenClip::ymin - y0) / (y1 - y0);
					clipVec.y = CohenClip::ymin;
				}
				else if (outcodeOut & CohenClip::RIGHT) // �E���Ɍ���
				{  
					clipVec.y = y0 + (y1 - y0) * (CohenClip::xmax - x0) / (x1 - x0);
					clipVec.x = CohenClip::xmax;
				}
				else if (outcodeOut & CohenClip::LEFT) // �����Ɍ���
				{   
					clipVec.y = y0 + (y1 - y0) * (CohenClip::xmin - x0) / (x1 - x0);
					clipVec.x = CohenClip::xmin;
				}

				// �����_���N���b�v���ꂽ�[�_�Ƃ��Đݒ�
				if (outcodeOut == outcode0) 
				{
					x0 = clipVec.x;
					y0 = clipVec.y;
					outcode0 = CohenClip::ComputeOutCode(x0, y0);
				}
				else
				{
					x1 = clipVec.x;
					y1 = clipVec.y;
					outcode1 = CohenClip::ComputeOutCode(x1, y1);
				}
			}
		}

		if (accept)
		{
			return Vec3(x1, y1, target.z);
		}
		else 
		{
			return target; // �N���b�s���O�Ɏ��s�����ꍇ�����̍��W��Ԃ�
		}
	}

#endif
}