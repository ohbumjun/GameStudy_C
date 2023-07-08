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
    unsigned int emp_id;
    unsigned int age;
    struct emp_* mgr;
    float salary;
} emp_t;


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
    static field_info_t emp_fields2[] = {
        CPP_FIELD_INFO(emp_t, emp_name, CHAR,    0),
        CPP_FIELD_INFO(emp_t, emp_id,   UINT32,  0),
        CPP_FIELD_INFO(emp_t, age,      UINT32,  0),
        CPP_FIELD_INFO(emp_t, mgr,      OBJ_PTR, emp_t),
        CPP_FIELD_INFO(emp_t, salary,   FLOAT, 0)
    };

    CPP_REG_STRUCT(struct_db, emp_t, emp_fields);
    CPP_REG_STRUCT(struct_db, emp_t, emp_fields2);

    print_structure_db(struct_db);
    return 0;
}
