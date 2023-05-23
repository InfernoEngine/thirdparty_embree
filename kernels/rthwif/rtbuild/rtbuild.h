// Copyright 2009-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
#  define RTHWIF_API_EXTERN_C extern "C"
#else
#  define RTHWIF_API_EXTERN_C
#endif

#if defined(_WIN32)
#if defined(EMBREE_RTHWIF_STATIC_LIB)
#  define RTHWIF_API_IMPORT RTHWIF_API_EXTERN_C
#  define RTHWIF_API_EXPORT RTHWIF_API_EXTERN_C
#else
#  define RTHWIF_API_IMPORT RTHWIF_API_EXTERN_C __declspec(dllimport)
#  define RTHWIF_API_EXPORT RTHWIF_API_EXTERN_C __declspec(dllexport)
#endif
#else
#  define RTHWIF_API_IMPORT RTHWIF_API_EXTERN_C
#  define RTHWIF_API_EXPORT RTHWIF_API_EXTERN_C __attribute__ ((visibility ("default")))
#endif

#if defined(RTHWIF_EXPORT_API)
#  define RTHWIF_API RTHWIF_API_EXPORT
#else
#  define RTHWIF_API RTHWIF_API_IMPORT
#endif

#define ZE_APICALL_

#if defined(EMBREE_LEVEL_ZERO)
#include <level_zero/ze_api.h>
#else
typedef struct _ze_driver_handle_t *ze_driver_handle_t;
typedef struct _ze_device_handle_t *ze_device_handle_t;
typedef enum _ze_result_t
{
    ZE_RESULT_SUCCESS = 0,                          ///< [Core] success
    ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY = 0x70000002,///< [Core] insufficient host memory to satisfy call
    ZE_RESULT_EXP_ERROR_OPERANDS_INCOMPATIBLE = 0x7ff00004, ///< [Core, Experimental] operands of comparison are not compatible
    ZE_RESULT_EXP_ERROR_RETRY_RTAS_BUILD = 0x7ff00005,  ///< [Core, Experimental] ray tracing acceleration structure build failed
                                                    ///< due to insufficient resources, retry with a larger buffer allocation
    ZE_RESULT_ERROR_INVALID_ARGUMENT = 0x78000004,  ///< [Validation] generic error code for invalid arguments
    ZE_RESULT_ERROR_INVALID_NULL_HANDLE = 0x78000005,   ///< [Validation] handle argument is not valid
    ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE = 0x78000006,  ///< [Validation] object pointed to by handle still in-use by device
    ZE_RESULT_ERROR_INVALID_NULL_POINTER = 0x78000007,  ///< [Validation] pointer argument may not be nullptr
    ZE_RESULT_ERROR_INVALID_ENUMERATION = 0x7800000c,   ///< [Validation] enumerator argument is not valid
    ZE_RESULT_ERROR_UNKNOWN = 0x7ffffffe,           ///< [Core] unknown or internal error
    ZE_RESULT_FORCE_UINT32 = 0x7fffffff
} ze_result_t;
typedef enum _ze_structure_type_t {} ze_structure_type_t;
#ifndef ZE_MAKE_VERSION
#  define ZE_MAKE_VERSION( _major, _minor )  (( _major << 16 )|( _minor & 0x0000ffff))
#endif // ZE_MAKE_VERSION
#ifndef ZE_BIT
#  define ZE_BIT( _i )  ( 1 << _i )
#endif // ZE_BIT
#define ZE_APICALL
#endif

#if !defined(ZE_RTAS_BUILDER_EXP_NAME)
#undef ZE_APIEXPORT
#define ZE_APIEXPORT RTHWIF_API_EXPORT
#include "ze_rtas.h"
#endif

RTHWIF_API ze_result_t zeRTASBuilderCreateExpImpl(ze_driver_handle_t hDriver, const ze_rtas_builder_exp_desc_t *pDescriptor, ze_rtas_builder_exp_handle_t *phBuilder);

inline ze_result_t zeRTASBuilderCreateExp(ze_driver_handle_t hDriver, const ze_rtas_builder_exp_desc_t *pDescriptor, ze_rtas_builder_exp_handle_t *phBuilder) {
  return zeRTASBuilderCreateExpImpl(hDriver,pDescriptor,phBuilder);
}

RTHWIF_API ze_result_t zeRTASBuilderDestroyExpImpl(ze_rtas_builder_exp_handle_t hBuilder);

inline ze_result_t zeRTASBuilderDestroyExp(ze_rtas_builder_exp_handle_t hBuilder) {
  return zeRTASBuilderDestroyExpImpl(hBuilder);
}


RTHWIF_API ze_result_t zeDeviceGetRTASPropertiesExpImpl( const ze_device_handle_t hDevice, ze_rtas_device_exp_properties_t* pRtasProp );
  
