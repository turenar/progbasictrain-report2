#include "config.inc.h"

#include "op_hash.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct mat_op_hash_node {
	size_t hash;
	const char* name;
	const mat_op_def_t* op_def;
	struct mat_op_hash_node* next;
} mat_op_hash_node_t;

struct mat_op_hash {
	size_t bucket_size;
	size_t count;
	size_t resize_threshold;
	double resize_ratio;
	mat_op_hash_node_t** bucket;
};

typedef struct mat_op_hash mat_op_hash_t;


static size_t hash_str(const char*);
static void expand_hash(mat_op_hash_t*, size_t bucket_size);
static void transfer(mat_op_hash_t*, mat_op_hash_node_t** new_bucket, size_t bucket_size);
static size_t index_for(size_t hash, size_t bucket_size);

mat_op_hash_t* mat_op_hash_new() {
	mat_op_hash_t* ret = (mat_op_hash_t*) malloc(sizeof(mat_op_hash_t));
	ret->bucket = NULL;
	ret->count = 0;
	ret->resize_ratio = 0.75;
	expand_hash(ret, 16);
	return ret;
}

void mat_op_hash_put(mat_op_hash_t* hash, const char* name, const mat_op_def_t* def) {
	size_t h = hash_str(name);
	size_t index = index_for(h, hash->bucket_size);
	mat_op_hash_node_t* next = hash->bucket[index];
#ifdef _DEBUG
	for (mat_op_hash_node_t* n = next; n != NULL; n = n->next) {
		assert(h != n->hash || strcmp(name, n->name) != 0);
	}
#endif
	mat_op_hash_node_t* node = (mat_op_hash_node_t*) malloc(sizeof(mat_op_hash_node_t));
	node->hash = h;
	node->name = name;
	node->op_def = def;
	node->next = next;

	hash->bucket[index] = node;
	hash->count++;
	if (hash->count >= hash->resize_threshold) {
		expand_hash(hash, hash->bucket_size << 1);
	}
}

const mat_op_def_t* mat_op_hash_get(mat_op_hash_t* hash, const char* name) {
	size_t h = hash_str(name);
	size_t index = index_for(h, hash->bucket_size);
	for (mat_op_hash_node_t* n = hash->bucket[index]; n != NULL; n = n->next) {
		if (h == n->hash && strcmp(name, n->name) == 0) {
			return n->op_def;
		}
	}
	return NULL;
}

void mat_op_hash_free(mat_op_hash_t* hash) {
	for (size_t i = 0; i < hash->bucket_size; ++i) {
		for (mat_op_hash_node_t* n = hash->bucket[i]; n != NULL;) {
			mat_op_hash_node_t* next = n->next;
			free(n);
			n = next;
		}
	}
	free(hash->bucket);
	free(hash);
}

static size_t hash_str(const char* str) {
	size_t h = 0xabcdef;
	for (const char* p = str; *p != '\0'; ++p) {
		h = (h * 31u) ^ (size_t) *p;
	}
	return h;
}

static void expand_hash(mat_op_hash_t* hash, size_t bucket_size) {
#ifdef __GNUC__
	assert(__builtin_popcountll(bucket_size) == 1);
#endif
	mat_op_hash_node_t** new_bucket = (mat_op_hash_node_t**) calloc(bucket_size, sizeof(mat_op_hash_node_t*));
	if (!new_bucket) {
		abort(); // TODO
	}
	if (hash->bucket) {
		transfer(hash, new_bucket, bucket_size);
		free(hash->bucket);
	}
	hash->bucket = new_bucket;
	hash->bucket_size = bucket_size;
	hash->resize_threshold = (size_t) (hash->resize_ratio * (double) bucket_size);
}

static void transfer(mat_op_hash_t* hash, mat_op_hash_node_t** new_bucket, size_t new_size) {
	mat_op_hash_node_t** old_bucket = hash->bucket;
	size_t old_size = hash->bucket_size;
	for (size_t i = 0; i < old_size; ++i) {
		mat_op_hash_node_t* node = old_bucket[i];
		while (node != NULL) {
			mat_op_hash_node_t* next = node->next;
			size_t index = index_for(node->hash, new_size);
			node->next = new_bucket[index];
			new_bucket[index] = node;
			node = next;
		}
	}
}

static size_t index_for(size_t hash, size_t bucket_size) {
#ifdef __GNUC__
	assert(__builtin_popcountll(bucket_size) == 1);
#endif
	return hash & (bucket_size - 1);
}

