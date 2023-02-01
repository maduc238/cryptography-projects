/**
 * @file sha1.h
 * @author Ma Duc (mavietduc@gmail.com)
 * 
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _SHA1_H_
#define _SHA1_H_

#include <stdint.h>
#include <stdio.h>

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data qua dai */
    shaStateError       /* loi :)*/
};
#endif
#define SHA1HashSize 20

/*
 *  Structure cua khoi thuc hien SHA1
 */
typedef struct SHA1Context
{
    uint32_t Intermediate_Hash[SHA1HashSize/4];

    uint32_t Length_Low;
    uint32_t Length_High;

    int_least16_t Message_Block_Index;
    uint8_t Message_Block[64];

    int Computed;
    int Corrupted;
    int Process_Count;

    uint8_t Print_Block_Input;     /* Optional, print binary tung khoi */

} SHA1Context;

/*
 *  Function Prototypes
 */
int SHA1Reset(  SHA1Context *);
int SHA1Input(  SHA1Context *,
                const uint8_t *,
                unsigned int);
int SHA1Result( SHA1Context *,
                uint8_t Message_Digest[SHA1HashSize]);

#endif
