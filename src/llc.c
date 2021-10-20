#include <stdio.h>
#include <stdlib.h>
#include "llc.h"

#define NULL_FREE(item, ptr) if(!(item)) {free(ptr);}
#define NULL_CHECK_VAL(item, val) if(!(item)) {return val;}
#define NULL_CHECK(item) if(!(item)) {return;}
#define TRY_FREE(item) if(item) {free(item);}

LLC_s* LLC_init(size_t cluster_size) {
	char* data = malloc(sizeof(char) * cluster_size);
	NULL_CHECK_VAL(data, NULL);

	cluster_s* cluster = malloc(sizeof(cluster_s));
	NULL_FREE(cluster, data);
	NULL_CHECK_VAL(cluster, NULL);

	LLC_s* llc = malloc(sizeof(LLC_s));
	NULL_FREE(llc, data);
	NULL_FREE(llc, cluster);
	NULL_CHECK_VAL(llc, NULL);

	cluster->data = data;
	cluster->next = NULL;

	llc->start = cluster;
	llc->active = cluster;
	llc->active_cluster_len = 0;
	llc->cluster_size = cluster_size;
	llc->max_len = cluster_size;
	llc->total_len = 0;

	return llc;
}

void LLC_destroy(LLC_s* llc) {
	NULL_CHECK(llc);
	if (!llc->start) {
		free(llc);
		return;
	}

	cluster_s* previous = llc->start;
	cluster_s* current = llc->start->next;

	while (current)
	{
		TRY_FREE(previous->data);
		TRY_FREE(previous);
		previous = current;
		current = current->next;
	}

	TRY_FREE(previous->data);
	TRY_FREE(previous);

	free(llc);
}

char LLC_push(LLC_s* llc, char c) {
	/* if the char can fit into the current cluster, add it */
	if (llc->active_cluster_len < llc->cluster_size) {
		llc->active->data[llc->active_cluster_len] = c;
		llc->active_cluster_len++;
		llc->total_len++;
	}
	/* else we need to put it into the next cluster */
	else
	{
		if (llc->active->next) /* move to the next cluster */
		{
			llc->active = llc->active->next;
			llc->active_cluster_len = 0;
		}
		else /* next cluster does not exist - we need to allocate a new one */
		{
			char* data = malloc(sizeof(char) * llc->cluster_size);
			NULL_CHECK_VAL(data, -1);

			cluster_s* cluster = malloc(sizeof(cluster_s));
			NULL_FREE(cluster, data);
			NULL_CHECK_VAL(cluster, -1);

			cluster->data = data;
			cluster->next = NULL;

			llc->active->next = cluster;
			llc->active = llc->active->next;
			llc->max_len += llc->cluster_size;
			llc->active_cluster_len = 0;
		}
		/* finally add the char */
		llc->active->data[llc->active_cluster_len] = c;
		llc->active_cluster_len++;
		llc->total_len++;
	}
	return 0;
}

char* LLC_dump(LLC_s* llc) {
	char* data = malloc(sizeof(char) * (llc->total_len + 1));
	
	NULL_CHECK_VAL(data, NULL);
	data[llc->total_len] = '\0';

	if (llc->total_len < 1)
	{
		return data;
	}

	size_t out_len = 0;

	llc->active = llc->start;

	while (out_len < llc->total_len)
	{
		data[out_len] = llc->active->data[out_len % llc->cluster_size];
		out_len++;
		if (out_len % llc->cluster_size == 0)
		{
			if (out_len >= llc->total_len || llc->active->next == NULL)
			{
				break;
			}
			else
			{
				llc->active = llc->active->next;
			}
		}
		
	}
	llc->active = llc->start;
	llc->total_len = 0;
	llc->active_cluster_len = 0;
	return data;
}

/*#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "common.h"


const char* warning_num[] = { "Not number after comma!",  "Number is not ending with number!" };

//check if token is EOF
bool isEOF(token_s* t){
	if(t->val[1] == '\0')
		if (t->val[0] == EOF || t->val[0] == '\x1a') {
			t->type = eof;
			return true;
		}
	return false;
}

//skips all numbers in stream
void skipNum(char** val) {
	while (isdigit(**val))
		(*val)++;
}

//checks if string is number
bool isNum(token_s* t) {
	char* val = t->val;
	if (!isdigit(*val))
		return false;
	val++;
	skipNum(&val);
	//check for dot in decima number and skips it
	//floating point number
	if (*val == '.') {
		val++;
		//check for number after comma
		if (!isdigit(*val)) {
			t->warning = warning_num[0];
			t->type = err;
			return true;
		}
		val++;
		//skips to end number
		skipNum(&val);
		//check if number is ended right way
		if (*val == '\0') {
			t->type = lit;
			return true;
		}

		t->warning = warning_num[1];
		t->type = lit;
		return true;
	}
	//dicimal number
	else if (*val == '\0') {
		t->type = lit;
		return true;
	}
	t->warning = warning_num[1];
	t->type = err;
	return true;
}

//return length of array
#define lenght(x)  (sizeof(x) / sizeof((x)[0]))

bool isDataType(token_s* t) {
	const char* types[] = { "integer", "number", "string"};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = dt;
			return true;
		}
	}
	return false;
}

bool isKeyWord(token_s* t) {
	const char* types[] = { "do", "else", "end", "function", "global",
		"if", "local", "nil", "require", "return", "then", "while"};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = kw;
			return true;
		}
	}
	return false;
}

bool isBracket(token_s* t) {
	const char* types[] = { "(", ")", "[", "]", "{", "}"};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = br;
			return true;
		}
	}
	return false;
}

// TODO: add rest of operators
bool isOperator(token_s* t) {
	const char* types[] = { "=", "+=", "-=", "*=", "/=", "%="};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = op;
			return true;
		}
	}
	return false;
}

// TODO: implement
bool isLable(token_s* t) {
	return false;
}

// TODO: implement
bool isSemicolum(token_s* t) {
	return false;
}

// TODO: implement
bool isComma(token_s* t) {
	return false;
}

// TODO: implement
bool isAssigment(token_s* t) {
	return false;
}

//Returns type of token based on recived value
// TODO: do check for expession, if it follows rules in specification 
// TODO: check if switch covers all options
void selector(token_s* t) {
	if (isEOF(t))
		return;// eof
	else if (isNum(t))
		return;// lit
	else if (isDataType(t))
		return;// dt
	else if (isKeyWord(t))
		return;// kw
	else if (isBracket(t))
		return;// br
	else if (isOperator(t))
		return;// op
	else if (isLable(t))
		return;// lab
	else if (isSemicolum(t))
		return;// sem
	else if (isComma(t))
		return;// com
	else if (isAssigment(t))
		return;// as

	t->type = id;
	return;// id
}
*/
