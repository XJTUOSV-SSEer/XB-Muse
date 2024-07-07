#ifndef CRYPTOENCLAVE_U_H__
#define CRYPTOENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_PRINT_STRING_DEFINED__
#define OCALL_PRINT_STRING_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));
#endif
#ifndef OCALL_INSERT_SET_STRING_DEFINED__
#define OCALL_INSERT_SET_STRING_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_insert_set_string, (void* set, void* str, size_t set_len, size_t string_len));
#endif
#ifndef OCALL_INSERT_MAP_STR_INT_DEFINED__
#define OCALL_INSERT_MAP_STR_INT_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_insert_map_str_int, (void* map, void* tag, int val, size_t map_len, size_t tag_len));
#endif
#ifndef OCALL_INSERT_VECTOR_GGMNODE_DEFINED__
#define OCALL_INSERT_VECTOR_GGMNODE_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_insert_vector_GGMNode, (void* vec, const unsigned char* keys, int* levels, int* indexs, size_t vec_len, size_t keys_len, size_t cnt));
#endif
#ifndef SGX_OC_CPUIDEX_DEFINED__
#define SGX_OC_CPUIDEX_DEFINED__
void SGX_UBRIDGE(SGX_CDECL, sgx_oc_cpuidex, (int cpuinfo[4], int leaf, int subleaf));
#endif
#ifndef SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
#endif
#ifndef SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));
#endif
#ifndef SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_setwait_untrusted_events_ocall, (const void* waiter, const void* self));
#endif
#ifndef SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_multiple_untrusted_events_ocall, (const void** waiters, size_t total));
#endif

sgx_status_t ecall_init(sgx_enclave_id_t eid);
sgx_status_t ecall_SRE_cKRev(sgx_enclave_id_t eid, const char* str, size_t key_len, const void* D, void* result, size_t FloomSize, size_t resultSize);
sgx_status_t ecall_check_doc(sgx_enclave_id_t eid, const void* remain_node, const void* D, char* val_tag, char* val_ct, void* NewInd, void* DelInd, void* flag, size_t remain_node_size, size_t bloom_filter_size, size_t val_tag_size, size_t val_ct_size, size_t val_ct_cnt, size_t NewInd_size, size_t DelInd_size, size_t flag_size, int index);
sgx_status_t ecall_insert_set_string(sgx_enclave_id_t eid, void* DelInd_ptr, char* val_tag, size_t DelInd_ptr_size, size_t val_tag_size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