inline ze_result_t zeDeviceGetRTASPropertiesExp( const ze_device_handle_t hDevice, ze_rtas_device_exp_properties_t* pRtasProp ) {
  return zeDeviceGetRTASPropertiesExpImpl( hDevice, pRtasProp);
}

RTHWIF_API ze_result_t zeRTASBuilderDeviceFormatCompatibilityCheckExpImpl( ze_rtas_builder_exp_handle_t hBuilder,
                                                                       const ze_rtas_format_exp_t accelFormat,
                                                                       const ze_rtas_format_exp_t otherAccelFormat);

inline ze_result_t zeRTASBuilderDeviceFormatCompatibilityCheckExp( ze_rtas_builder_exp_handle_t hBuilder,
                                                                       const ze_rtas_format_exp_t accelFormat,
                                                                       const ze_rtas_format_exp_t otherAccelFormat)
{
  return zeRTASBuilderDeviceFormatCompatibilityCheckExpImpl( hBuilder, accelFormat, otherAccelFormat);
}

RTHWIF_API ze_result_t zeRTASBuilderGetBuildPropertiesExpImpl(ze_rtas_builder_exp_handle_t hBuilder,
                                                              const ze_rtas_builder_build_op_exp_desc_t* args,
                                                              ze_rtas_builder_exp_properties_t* pProp);
  
inline ze_result_t zeRTASBuilderGetBuildPropertiesExp(ze_rtas_builder_exp_handle_t hBuilder,
                                                          const ze_rtas_builder_build_op_exp_desc_t* args,
                                                          ze_rtas_builder_exp_properties_t* pProp)
{
  return zeRTASBuilderGetBuildPropertiesExpImpl(hBuilder, args, pProp);
}
  
RTHWIF_API ze_result_t zeRTASBuilderBuildExpImpl(ze_rtas_builder_exp_handle_t hBuilder,
                                                 const ze_rtas_builder_build_op_exp_desc_t* args,
                                                 void *pScratchBuffer, size_t scratchBufferSizeBytes,
                                                 void *pRtasBuffer, size_t rtasBufferSizeBytes,
                                                 ze_rtas_parallel_operation_exp_handle_t hParallelOperation,
                                                 void *pBuildUserPtr, ze_rtas_aabb_exp_t *pBounds, size_t *pRtasBufferSizeBytes);

inline ze_result_t zeRTASBuilderBuildExp(ze_rtas_builder_exp_handle_t hBuilder,
                                         const ze_rtas_builder_build_op_exp_desc_t* args,
                                         void *pScratchBuffer, size_t scratchBufferSizeBytes,
                                         void *pRtasBuffer, size_t rtasBufferSizeBytes,
                                         ze_rtas_parallel_operation_exp_handle_t hParallelOperation,
                                         void *pBuildUserPtr, ze_rtas_aabb_exp_t *pBounds, size_t *pRtasBufferSizeBytes)
{
  return zeRTASBuilderBuildExpImpl(hBuilder, args, pScratchBuffer, scratchBufferSizeBytes, pRtasBuffer, rtasBufferSizeBytes,
                                   hParallelOperation, pBuildUserPtr, pBounds, pRtasBufferSizeBytes);
}

RTHWIF_API ze_result_t zeRTASParallelOperationCreateExpImpl(ze_driver_handle_t hDriver, ze_rtas_parallel_operation_exp_handle_t* phParallelOperation);

inline ze_result_t zeRTASParallelOperationCreateExp(ze_driver_handle_t hDriver, ze_rtas_parallel_operation_exp_handle_t* phParallelOperation)
{
  return zeRTASParallelOperationCreateExpImpl(hDriver, phParallelOperation);

}

RTHWIF_API ze_result_t zeRTASParallelOperationDestroyExpImpl( ze_rtas_parallel_operation_exp_handle_t hParallelOperation );

inline ze_result_t zeRTASParallelOperationDestroyExp( ze_rtas_parallel_operation_exp_handle_t hParallelOperation ) {
  return zeRTASParallelOperationDestroyExpImpl( hParallelOperation );
};

RTHWIF_API ze_result_t zeRTASParallelOperationGetPropertiesExpImpl( ze_rtas_parallel_operation_exp_handle_t hParallelOperation, ze_rtas_parallel_operation_exp_properties_t* pProperties );

inline ze_result_t zeRTASParallelOperationGetPropertiesExp( ze_rtas_parallel_operation_exp_handle_t hParallelOperation, ze_rtas_parallel_operation_exp_properties_t* pProperties )
{
  return zeRTASParallelOperationGetPropertiesExpImpl( hParallelOperation, pProperties );
}
 
RTHWIF_API ze_result_t zeRTASParallelOperationJoinExpImpl( ze_rtas_parallel_operation_exp_handle_t hParallelOperation);

inline ze_result_t zeRTASParallelOperationJoinExp( ze_rtas_parallel_operation_exp_handle_t hParallelOperation)
{
  return zeRTASParallelOperationJoinExpImpl(hParallelOperation);
}
