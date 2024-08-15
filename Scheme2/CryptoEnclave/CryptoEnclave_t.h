#ifndef CRYPTOENCLAVE_T_H__
#define CRYPTOENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void ecall_init(void);
void ecall_SRE_cKRev(const char* str, size_t key_len, const void* D, void* result, size_t FloomSize, size_t resultSize);
void ecall_check_doc(const void* remain_node, const void* D, char* val_tag, char* val_ct, void* NewInd, void* DelInd, void* flag, size_t remain_node_size, size_t bloom_filter_size, size_t val_tag_size, size_t val_ct_size, size_t val_ct_cnt, size_t NewInd_size, size_t DelInd_size, size_t flag_size, int index);
void ecall_insert_set_string(void* DelInd_ptr, char* val_tag, size_t DelInd_ptr_size, size_t val_tag_size);

sgx_status_t SGX_CDECL ocall_print_string(const char* str);
sgx_status_t SGX_CDECL ocall_print_string_bytes(const char* str, size_t len);
sgx_status_t SGX_CDECL ocall_insert_set_string(void* set, void* str, size_t set_len, size_t string_len);
sgx_status_t SGX_CDECL ocall_insert_map_str_int(void* map, void* tag, int val, size_t map_len, size_t tag_len);
sgx_status_t SGX_CDECL ocall_insert_vector_GGMNode(void* vec, const unsigned char* keys, int* levels, int* indexs, size_t vec_len, size_t keys_len, size_t cnt);
sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
