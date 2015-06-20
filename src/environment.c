#include <apr_strings.h>

#include "environment.h"


environment* environment_new(apr_pool_t* pool)
{
	environment* env = apr_palloc(pool, sizeof(environment));
	env->pool = pool;
	env->key_value_hash = apr_hash_make(pool);
	return env;
}

void environment_put(environment* env, char* key, cell* value)
{
	apr_hash_set(env->key_value_hash, apr_pstrdup(env->pool, key), APR_HASH_KEY_STRING, copy_cell(env->pool, value));
}

cell* environment_get(apr_pool_t* evaluation_pool, environment* env, char* key)
{
	cell* hash_result = apr_hash_get(env->key_value_hash, key, APR_HASH_KEY_STRING);
	if (hash_result != NULL)
	{
		return copy_cell(evaluation_pool, hash_result);
	}
	else
	{
		return NULL;
	}	 
}
