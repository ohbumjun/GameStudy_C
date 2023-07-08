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
- '0' �̶�� nullptr �� reinterpret_cast �� ���ؼ� "struct_name" �̶�� type �� ����Ű�� pointer �� ��ȯ
- & operator �� ���� fld_name �̶�� Ư�� field �� �޸� �ּҿ� ����
- size_t �� ����, ������� offset byte �� �ȴ�.
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
	// �ش� �ʵ尡 �ٸ� data �� ���� �������� ���
	// �����Ͱ� ����Ű�� �ʵ忡 ���� name
	char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

typedef struct _struct_db_rec_ struct_db_rec_t;

struct _struct_db_rec_
{
	// ���Ḯ��Ʈ ����
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

�ش� �ڷᱸ���� MLD structure database �� ����Ѵٰ� �� ��

�ϳ��� _struct_db_rec_ �� ������� ���̰�
�� _struct_db_rec_ �� �Ʒ��� ���� �޸� ������ ������ �� ���̴�.
[NULL]["emp_t"][sizeof(emp_t)][3 = field ����][fields �������� ��� ���� field array �� ���� ������]

ex) fields[0] : field_info_t �ϳ� == ["a"][sizeof(int)][0 = offset][INT = Ÿ��][NULL = nested structure ����] ...
	fields[1] : ~~
	fields[2] : strct_emp* mgr �� ���� ���� == ["mgr"][sizeof(void*)][offset][OBJ_PTR][emp_t = �����Ͱ� ����Ű�� ������ strucuture �� �̸�]

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