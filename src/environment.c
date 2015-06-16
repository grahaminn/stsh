#include "environment.h"

environment* environment_new(apr_pool_t* pool)
{
	environment* env = apr_palloc(pool, sizeof(environment));
	env->key_value_hash = apr_hash_make(pool);
	return env;
}

void environment_put(apr_pool_t* pool, environment* env, cell* key, cell* value)
{
	apr_hash_set(env->key_value_hash, key->sym, APR_HASH_KEY_STRING, copy_cell(pool, value));
}

cell* environment_get(apr_pool_t* pool, environment* env, cell* key)
{
	cell* hash_result = apr_hash_get(env->key_value_hash, key->sym, APR_HASH_KEY_STRING);
	if (hash_result != NULL)
	{
		return copy_cell(pool, hash_result);
	}
	else
	{
		return err_cell(pool, "unbound symbol!");
	}	 
}
