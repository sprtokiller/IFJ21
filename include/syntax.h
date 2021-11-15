#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "symtable.h"

typedef void (*rule)();

#define TABLE_H 10
#define TABLE_W 10

typedef rule (*rules)[TABLE_W][TABLE_H];

#define RULE_PATH SOURCE_DIR "/rule/ept.csv"

void Syntax_init(rules Rules);
bool Syntax_run(rules Rules, Sym_table st);