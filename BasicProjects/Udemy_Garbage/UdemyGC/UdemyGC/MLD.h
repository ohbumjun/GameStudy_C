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
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string_view>

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

typedef enum {

	MLD_FALSE,
	MLD_TRUE
} mld_boolean_t;

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

/*struct_name �� �ش��ϴ� struct_db_rec_t ã�� */
struct_db_rec_t*
struct_db_look_up(struct_db_t* struct_db, std::string_view struct_name);

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
        rec->ds_size = sizeof(st_name);													\
        rec->n_fields = fields_arr ? sizeof(fields_arr)/sizeof(field_info_t) : 0;     \
        rec->fields = fields_arr;                                    \
        if(add_structure_to_struct_db(struct_db, rec)){              \
            assert(0);                                               \
        }                                                            \
    }while(0);


/*Object Database structure definitions Starts here*/

typedef struct _object_db_rec_ object_db_rec_t;

struct _object_db_rec_ {
	// _object_db_rec_ ���� ���Ḯ��Ʈ ���·� ������ ���̴�.
	// �� ���� ��忡 ���� ������
	object_db_rec_t* next;

	// �Ҵ��� object �� �޸� �ּ� 
	// _object_db_ ���� �ش� object_rec �� ã�� key �� ����� ���̴�.
	void* ptr;

	// �������� �޸� ������ calloc �� ���ؼ� �Ҵ��� �ٵ�, ��� object �� 
	// ���������� �Ҵ�Ǿ��°�
	unsigned int units;

	// �ش� object �� ���� structure_db_rec ��ü�� ����Ű�� ������
	// xalloc �� ���� "string" ���� �̿��Ͽ� struct_db ���� ã�Ƴ��� ��ũ�����ֱ�
	struct_db_rec_t* struct_rec;

	mld_boolean_t is_visited; /*Used for Graph traversal*/
	mld_boolean_t is_root;    /*Is this object is Root object*/
};

typedef struct _object_db_ {
	struct_db_t* struct_db;
	object_db_rec_t* head;
	unsigned int count;
} object_db_t;


/*Dumping functions*/
void
print_object_rec(object_db_rec_t* obj_rec, int i);

void
print_object_db(object_db_t* object_db);

object_db_rec_t*
object_db_look_up(object_db_t* object_db, void* ptr);

void
add_object_to_object_db(object_db_t* object_db,
	void* ptr,
	int units,
	struct_db_rec_t* struct_rec,
	mld_boolean_t is_root);

/*print the values of all supported fields by mld library for the object record passed as argument.*/
void
mld_dump_object_rec_detail(object_db_rec_t* obj_rec);

/*
xcalloc(object_db, "emp_t", 1) == calloc(1, sizeof(emp_t)) �� ����
��, emp_t Ÿ���� object �� 1 ����ŭ �������� �޸� ������ �Ҵ�.
- ���Ӱ� alloc �� object �� ���� ���ο� object record �Ҵ� .
  �׸��� object_db �� �߰�
- object record �� structure_record �� ��������ش�. �̶� ���Ǵ� ����
  �ι�° ���� "emp_t" �̴�. �ش� string key �� ���� ��������ְ��� �ϴ�
  structure_record �� ã�´�.
 */
template<typename T>
T*
xcalloc(object_db_t* object_db, std::string_view struct_name, int units);

template<typename T>
inline T* xcalloc(object_db_t* object_db, std::string_view struct_name, int units)
{
	struct_db_rec_t* struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
	assert(struct_rec);
	T* ptr = reinterpret_cast<T*>(calloc(units, struct_rec->ds_size));
	add_object_to_object_db(object_db, ptr, units, struct_rec, MLD_FALSE);
	return ptr;
}

void
xfree(void* ptr, object_db_t* object_db);

/*
<Application Data Structure>
Application ���� object ����, ���� �پ��� reference ������ ���Ѵ�.
- ��������� �׷��� reference �� graph ���� edge �� ǥ���ϰ�
- ������ object �� node �� ǥ���Ѵٸ�
  graph �� ������ �ٵ�
  ��ǻ� Application �� isolated graph ���� �����̴�.

- ������ isolated graph ����, root node �� �����Ѵ�.
  ���� root object ���� global / static object �̴�.

- ������, isolated graph ��, directed cyclic graph ���°� �� ���� �ִ�.
  ���� ���, A graph, B graph �� �ִ�. ������ graph �� ���� ���
  a, b �� �ִٰ� �غ���.

  a -> b
  b -> a

  �̷��� reference �� �ɸ��� ���� A, B graph �� ���̻� isolated graph ��
  �ƴϰ� �ȴ�.

  ������ A, B graph �� �ϳ��� graph �� ��������
  �� graph �� ���ο� �ܹ��� ��ȯ������ �ɸ�

  directed cyclic graph �� �ȴ�.

- global ������, application ���� ��� file, �Լ� �� ���� ������ �� �ִ�
  �����̴�. 
  
  application �� ��� �ϳ��� global object�� ���Ѵ�.
  ���� application ���� ��� object �� ��ȸ�ϴ�, �������� �� ���̴�.

- ���� global object ���� global �����鿡 ���ؼ� �����ȴ�.
  ���� global object ���� ���� leak �� �� ����.
*/

