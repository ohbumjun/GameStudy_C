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

/*struct_name 에 해당하는 struct_db_rec_t 찾기 */
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
        rec->ds_size = sizeof(st_name);                              \
        rec->n_fields = sizeof(fields_arr)/sizeof(field_info_t);     \
        rec->fields = fields_arr;                                    \
        if(add_structure_to_struct_db(struct_db, rec)){              \
            assert(0);                                               \
        }                                                            \
    }while(0);


/*Object Database structure definitions Starts here*/

typedef struct _object_db_rec_ object_db_rec_t;

struct _object_db_rec_ {
	// _object_db_rec_ 들을 연결리스트 형태로 관리할 것이다.
	// 그 다음 노드에 대한 포인터
	object_db_rec_t* next;

	// 할당한 object 의 메모리 주소 
	// _object_db_ 에서 해당 object_rec 를 찾는 key 로 사용할 것이다.
	void* ptr;

	// 연속적인 메모리 공간에 calloc 을 통해서 할당할 텐데, 몇개의 object 가 
	// 연속적으로 할당되었는가
	unsigned int units;

	// 해당 object 가 속한 structure_db_rec 객체를 가리키는 포인터
	// xalloc 시 들어온 "string" 값을 이용하여 struct_db 에서 찾아내서 링크시켜주기
	struct_db_rec_t* struct_rec;
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
	struct_db_rec_t* struct_rec);

/*print the values of all supported fields by mld library for the object record passed as argument.*/
void
mld_dump_object_rec_detail(object_db_rec_t* obj_rec);
/*
xcalloc(object_db, "emp_t", 1) == calloc(1, sizeof(emp_t)) 와 동일
즉, emp_t 타입의 object 를 1 개만큼 연속적인 메모리 공간에 할당.
- 새롭게 alloc 된 object 에 대한 새로운 object record 할당 .
  그리고 object_db 에 추가
- object record 와 structure_record 를 연결시켜준다. 이때 사용되는 것이
  두번째 인자 "emp_t" 이다. 해당 string key 로 내가 연결시켜주고자 하는
  structure_record 를 찾는다.
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
	add_object_to_object_db(object_db, ptr, units, struct_rec);
	return ptr;
}

void
xfree(void* ptr, object_db_t* object_db);

/*
<Application Data Structure>
Application 상의 object 들은, 서로 다양한 reference 정보를 지닌다.
- 결과적으로 그러한 reference 를 graph 상의 edge 로 표현하고
- 각각의 object 를 node 로 표현한다면
  graph 가 형성될 텐데
  사실상 Application 은 isolated graph 들의 모음이다.

- 각각의 isolated graph 들은, root node 가 존재한다.
  보통 root object 들은 global / static object 이다.

- 하지만, isolated graph 가, directed cyclic graph 형태가 될 수도 있다.
  예를 들어, A graph, B graph 가 있다. 각각의 graph 에 속한 노드
  a, b 가 있다고 해보자.

  a -> b
  b -> a

  이렇게 reference 가 걸리는 순간 A, B graph 는 더이상 isolated graph 가
  아니게 된다.

  오히려 A, B graph 는 하나의 graph 로 합쳐지고
  이 graph 는 내부에 단방향 순환참조가 걸린

  directed cyclic graph 가 된다.

- global 변수는, application 상의 모든 file, 함수 등 에서 접근할 수 있는
  변수이다. 
  
  application 은 적어도 하나의 global object를 지닌다.
  따라서 application 상의 모든 object 를 순회하는, 시작점이 될 것이다.

- 보통 global object 들은 global 변수들에 의해서 참조된다.
  따라서 global object 들은 절대 leak 될 수 없다.
*/

/*
<Root Object 관리>
- Application 은 MLD 에게 모든 root object 를 알려줘야 한다.
  MLD 는 application 이 root object 를 등록할 수 있는 API 를 제공한다.

- root object 는 2가지 방식으로 만들어질 수 있다
  1) global root object
  ex) Object a = new Object;  int main(){}
      이와 같이 main loop 밖에 생성된 object 는 root 가 된다.
	  해당 object 는 xalloc 등 MLD DB 에 object 정보를 등록하는
	  xalloc 등의 함수로 생성된 것이 아니므로 MLD 는 해당
	  object 의 존재를 모른다.

	  따라서 별도의 API를 통해 MDL DB 에 등록해줘야 한다.

	  ex) mld_register_global_object_as_root()
		  - new object db record 만들고 + root 로 표시

  - 대부분의 경우 global object 는 root object 가 된다.

  2) dynamic root object
  ex) object* obj = xalloc();
      이 경우 xalloc 을 통해 MLD DB 에 등록은 해주지만
	  별도로 해당 obj 가 root 라는 것도 알려줘야 한다.

	  mld_set_dynamic_object_as_root 라는 함수를 통해
	  해당 정보를 세팅해줄 것이다.
 
  - MLD 는 이러한 dynamic root object 가 절대 leak 될 수 없다.
    라고 가정할 것이다. 만약 DRO 가 leak 이 되면 MLD 는
	해당 사항을 report 하지 않는다.

	왜냐하면 memory detection algorithm 은 root object 가
	항상 reachable 하다고 가정하기 때문이다.
 */

#endif /* __MLD__ */

