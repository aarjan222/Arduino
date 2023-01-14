/*
 * array.h
 *
 * Created : 23/2/2019
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _ARRAY_CONTAINER_H_
#define _ARRAY_CONTAINER_H_

#include <stdlib.h>
#include<stdio.h>

template <typename T, size_t N>
size_t arrSize(const T (&arr)[N])
{
        return N;
}

template <size_t N>
void arrPrint(const float (&arr)[N])
{
        printf("[ ");
        for (size_t i = 0; i < N; ++i) {
                printf("%ld ", (int32_t)(arr[i]*1000.0));
        }
        printf("]");
}

template <size_t N>
void arrPrint(const uint8_t (&arr)[N])
{
        printf("[ ");
        for (size_t i = 0; i < N; ++i) {
                printf("%d ", (uint8_t)(arr[i]));
        }
        printf("]");
}

template <size_t N>
void arrMult(float (&arr)[N], float num)
{
        for (size_t i = 0; i < N; ++i) {
                arr[i] *= num;
        }
}

template<typename T, size_t N>
void arrFill(T (&arr)[N], T num)
{
        for (size_t i = 0; i < N; ++i) {
                arr[i] = num;
        }
}

// Only works with datatypes with defined == operator. Doesn't with float/double.
// Returns the first index of the element
template <typename T, size_t N>
int arrIndex(T (&arr)[N], T elem)
{
        for (size_t i = 0; i < N; ++i) {
                if (arr[i] == elem) {
                        return (int)i;
                }
        }
        return -1;
}

template <typename T, size_t N>
void arrCopy(T (&dest)[N], const T (&src)[N])
{
        for (size_t i = 0; i < N; ++i) {
                dest[i] = src[i];
        }
}

template <typename T, size_t N, size_t M>
void arrCopy(T (&dest)[N][M], const T (&src)[N][M])
{
        for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < M; ++j) {
                        dest[i][j] = src[i][j];
                }
        }
}

#endif // !_ARRAY_CONTAINER_H_
