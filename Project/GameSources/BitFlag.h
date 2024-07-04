#pragma once
using namespace std; // STD�̏ȗ�

/*!
@brief �r�b�g�t���O�p�p�����N���X
@param �t���O�w��q�penum
@param bit�v�Z����^
*/
template <class Enum, class BitType>
class BitFlag
{
protected:

    static_assert(is_enum<Enum>(), "�ϐ��錾���ɂ�����enum����Ă܂����H");

    BitType m_flags; // �t���O�ۑ���

    /*!
    @brief �R���X�g���N�^
    @param �����l = 0
    */
    BitFlag(BitType initialValue = 0) : m_flags(initialValue) {}

    /*!
    @brief �r�b�g�V�t�g�֐�
    @param enum
    @return BitType
    */
    BitType BitShift(Enum flag) const
    {
        return static_cast<BitType>(1) << static_cast<BitType>(flag);
    }

    /*!
    @brief �t���O���I���ɂȂ��Ă邩�̎擾�֐�
    @return �I����������true�A�I�t��������false
    */
    bool isFlag(Enum flag) const
    {
        return (m_flags & BitShift(flag)) != 0;
    }

    /*!
    @brief �t���O�ݒ�p�N���X
    */
    class SetResult
    {
    private:

        BitFlag& parent; // �Ăяo����
        BitType value; // �ύX����r�b�g

    public:

        /*!
        @brief �R���X�g���N�^
        @param �t���O�Ǘ��N���X
        @param �ύX����r�b�g
        */
        SetResult(BitFlag& parent, BitType value) : parent(parent), value(value) {}

        /*!
        @brief Set(enum) = bool�Őݒ�o����l�ɂ���ׂ�=���Z�q�̃I�[�o�[���[�h
        @param bool
        */
        void operator=(bool boolen)
        {
            // �^�U�ɉ����ăt���O���I���I�t
            if (boolen) 
            {
                parent.m_flags |= value;
            }
            else 
            {
                parent.m_flags &= ~value;
            }
        }
    };

public:

    ///*!
    //@brief �t���O���I���ɂ���֐�
    //@param enum
    //*/
    //void SetFlag(Enum flag)
    //{
    //    // �r�b�g�V�t�g����enum�̈ʒu�̃r�b�g���I�A���Z�ŃI���ɂ���
    //    m_flags |= BitShift(flag);
    //}

    ///*!
    //@brief �t���O���I���ɂ���֐�
    //@param enum(�ϒ�)
    //*/
    //template<class... Enums>
    //void SetFlag(Enums... flags)
    //{
    //    // �ϒ������̐��������[�v������
    //    for (Enum flag : { static_cast<Enum>(flags)... })
    //    {
    //        SetFlag(flag);
    //    }
    //}

    ///*!
    //@brief �t���O���I�t�ɂ���֐�
    //@param enum
    //*/
    //void ClearFlag(Enum flag)
    //{
    //    // �r�b�g�V�t�g����enum�̈ʒu�̃r�b�g���m�b�g���Z�ŃI�t�ɂ���
    //    m_flags &= ~BitShift(flag);
    //}

    ///*!
    //@brief �t���O���I�t�ɂ���֐�
    //@param enum(�ϒ�)
    //*/
    //template<class... Enums>
    //void ClearFlag(Enums... flags)
    //{
    //    // �ϒ������̐��������[�v������
    //    for (Enum flag : { static_cast<Enum>(flags)... })
    //    {
    //        ClearFlag(flag);
    //    }
    //}

    /*!
    @brief �t���O���I���ɂȂ��Ă邩�̎擾�֐�
    @param enum(�ϒ�)
    */
    template<class... Enums>
    bool operator()(Enums... flags) const
    {
        bool result = true; // �ۑ��p�ϐ�

        // �ϒ������̐��������[�v������
        for (Enum flag : { static_cast<Enum>(flags)... })
        {
            result = result && isFlag(flag);
        }
        return result;
    }

    /*!
    @brief Set(enum) = bool�Őݒ�o����l�ɂ���ׂ�()���Z�q�̃I�[�o�[���[�h
    @param enum(�ϒ�)
    */
    template<class... Enums>
    SetResult Set(Enums... flags)
    {
        BitType temp = 0; // �ۑ��p�ϐ�

        // �ϒ������̐��������[�v������
        for (Enum flag : { static_cast<Enum>(flags)... })
        {
            temp |= BitShift(flag);
        }

        // �ݒ�p�N���X�𐶐����ĕԂ�
        return SetResult(*this, temp);
    }

    /*!
    @brief ���Z�b�g�֐�
    */
    void Reset()
    {
        m_flags = 0;
    }

    /*!
    @brief !=���Z�q
    */
    void operator=(BitFlag<Enum, BitType> temp)
    {
        this->m_flags = temp.m_flags;
    }

    /*!
    @brief !=���Z�q
    */
    bool operator!=(BitFlag<Enum, BitType> temp)
    {
        return this->m_flags != temp.m_flags;
    }

    /*!
    @brief ==���Z�q
    */
    bool operator==(BitFlag<Enum, BitType> temp)
    {
        return this->m_flags == temp.m_flags;
    }
};