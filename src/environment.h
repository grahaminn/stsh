#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "cell.h"
#include <apr_hash.h>

struct environment 
{
	apr_pool_t* pool;
	apr_hash_t* key_value_hash;
};

environment* environment_new(apr_pool_t* poll);
cell* environment_get(apr_pool_t* evaluation_pool, environment* env, char* key);
void environment_put(environment* env, char* key, cell* value);
#endif 
