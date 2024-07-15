#pragma once
#include "BitFlag.h"

/*!
@brief �r�b�g�t���O�p�N���X(8�܂�:1�o�C�g)
@param �t���O�w��q�penum
*/
template <class Enum>
class Bool8_t : public BitFlag<Enum, uint8_t>
{
public:

    /*!
    @brief �R���X�g���N�^
    @param �����l = 0
    */
    Bool8_t(uint8_t initialValue = 0) : BitFlag<Enum, uint8_t>(initialValue) {}
};

/*!
@brief �r�b�g�t���O�p�N���X(16�܂�:2�o�C�g)
@param �t���O�w��q�penum
*/
template <class Enum>
class Bool16_t : public BitFlag<Enum, uint16_t>
{
public:

    /*!
    @brief �R���X�g���N�^
    @param �����l = 0
    */
    Bool16_t(uint16_t initialValue = 0) : BitFlag<Enum, uint16_t>(initialValue) {}
};

/*!
@brief �r�b�g�t���O�p�N���X(32�܂�:4�o�C�g)
@param �t���O�w��q�penum
*/
template <class Enum>
class Bool32_t : public BitFlag<Enum, uint32_t>
{
public:

    /*!
    @brief �R���X�g���N�^
    @param �����l = 0
    */
    Bool32_t(uint32_t initialValue = 0) : BitFlag<Enum, uint32_t>(initialValue) {}
};

/*!
@brief �r�b�g�t���O�p�N���X(64�܂�:8�o�C�g)
@param �t���O�w��q�penum
*/
template <class Enum>
class Bool64_t : public BitFlag<Enum, uint64_t>
{
public:

    /*!
    @brief �R���X�g���N�^
    @param �����l = 0
    */
    Bool64_t(uint64_t initialValue = 0) : BitFlag<Enum, uint64_t>(initialValue) {}
};