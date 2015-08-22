#include <apr_strings.h>

#include "environment.h"


environment* environment_new(apr_pool_t* pool, environment* parent)
{
	environment* env = apr_palloc(pool, sizeof(environment));
	env->parent = parent;
	env->pool = pool;
	env->key_value_hash = apr_hash_make(pool);
	return env;
}

environment* environment_copy(apr_pool_t* pool, environment* env)
{
	environment* copy = apr_palloc(pool, sizeof(environment));
	copy->parent = env->parent;
	copy->pool = pool;
	copy->key_value_hash = apr_hash_copy(pool, env->key_value_hash);
	return copy;
}

void environment_put(environment* env, char* key, cell* value)
{
	apr_hash_set(env->key_value_hash, apr_pstrdup(env->pool, key), APR_HASH_KEY_STRING, copy_cell(env->pool, value));
}

void environment_global_define(environment* env, char* key, cell* value)
{
	if (env->parent)
	{
		environment_global_define(env->parent, key, value);
	}
	else
	{
		environment_put(env, key, value);
	}
}

cell* environment_get(apr_pool_t* evaluation_pool, environment* env, char* key)
{
	cell* hash_result = apr_hash_get(env->key_value_hash, key, APR_HASH_KEY_STRING);
	if (hash_result)
	{
		return copy_cell(evaluation_pool, hash_result);
	}
	else if (env->parent)
	{
		return environment_get(env->parent->pool, env->parent, key);
	}
	else 
	{
		return NULL;
	}	 
}
