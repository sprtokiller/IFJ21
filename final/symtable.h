#include <stdio.h>

#include "XString.h"
#include "token.h"

#ifdef HASH_T
#include "RuntimeClass.h"


#define HashMap(__type__) __rcat2(htab_, __type__)
#define c_class HashMap(HASH_T)
#pragma push_macro("HASH_T")

#ifndef TABLE_SIZE
#define TABLE_SIZE 100 //Change this later
#endif // !TABLE_SIZE

//A symbol, represented as a hash table item
typedef struct Template(item) Template(item);

//dont use this!
//just for editors autocomplete
struct Template(item)
{
	String identifier;
	HASH_T value;
	struct Template(item)* next_synonym;
};

typedef Template(item)* c_class[TABLE_SIZE];

#ifdef IsClass
/// @brief If object stored is a class it should be destroyed with destructor
/// @param  element to destroy
inline void Template(_Destroy_single)(HASH_T* element)
{
	__rdtor(HASH_T)(element);
}
#else
inline void Template(_Destroy_single)(HASH_T* element)
{
	unused_param(element);
}
#endif // IsClass


void Destructor(selfptr);
void Constructor(selfptr);
HASH_T* Template(emplace)(selfptr, const char* id);
HASH_T* Template(find)(selfptr, const char* id);

//void Sym_table_insert(selfptr, const char* id, token_type token_type, uint32_t frame_count);
//void Sym_table_print(const selfptr);


#undef IsClass
#undef HASH_T
#endif // HASH_T


