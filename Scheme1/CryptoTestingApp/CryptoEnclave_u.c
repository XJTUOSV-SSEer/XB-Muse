#include "CryptoEnclave_u.h"
#include <errno.h>

typedef struct ms_ecall_SRE_cKRev_t {
	const char* ms_str;
	size_t ms_str_len;
	size_t ms_key_len;
	const void* ms_D;
	void* ms_result;
	size_t ms_FloomSize;
	size_t ms_resultSize;
} ms_ecall_SRE_cKRev_t;

typedef struct ms_ecall_check_doc_t {
	const void* ms_remain_node;
	const void* ms_D;
	char* ms_val_tag;
	char* ms_val_ct;
	void* ms_NewInd;
	void* ms_DelInd;
	void* ms_flag;
	size_t ms_remain_node_size;
	size_t ms_bloom_filter_size;
	size_t ms_val_tag_size;
	size_t ms_val_ct_size;
	size_t ms_val_ct_cnt;
	size_t ms_NewInd_size;
	size_t ms_DelInd_size;
	size_t ms_flag_size;
	int ms_index;
} ms_ecall_check_doc_t;

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_ocall_print_string_bytes_t {
	const char* ms_str;
	size_t ms_len;
} ms_ocall_print_string_bytes_t;

typedef struct ms_ocall_insert_set_string_t {
	void* ms_set;
	void* ms_str;
	size_t ms_set_len;
	size_t ms_string_len;
} ms_ocall_insert_set_string_t;

typedef struct ms_ocall_insert_map_str_int_t {
	void* ms_map;
	void* ms_tag;
	int ms_val;
	size_t ms_map_len;
	size_t ms_tag_len;
} ms_ocall_insert_map_str_int_t;

typedef struct ms_ocall_insert_vector_GGMNode_t {
	void* ms_vec;
	const unsigned char* ms_keys;
	int* ms_levels;
	int* ms_indexs;
	size_t ms_vec_len;
	size_t ms_keys_len;
	size_t ms_cnt;
} ms_ocall_insert_vector_GGMNode_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_print_string_bytes(void* pms)
{
	ms_ocall_print_string_bytes_t* ms = SGX_CAST(ms_ocall_print_string_bytes_t*, pms);
	ocall_print_string_bytes(ms->ms_str, ms->ms_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_insert_set_string(void* pms)
{
	ms_ocall_insert_set_string_t* ms = SGX_CAST(ms_ocall_insert_set_string_t*, pms);
	ocall_insert_set_string(ms->ms_set, ms->ms_str, ms->ms_set_len, ms->ms_string_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_insert_map_str_int(void* pms)
{
	ms_ocall_insert_map_str_int_t* ms = SGX_CAST(ms_ocall_insert_map_str_int_t*, pms);
	ocall_insert_map_str_int(ms->ms_map, ms->ms_tag, ms->ms_val, ms->ms_map_len, ms->ms_tag_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_insert_vector_GGMNode(void* pms)
{
	ms_ocall_insert_vector_GGMNode_t* ms = SGX_CAST(ms_ocall_insert_vector_GGMNode_t*, pms);
	ocall_insert_vector_GGMNode(ms->ms_vec, ms->ms_keys, ms->ms_levels, ms->ms_indexs, ms->ms_vec_len, ms->ms_keys_len, ms->ms_cnt);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[10];
} ocall_table_CryptoEnclave = {
	10,
	{
		(void*)CryptoEnclave_ocall_print_string,
		(void*)CryptoEnclave_ocall_print_string_bytes,
		(void*)CryptoEnclave_ocall_insert_set_string,
		(void*)CryptoEnclave_ocall_insert_map_str_int,
		(void*)CryptoEnclave_ocall_insert_vector_GGMNode,
		(void*)CryptoEnclave_sgx_oc_cpuidex,
		(void*)CryptoEnclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)CryptoEnclave_sgx_thread_set_untrusted_event_ocall,
		(void*)CryptoEnclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)CryptoEnclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};
sgx_status_t ecall_init(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 0, &ocall_table_CryptoEnclave, NULL);
	return status;
}

sgx_status_t ecall_SRE_cKRev(sgx_enclave_id_t eid, const char* str, size_t key_len, const void* D, void* result, size_t FloomSize, size_t resultSize)
{
	sgx_status_t status;
	ms_ecall_SRE_cKRev_t ms;
	ms.ms_str = str;
	ms.ms_str_len = str ? strlen(str) + 1 : 0;
	ms.ms_key_len = key_len;
	ms.ms_D = D;
	ms.ms_result = result;
	ms.ms_FloomSize = FloomSize;
	ms.ms_resultSize = resultSize;
	status = sgx_ecall(eid, 1, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_check_doc(sgx_enclave_id_t eid, const void* remain_node, const void* D, char* val_tag, char* val_ct, void* NewInd, void* DelInd, void* flag, size_t remain_node_size, size_t bloom_filter_size, size_t val_tag_size, size_t val_ct_size, size_t val_ct_cnt, size_t NewInd_size, size_t DelInd_size, size_t flag_size, int index)
{
	sgx_status_t status;
	ms_ecall_check_doc_t ms;
	ms.ms_remain_node = remain_node;
	ms.ms_D = D;
	ms.ms_val_tag = val_tag;
	ms.ms_val_ct = val_ct;
	ms.ms_NewInd = NewInd;
	ms.ms_DelInd = DelInd;
	ms.ms_flag = flag;
	ms.ms_remain_node_size = remain_node_size;
	ms.ms_bloom_filter_size = bloom_filter_size;
	ms.ms_val_tag_size = val_tag_size;
	ms.ms_val_ct_size = val_ct_size;
	ms.ms_val_ct_cnt = val_ct_cnt;
	ms.ms_NewInd_size = NewInd_size;
	ms.ms_DelInd_size = DelInd_size;
	ms.ms_flag_size = flag_size;
	ms.ms_index = index;
	status = sgx_ecall(eid, 2, &ocall_table_CryptoEnclave, &ms);
	return status;
}

