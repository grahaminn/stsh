#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "cell.h"
#include <apr_hash.h>

struct environment 
{
	apr_hash_t* key_value_hash;
};

environment* environment_new(apr_pool_t* poll);
cell* environment_get(apr_pool_t* pool, environment* env, cell* key);
void environment_put(apr_pool_t* pool, environment* env, cell* key, cell* value);
#endif 
