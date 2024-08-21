#include "CryptoEnclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_ecall_init(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ecall_init();
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_SRE_cKRev(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_SRE_cKRev_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_SRE_cKRev_t* ms = SGX_CAST(ms_ecall_SRE_cKRev_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_str = ms->ms_str;
	size_t _len_str = ms->ms_str_len ;
	char* _in_str = NULL;
	const void* _tmp_D = ms->ms_D;
	size_t _tmp_FloomSize = ms->ms_FloomSize;
	size_t _len_D = _tmp_FloomSize;
	void* _in_D = NULL;
	void* _tmp_result = ms->ms_result;
	size_t _tmp_resultSize = ms->ms_resultSize;
	size_t _len_result = _tmp_resultSize;
	void* _in_result = NULL;

	CHECK_UNIQUE_POINTER(_tmp_str, _len_str);
	CHECK_UNIQUE_POINTER(_tmp_D, _len_D);
	CHECK_UNIQUE_POINTER(_tmp_result, _len_result);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_str != NULL && _len_str != 0) {
		_in_str = (char*)malloc(_len_str);
		if (_in_str == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_str, _len_str, _tmp_str, _len_str)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_str[_len_str - 1] = '\0';
		if (_len_str != strlen(_in_str) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_tmp_D != NULL && _len_D != 0) {
		_in_D = (void*)malloc(_len_D);
		if (_in_D == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_D, _len_D, _tmp_D, _len_D)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_result != NULL && _len_result != 0) {
		_in_result = (void*)malloc(_len_result);
		if (_in_result == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_result, _len_result, _tmp_result, _len_result)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_SRE_cKRev((const char*)_in_str, ms->ms_key_len, (const void*)_in_D, _in_result, _tmp_FloomSize, _tmp_resultSize);
	if (_in_result) {
		if (memcpy_s(_tmp_result, _len_result, _in_result, _len_result)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_str) free(_in_str);
	if (_in_D) free(_in_D);
	if (_in_result) free(_in_result);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_check_doc(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_check_doc_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_check_doc_t* ms = SGX_CAST(ms_ecall_check_doc_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const void* _tmp_remain_node = ms->ms_remain_node;
	size_t _tmp_remain_node_size = ms->ms_remain_node_size;
	size_t _len_remain_node = _tmp_remain_node_size;
	void* _in_remain_node = NULL;
	const void* _tmp_D = ms->ms_D;
	size_t _tmp_bloom_filter_size = ms->ms_bloom_filter_size;
	size_t _len_D = _tmp_bloom_filter_size;
	void* _in_D = NULL;
	char* _tmp_val_tag = ms->ms_val_tag;
	size_t _tmp_val_tag_size = ms->ms_val_tag_size;
	size_t _len_val_tag = _tmp_val_tag_size;
	char* _in_val_tag = NULL;
	char* _tmp_val_ct = ms->ms_val_ct;
	size_t _tmp_val_ct_size = ms->ms_val_ct_size;
	size_t _tmp_val_ct_cnt = ms->ms_val_ct_cnt;
	size_t _len_val_ct = _tmp_val_ct_cnt * _tmp_val_ct_size;
	char* _in_val_ct = NULL;
	void* _tmp_NewInd = ms->ms_NewInd;
	size_t _tmp_NewInd_size = ms->ms_NewInd_size;
	size_t _len_NewInd = _tmp_NewInd_size;
	void* _in_NewInd = NULL;
	void* _tmp_DelInd = ms->ms_DelInd;
	size_t _tmp_DelInd_size = ms->ms_DelInd_size;
	size_t _len_DelInd = _tmp_DelInd_size;
	void* _in_DelInd = NULL;
	void* _tmp_flag = ms->ms_flag;
	size_t _tmp_flag_size = ms->ms_flag_size;
	size_t _len_flag = _tmp_flag_size;
	void* _in_flag = NULL;

	if (_tmp_val_ct_size != 0 &&
		(size_t)_tmp_val_ct_cnt > (SIZE_MAX / _tmp_val_ct_size)) {
		return SGX_ERROR_INVALID_PARAMETER;
	}

	CHECK_UNIQUE_POINTER(_tmp_remain_node, _len_remain_node);
	CHECK_UNIQUE_POINTER(_tmp_D, _len_D);
	CHECK_UNIQUE_POINTER(_tmp_val_tag, _len_val_tag);
	CHECK_UNIQUE_POINTER(_tmp_val_ct, _len_val_ct);
	CHECK_UNIQUE_POINTER(_tmp_NewInd, _len_NewInd);
	CHECK_UNIQUE_POINTER(_tmp_DelInd, _len_DelInd);
	CHECK_UNIQUE_POINTER(_tmp_flag, _len_flag);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_remain_node != NULL && _len_remain_node != 0) {
		_in_remain_node = (void*)malloc(_len_remain_node);
		if (_in_remain_node == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_remain_node, _len_remain_node, _tmp_remain_node, _len_remain_node)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_D != NULL && _len_D != 0) {
		_in_D = (void*)malloc(_len_D);
		if (_in_D == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_D, _len_D, _tmp_D, _len_D)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_val_tag != NULL && _len_val_tag != 0) {
		if ( _len_val_tag % sizeof(*_tmp_val_tag) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_val_tag = (char*)malloc(_len_val_tag);
		if (_in_val_tag == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_val_tag, _len_val_tag, _tmp_val_tag, _len_val_tag)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_val_ct != NULL && _len_val_ct != 0) {
		if ( _len_val_ct % sizeof(*_tmp_val_ct) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_val_ct = (char*)malloc(_len_val_ct);
		if (_in_val_ct == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_val_ct, _len_val_ct, _tmp_val_ct, _len_val_ct)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_NewInd != NULL && _len_NewInd != 0) {
		_in_NewInd = (void*)malloc(_len_NewInd);
		if (_in_NewInd == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_NewInd, _len_NewInd, _tmp_NewInd, _len_NewInd)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_DelInd != NULL && _len_DelInd != 0) {
		_in_DelInd = (void*)malloc(_len_DelInd);
		if (_in_DelInd == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_DelInd, _len_DelInd, _tmp_DelInd, _len_DelInd)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_flag != NULL && _len_flag != 0) {
		_in_flag = (void*)malloc(_len_flag);
		if (_in_flag == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_flag, _len_flag, _tmp_flag, _len_flag)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_check_doc((const void*)_in_remain_node, (const void*)_in_D, _in_val_tag, _in_val_ct, _in_NewInd, _in_DelInd, _in_flag, _tmp_remain_node_size, _tmp_bloom_filter_size, _tmp_val_tag_size, _tmp_val_ct_size, _tmp_val_ct_cnt, _tmp_NewInd_size, _tmp_DelInd_size, _tmp_flag_size, ms->ms_index);
	if (_in_NewInd) {
		if (memcpy_s(_tmp_NewInd, _len_NewInd, _in_NewInd, _len_NewInd)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_in_DelInd) {
		if (memcpy_s(_tmp_DelInd, _len_DelInd, _in_DelInd, _len_DelInd)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_in_flag) {
		if (memcpy_s(_tmp_flag, _len_flag, _in_flag, _len_flag)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_remain_node) free(_in_remain_node);
	if (_in_D) free(_in_D);
	if (_in_val_tag) free(_in_val_tag);
	if (_in_val_ct) free(_in_val_ct);
	if (_in_NewInd) free(_in_NewInd);
	if (_in_DelInd) free(_in_DelInd);
	if (_in_flag) free(_in_flag);
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[3];
} g_ecall_table = {
	3,
	{
		{(void*)(uintptr_t)sgx_ecall_init, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_SRE_cKRev, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_check_doc, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[10][3];
} g_dyn_entry_table = {
	10,
	{
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_string_bytes(const char* str, size_t len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = len;

	ms_ocall_print_string_bytes_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_bytes_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_bytes_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_bytes_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_bytes_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	ms->ms_len = len;
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_insert_set_string(void* set, void* str, size_t set_len, size_t string_len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_set = set_len;
	size_t _len_str = string_len;

	ms_ocall_insert_set_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_insert_set_string_t);
	void *__tmp = NULL;

	void *__tmp_set = NULL;
	void *__tmp_str = NULL;

	CHECK_ENCLAVE_POINTER(set, _len_set);
	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (set != NULL) ? _len_set : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_insert_set_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_insert_set_string_t));
	ocalloc_size -= sizeof(ms_ocall_insert_set_string_t);

	if (set != NULL) {
		ms->ms_set = (void*)__tmp;
		__tmp_set = __tmp;
		if (memcpy_s(__tmp_set, ocalloc_size, set, _len_set)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_set);
		ocalloc_size -= _len_set;
	} else {
		ms->ms_set = NULL;
	}
	
	if (str != NULL) {
		ms->ms_str = (void*)__tmp;
		__tmp_str = __tmp;
		if (memcpy_s(__tmp_str, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	ms->ms_set_len = set_len;
	ms->ms_string_len = string_len;
	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
		if (set) {
			if (memcpy_s((void*)set, _len_set, __tmp_set, _len_set)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (str) {
			if (memcpy_s((void*)str, _len_str, __tmp_str, _len_str)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_insert_map_str_int(void* map, void* tag, int val, size_t map_len, size_t tag_len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_map = map_len;
	size_t _len_tag = tag_len;

	ms_ocall_insert_map_str_int_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_insert_map_str_int_t);
	void *__tmp = NULL;

	void *__tmp_map = NULL;
	void *__tmp_tag = NULL;

	CHECK_ENCLAVE_POINTER(map, _len_map);
	CHECK_ENCLAVE_POINTER(tag, _len_tag);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (map != NULL) ? _len_map : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (tag != NULL) ? _len_tag : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_insert_map_str_int_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_insert_map_str_int_t));
	ocalloc_size -= sizeof(ms_ocall_insert_map_str_int_t);

	if (map != NULL) {
		ms->ms_map = (void*)__tmp;
		__tmp_map = __tmp;
		if (memcpy_s(__tmp_map, ocalloc_size, map, _len_map)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_map);
		ocalloc_size -= _len_map;
	} else {
		ms->ms_map = NULL;
	}
	
	if (tag != NULL) {
		ms->ms_tag = (void*)__tmp;
		__tmp_tag = __tmp;
		if (memcpy_s(__tmp_tag, ocalloc_size, tag, _len_tag)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_tag);
		ocalloc_size -= _len_tag;
	} else {
		ms->ms_tag = NULL;
	}
	
	ms->ms_val = val;
	ms->ms_map_len = map_len;
	ms->ms_tag_len = tag_len;
	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
		if (map) {
			if (memcpy_s((void*)map, _len_map, __tmp_map, _len_map)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (tag) {
			if (memcpy_s((void*)tag, _len_tag, __tmp_tag, _len_tag)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_insert_vector_GGMNode(void* vec, const unsigned char* keys, int* levels, int* indexs, size_t vec_len, size_t keys_len, size_t cnt)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_vec = vec_len;
	size_t _len_keys = keys ? strlen(keys) + 1 : 0;
	size_t _len_levels = cnt * sizeof(int);
	size_t _len_indexs = cnt * sizeof(int);

	ms_ocall_insert_vector_GGMNode_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_insert_vector_GGMNode_t);
	void *__tmp = NULL;

	void *__tmp_vec = NULL;
	void *__tmp_levels = NULL;
	void *__tmp_indexs = NULL;

	CHECK_ENCLAVE_POINTER(vec, _len_vec);
	CHECK_ENCLAVE_POINTER(keys, _len_keys);
	CHECK_ENCLAVE_POINTER(levels, _len_levels);
	CHECK_ENCLAVE_POINTER(indexs, _len_indexs);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (vec != NULL) ? _len_vec : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (keys != NULL) ? _len_keys : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (levels != NULL) ? _len_levels : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (indexs != NULL) ? _len_indexs : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_insert_vector_GGMNode_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_insert_vector_GGMNode_t));
	ocalloc_size -= sizeof(ms_ocall_insert_vector_GGMNode_t);

	if (vec != NULL) {
		ms->ms_vec = (void*)__tmp;
		__tmp_vec = __tmp;
		if (memcpy_s(__tmp_vec, ocalloc_size, vec, _len_vec)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_vec);
		ocalloc_size -= _len_vec;
	} else {
		ms->ms_vec = NULL;
	}
	
	if (keys != NULL) {
		ms->ms_keys = (const unsigned char*)__tmp;
		if (_len_keys % sizeof(*keys) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, keys, _len_keys)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_keys);
		ocalloc_size -= _len_keys;
	} else {
		ms->ms_keys = NULL;
	}
	
	if (levels != NULL) {
		ms->ms_levels = (int*)__tmp;
		__tmp_levels = __tmp;
		if (_len_levels % sizeof(*levels) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp_levels, ocalloc_size, levels, _len_levels)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_levels);
		ocalloc_size -= _len_levels;
	} else {
		ms->ms_levels = NULL;
	}
	
	if (indexs != NULL) {
		ms->ms_indexs = (int*)__tmp;
		__tmp_indexs = __tmp;
		if (_len_indexs % sizeof(*indexs) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp_indexs, ocalloc_size, indexs, _len_indexs)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_indexs);
		ocalloc_size -= _len_indexs;
	} else {
		ms->ms_indexs = NULL;
	}
	
	ms->ms_vec_len = vec_len;
	ms->ms_keys_len = keys_len;
	ms->ms_cnt = cnt;
	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
		if (vec) {
			if (memcpy_s((void*)vec, _len_vec, __tmp_vec, _len_vec)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (levels) {
			if (memcpy_s((void*)levels, _len_levels, __tmp_levels, _len_levels)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (indexs) {
			if (memcpy_s((void*)indexs, _len_indexs, __tmp_indexs, _len_indexs)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(int);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	void *__tmp_cpuinfo = NULL;

	CHECK_ENCLAVE_POINTER(cpuinfo, _len_cpuinfo);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (cpuinfo != NULL) ? _len_cpuinfo : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));
	ocalloc_size -= sizeof(ms_sgx_oc_cpuidex_t);

	if (cpuinfo != NULL) {
		ms->ms_cpuinfo = (int*)__tmp;
		__tmp_cpuinfo = __tmp;
		if (_len_cpuinfo % sizeof(*cpuinfo) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_cpuinfo, 0, _len_cpuinfo);
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		ocalloc_size -= _len_cpuinfo;
	} else {
		ms->ms_cpuinfo = NULL;
	}
	
	ms->ms_leaf = leaf;
	ms->ms_subleaf = subleaf;
	status = sgx_ocall(5, ms);

	if (status == SGX_SUCCESS) {
		if (cpuinfo) {
			if (memcpy_s((void*)cpuinfo, _len_cpuinfo, __tmp_cpuinfo, _len_cpuinfo)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);

	ms->ms_self = self;
	status = sgx_ocall(6, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);

	ms->ms_waiter = waiter;
	status = sgx_ocall(7, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);

	ms->ms_waiter = waiter;
	ms->ms_self = self;
	status = sgx_ocall(8, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(void*);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(waiters, _len_waiters);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (waiters != NULL) ? _len_waiters : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);

	if (waiters != NULL) {
		ms->ms_waiters = (const void**)__tmp;
		if (_len_waiters % sizeof(*waiters) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, waiters, _len_waiters)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		ocalloc_size -= _len_waiters;
	} else {
		ms->ms_waiters = NULL;
	}
	
	ms->ms_total = total;
	status = sgx_ocall(9, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

