#include "environment.h"

environment* environment_new(apr_pool_t* pool)
{
	environment* env = apr_palloc(pool, sizeof(environment));
	env->key_value_hash = apr_hash_make(pool);
	return env;
}

void environment_put(environment* env, cell* key, cell* value)
{
	/* */
}
