/*
 * =====================================================================================
 *
 *       Filename:  mld.c
 *
 *    Description:  This file implements the functions and routines for mld library
 *
 *        Version:  1.0
 *        Created:  Thursday 28 February 2019 06:17:28  IST
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
#include "css.h"

const char* DATA_TYPE[] = { "UINT8", "UINT32", "INT32",
                     "CHAR", "OBJ_PTR", "FLOAT",
                     "DOUBLE", "OBJ_STRUCT" };

void
print_structure_rec(struct_db_rec_t* struct_rec) {
    if (!struct_rec) return;
    int j = 0;
    field_info_t* field = NULL;
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    for (j = 0; j < struct_rec->n_fields; j++) {
        field = &struct_rec->fields[j];
        printf("  %-20s |", "");
        printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n",
            j, field->fname, DATA_TYPE[field->dtype], field->size, field->offset, field->nested_str_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_CYAN "--------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}

void
print_structure_db(struct_db_t* struct_db) {

    if (!struct_db) return;
    printf("printing structure db\n");
    int i = 1;
    struct_db_rec_t* struct_rec = NULL;
    struct_rec = struct_db->head;
    printf("No of Structures Registered = %d\n", struct_db->count);
    while (struct_rec) {
        printf("structure No : %d (memory : %p)\n", i++, struct_rec);
        print_structure_rec(struct_rec);
        struct_rec = struct_rec->next;
    }
}

int
add_structure_to_struct_db(struct_db_t* struct_db,
    struct_db_rec_t* struct_rec) {

    struct_db_rec_t* head = struct_db->head;

    /*Add first structure into structure DB*/
    if (head == nullptr) {
        struct_db->head = struct_rec;
        struct_rec->next = NULL;
        struct_db->count++;
    }
    else
    {
        struct_rec->next = struct_db->head;
        struct_db->head = struct_rec;
        struct_db->count++;
    }

    /*Rest of the cases, Implement yourself*/
    return 0;
}

struct_db_rec_t* struct_db_look_up(struct_db_t* struct_db, std::string_view struct_name)
{
    struct_db_rec_t* cur_struct_rec = nullptr;
    cur_struct_rec = struct_db->head;

    while (cur_struct_rec)
    {
        if (strncmp(cur_struct_rec->struct_name, struct_name.data(), MAX_STRUCTURE_NAME_SIZE) == 0)
        {
            return cur_struct_rec;
        }
        cur_struct_rec = cur_struct_rec->next;
    }
    return nullptr;
}

void print_object_rec(object_db_rec_t* obj_rec, int i)
{
    if (!obj_rec) return;
    printf(ANSI_COLOR_MAGENTA"-----------------------------------------------------------------------------------|\n");
    printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s |\n",
        i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name);
    printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------|\n");
}

void print_object_db(object_db_t* object_db)
{
    object_db_rec_t* head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN "Printing OBJECT DATABASE\n");
    for (; head; head = head->next) {
        print_object_rec(head, i++);
    }
}

object_db_rec_t*
object_db_look_up(object_db_t* object_db, void* ptr) {

    object_db_rec_t* head = object_db->head;
    if (!head) return nullptr;

    for (; head; head = head->next) {
        if (head->ptr == ptr)
            return head;
    }
    return nullptr;
}

/*Working with objects*/
void
add_object_to_object_db(object_db_t* object_db,
    void* ptr,
    int units,
    struct_db_rec_t* struct_rec) {

    object_db_rec_t* obj_rec = object_db_look_up(object_db, ptr);

    /*Dont add same object twice*/
    // assert(!obj_rec);
    if (obj_rec != nullptr)
    {
        return;
    }

    obj_rec = reinterpret_cast<object_db_rec_t*>(calloc(1, sizeof(object_db_rec_t)));

    obj_rec->next = nullptr;
    obj_rec->ptr = ptr;
    obj_rec->units = units;
    obj_rec->struct_rec = struct_rec;

    object_db_rec_t* head = object_db->head;

    if (!head) {
        object_db->head = obj_rec;
        obj_rec->next = nullptr;
        object_db->count++;
        return;
    }

    // 새로 추가한 obj_rec 를 head 로 세팅한다.
    obj_rec->next = head;
    object_db->head = obj_rec;
    object_db->count++;
}

void mld_dump_object_rec_detail(object_db_rec_t* obj_rec)
{
    int n_fields = obj_rec->struct_rec->n_fields;
    field_info_t* field = nullptr;  

    int units = obj_rec->units, obj_index = 0, field_index = 0;

    for (; obj_index < units; obj_index++) {

        // n번째 unit. 즉, n번째 object 메모리 주소에 접근한다.
        char* current_object_ptr = (char*)(obj_rec->ptr) + \
            (obj_index * obj_rec->struct_rec->ds_size);

        for (field_index = 0; field_index < n_fields; field_index++) {

            // 현재 조사하고자 하는 필드
            field = &obj_rec->struct_rec->fields[field_index];

            // offset 만으로도, 내가 원하는 value 에 바로 접근가능하다
            // 반드시 struct_t 라는 runtime type 을 사용할 필요가 없다. 
            switch (field->dtype) {
            case UINT8:
            case INT32:
            case UINT32:
                printf("%s[%d]->%s = %d\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, *(int*)(current_object_ptr + field->offset));
                break;
            case CHAR:
                // char 의 경우, 해당 메모리 주소 그대로 출력한다.
                printf("%s[%d]->%s = %s\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, (char*)(current_object_ptr + field->offset));
                break;
            case FLOAT:
                printf("%s[%d]->%s = %f\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, *(float*)(current_object_ptr + field->offset));
                break;
            case DOUBLE:
                printf("%s[%d]->%s = %f\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, *(double*)(current_object_ptr + field->offset));
                break;
            case OBJ_PTR:
                printf("%s[%d]->%s = %p\n", obj_rec->struct_rec->struct_name, obj_index, field->fname, (void*)*(int*)(current_object_ptr + field->offset));
                break;
            case OBJ_STRUCT:
                /*Later*/
                break;
            default:

                break;
            }
        }
    }
}

void xfree(void* ptr, object_db_t* object_db)
{
    object_db_rec_t* target_obj = object_db_look_up(object_db, ptr);

    assert(target_obj);

    bool target_found = false;
    object_db_rec_t* cur_obj_db_rec = object_db->head;
    object_db_rec_t* past_obj_db_rec = nullptr;

    while (cur_obj_db_rec)
    {
        if (cur_obj_db_rec->ptr == ptr)
        {
            target_found = true;
            past_obj_db_rec->next = cur_obj_db_rec->next;
            break;
        }
        past_obj_db_rec = cur_obj_db_rec;
        cur_obj_db_rec = cur_obj_db_rec->next;
    }

    assert(target_found);

    free(cur_obj_db_rec->ptr);
    free(cur_obj_db_rec);
}

