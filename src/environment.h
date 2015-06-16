#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "cell.h"
#include <apr_hash.h>

struct environment 
{
	apr_hash_t* key_value_hash;
};

environment* environment_new(apr_pool_t* poll);
void environment_del(environment* env);
cell* environment_get(environment* env, cell* key);
void environment_put(environment* env, cell* key, cell* value);
#endif 
