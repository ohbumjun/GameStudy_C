/*
 * =====================================================================================
 *
 *       Filename:  mld.h
 *
 *    Description:  This file defines the data structures used for MLD tool
 *
 *        Version:  1.0
 *        Created:  Thursday 28 February 2019 05:14:18  IST
 *       Revision:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Networking Developer (AS), sachinites@gmail.com
 *        Company:  Brocade Communications(Jul 2012- Mar 2016), Current : Juniper Networks(Apr 2017 - Present)
 *
 *        This file is part of the MLD distribution (https://github.com/sachinites).
 *        Copyright (c) 2017 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify
 *        it under the terms of the GNU General Public License as published by
 *        the Free Software Foundation, version 3.
 *
 *        This program is distributed in the hope that it will be useful, but
 *        WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *        General Public License for more details.
 *
 *        You should have received a copy of the GNU General Public License
 *        along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * =====================================================================================
 */

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef __MLD__
#include <assert.h>
#include <string.h>

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128


typedef enum
{
	UINT8,
	UINT32,
	INT32,
	CHAR,
	OBJ_PTR,
	FLOAT,
	DOUBLE,
	OBJ_STRUCT
} data_type_t;


#define C_OFFSETOF(struct_name, fld_name)     \
    (unsigned int)&(((struct_name *)0)->fld_name)

/*
- '0' 이라는 nullptr 을 reinterpret_cast 를 통해서 "struct_name" 이라는 type 을 가리키는 pointer 로 변환
- & operator 을 통해 fld_name 이라는 특정 field 의 메모리 주소에 접근
- size_t 를 통해, 결과값은 offset byte 가 된다.
*/
#define CPP_OFFSETOF(struct_name, fld_name)     \
    (size_t)(&reinterpret_cast<struct_name*>(0)->fld_name)

#define C_FIELD_SIZE(struct_name, fld_name)   \
    sizeof(((struct_name *)0)->fld_name)

#define CPP_FIELD_SIZE(struct_name, fld_name)   \
    sizeof(reinterpret_cast<struct_name*>(0)->fld_name)

typedef struct struct_field_info
{
	// name of field
	char fname[MAX_FIELD_NAME_SIZE];
	// Data Type
	data_type_t dtype;
	unsigned int size;
	unsigned int offset;
	// 해당 필드가 다른 data 에 대한 포인터일 경우
	// 포인터가 가리키는 필드에 대한 name
	char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

typedef struct _struct_db_rec_ struct_db_rec_t;

struct _struct_db_rec_
{
	// 연결리스트 형태
	struct _struct_db_rec_* next;

	// key
	char struct_name[MAX_STRUCTURE_NAME_SIZE];

	// size of structure
	unsigned int ds_size;

	// no of field in structure
	unsigned int n_fields;

	// pointer to array of fields
	field_info_t* fields;
};

// header of linked list representing "structure database"
typedef struct _struct_db_ {
	struct_db_rec_t* head;
	unsigned int count;
} struct_db_t;


/*
typedef strct_emp
{
	int a;
	float b;
	strct_emp* mgr;
} emp_t;

해당 자료구조를 MLD structure database 에 등록한다고 할 때

하나의 _struct_db_rec_ 가 만들어질 것이고
그 _struct_db_rec_ 는 아래와 같은 메모리 구조를 가지게 될 것이다.
[NULL]["emp_t"][sizeof(emp_t)][3 = field 개수][fields 정보들을 모두 담은 field array 에 대한 포인터]

ex) fields[0] : field_info_t 하나 == ["a"][sizeof(int)][0 = offset][INT = 타입][NULL = nested structure 정보] ...
	fields[1] : ~~
	fields[2] : strct_emp* mgr 에 대한 정보 == ["mgr"][sizeof(void*)][offset][OBJ_PTR][emp_t = 포인터가 가리키는 데이터 strucuture 의 이름]

*/

/*Structure Data base Definition Ends*/

/* Printing functions*/
void
print_structure_rec(struct_db_rec_t* struct_rec);

void
print_structure_db(struct_db_t* struct_db);

int /*return 0 on success, -1 on failure for some reason*/
add_structure_to_struct_db(struct_db_t* struct_db, struct_db_rec_t* struct_rec);


#define CPP_FIELD_INFO(struct_name, fld_name, dtype, nested_struct_name)    \
   {#fld_name, dtype, CPP_FIELD_SIZE(struct_name, fld_name),                \
        CPP_OFFSETOF(struct_name, fld_name), #nested_struct_name} 

#define C_FIELD_INFO(struct_name, fld_name, dtype, nested_struct_name)    \
   {#fld_name, dtype, C_FIELD_SIZE(struct_name, fld_name),                \
        C_OFFSETOF(struct_name, fld_name), #nested_struct_name} 

#define CPP_REG_STRUCT(struct_db, st_name, fields_arr)                    \
    do{                                                               \
        struct_db_rec_t *rec = reinterpret_cast<struct_db_rec_t*>(calloc(1, sizeof(struct_db_rec_t)));    \
        strncpy(rec->struct_name, #st_name, MAX_STRUCTURE_NAME_SIZE); \
        rec->ds_size = sizeof(st_name);                              \
        rec->n_fields = sizeof(fields_arr)/sizeof(field_info_t);     \
        rec->fields = fields_arr;                                    \
        if(add_structure_to_struct_db(struct_db, rec)){              \
            assert(0);                                               \
        }                                                            \
    }while(0);

#endif /* __MLD__ */