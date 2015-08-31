#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "cell.h"
#include <apr_hash.h>

struct environment;
typedef struct environment environment;

struct environment 
{
	environment* parent;
	apr_pool_t* pool;
	apr_hash_t* key_value_hash;
};

environment* environment_new(apr_pool_t* pool, environment* parent);
environment* environment_copy(apr_pool_t* pool, environment* env);
cell* environment_get(apr_pool_t* evaluation_pool, environment* env, char* key);
void environment_put(environment* env, char* key, cell* value);
void environment_global_define(environment* env, char* key, cell* value);
#endif 