/*
<Root Object ����>
- Application �� MLD ���� ��� root object �� �˷���� �Ѵ�.
  MLD �� application �� root object �� ����� �� �ִ� API �� �����Ѵ�.

- root object �� 2���� ������� ������� �� �ִ�
  1) global root object
  ex) Object a = new Object;  int main(){}
      �̿� ���� main loop �ۿ� ������ object �� root �� �ȴ�.
	  �ش� object �� xalloc �� MLD DB �� object ������ ����ϴ�
	  xalloc ���� �Լ��� ������ ���� �ƴϹǷ� MLD �� �ش�
	  object �� ���縦 �𸥴�.

	  ���� ������ API�� ���� MDL DB �� �������� �Ѵ�.

	  ex) mld_register_global_object_as_root()
		  - new object db record ����� + root �� ǥ��

  - ��κ��� ��� global object �� root object �� �ȴ�.

  2) dynamic root object
  ex) object* obj = xalloc();
      �� ��� xalloc �� ���� MLD DB �� ����� ��������
	  ������ �ش� obj �� root ��� �͵� �˷���� �Ѵ�.

	  mld_set_dynamic_object_as_root ��� �Լ��� ����
	  �ش� ������ �������� ���̴�.
 
  - MLD �� �̷��� dynamic root object �� ���� leak �� �� ����.
    ��� ������ ���̴�. ���� DRO �� leak �� �Ǹ� MLD ��
	�ش� ������ report ���� �ʴ´�.

	�ֳ��ϸ� memory detection algorithm �� root object ��
	�׻� reachable �ϴٰ� �����ϱ� �����̴�.
 */

 /*APIs to register root objects*/
void mld_register_root_object(object_db_t* object_db,
	void* objptr,
	char* struct_name,
	unsigned int units);

void
set_mld_object_as_global_root(object_db_t* object_db, void* obj_ptr);


/*
<How to Traverse Graphs>
- root object ���� �����ؾ� �Ѵ�. root object �߿��� �
  root ���� �����ص� �������.

  �̸� ���� object DB ������ root object �� ���� ã�ڸ���
  �ش� root object ���� �����ϵ��� �Ѵ�.
	- child object ? : Ư�� object ���� "�ٷ�" rechable �� object
	- child object �� ��������� Ÿ�� ���鼭 Ž���ϴ� �����
	  ���Ѵ�. Ư�� object �� �湮�� ������ Is Visited ������ true ��
	  �����Ѵ�.
	  - DFS �˰����� ������ ���̴�.
    - �ѹ� �湮�� object �� �ٽ� Ž���ϴ� ���� �����ϱ� ����
	   IsVisited ������ �̿��Ѵ�.
- �ش� root object �� ���� Ž���� ���ƴٸ�,
  �� ���� root object �� ã�Ƽ� ���� ������ �ݺ��Ѵ�.

- ���������� IsVisited �� false ���, Leak �� object �� �ȴ�.
*/
void
run_mld_algorithm(object_db_t* object_db);

/*
<Primitive Data type �� ���� pointer �� �ٷ�� ���>

---- ���� ��Ȳ ----

typedef emp_t
{
	unsigned int* _last6;  // unsigned int �迭�� ���� ������ [uint][uint][uint][uint][uint][uint]
}

�ش� emp_t �� ���� �����͸� �Ҵ��� ��, �츮�� �Ʒ��� ���� ���¸� ���ϰԵȴ�.
empt_t* emp = xalloc(object_db, "emp_t", 1);
emp->_last6 = xalloc(object_db, "?", 6);

�̶� "?" �� ����� �ؾ��ұ� ? �׳� UINT32 ��� �ϸ� �ǳ� ?
UINT32 �� data_type_t ��� enum �����θ� ���Ǿ��� ��.
UINT32 �� ���� struct_db_rec_t �� ������ �ʿ��ϴ�.

��, MLD �� �̿� ���� "Primitive Data Type" �� ���� ���� �� ������ 
�˰� �־�� �Ѵٴ� ���̴�.

�ٽ� ����, OJB_PTR �� �޸� field �� �������� �ʴ� primitive object type �� ����
pointer �� MLD �� �ٷ� �� �ֱ� ���ؼ���, 
primitive type �� ���� type ������ structure database �� �����ؾ� �Ѵ�.

���� �Ʒ� �Լ��� ���� structure database �� primitive data type �� ���� ������ �Ѱ��� ���̴�.

---- ��� ���� ----
1) struct_db ������ ���� ȣ��
2) ���� xcalloc(object_db, "int", 6) �� ���� ������� �Ҵ� ����
*/
void mld_init_primitive_data_types_support(struct_db_t* struct_db);

void
report_leaked_objects(object_db_t* object_db);

#endif /* __MLD__ */

