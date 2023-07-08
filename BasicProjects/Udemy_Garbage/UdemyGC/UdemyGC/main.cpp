/*
 * =====================================================================================
 *
 *       Filename:  app.c
 *
 *    Description:  This file represents the application demonstrating the use of MLD library
 *
 *        Version:  1.0
 *        Created:  Thursday 28 February 2019 06:41:32  IST
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

#include "MLD.h"
#include <memory.h>
#include <stdlib.h>

 /*Application Structures*/

typedef struct emp_ {

    char emp_name[30];
    
    // emp_id 의 offset 이 30 이 아니라, 32 인 이유
    // OS 단에서 4byte 혹은 8byte 배수 단위로 offset 을 맞추기 때문이다
    unsigned int emp_id;
    unsigned int age;
    struct emp_* mgr;
    float salary;
} emp_t;

typedef struct student_ {

    char stud_name[32];
    unsigned int roolno;
    unsigned int age;
    float aggregate;
    struct student_* best_colleage;
} student_t;

int
main(int argc, char** argv) {

    /*Step 1 : Initialize a new structure database */
    // calloc : dynamically allocate memory for an array of elements (배열 동적 할당 함수)
    struct_db_t* struct_db = reinterpret_cast<struct_db_t*>(calloc(1, sizeof(struct_db_t)));

    /*Create structure record for structure emp_t*/
    static field_info_t emp_fields[] = {
        CPP_FIELD_INFO(emp_t, emp_name, CHAR,    0),
        CPP_FIELD_INFO(emp_t, emp_id,   UINT32,  0),
        CPP_FIELD_INFO(emp_t, age,      UINT32,  0),
        CPP_FIELD_INFO(emp_t, mgr,      OBJ_PTR, emp_t),
        CPP_FIELD_INFO(emp_t, salary,   FLOAT, 0)
    };
    static field_info_t stud_fields[] = {
        CPP_FIELD_INFO(student_t, stud_name, CHAR,    0),
        CPP_FIELD_INFO(student_t, roolno,   UINT32,  0),
        CPP_FIELD_INFO(student_t, age,      UINT32,  0),
        CPP_FIELD_INFO(student_t, aggregate,   FLOAT, 0),
        CPP_FIELD_INFO(student_t, best_colleage, OBJ_PTR, student_t),
    };

    CPP_REG_STRUCT(struct_db, emp_t, emp_fields);
    CPP_REG_STRUCT(struct_db, student_t, stud_fields);

    print_structure_db(struct_db);

    /*Working with object database*/
    /*Step 1 : Initialize a new Object database */
    object_db_t* object_db = reinterpret_cast<object_db_t*>(calloc(1, sizeof(object_db_t)));
    object_db->struct_db = struct_db;

    /*Step 2 : Create some sample objects, equivalent to standard
     * calloc(1, sizeof(student_t))*/
    student_t* abhishek = xcalloc<student_t>(object_db, "student_t", 1);
    student_t* shivani = xcalloc<student_t>(object_db, "student_t", 1);
    emp_t* joseph = xcalloc<emp_t>(object_db, "emp_t", 2);

    print_object_db(object_db);

    return 0;
}

