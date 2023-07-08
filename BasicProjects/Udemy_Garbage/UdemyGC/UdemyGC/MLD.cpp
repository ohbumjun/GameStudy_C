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
    printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s | is_root = %s |\n",
        i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name, obj_rec->is_root ? "TRUE " : "FALSE");
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
    struct_db_rec_t* struct_rec,
    mld_boolean_t is_root) {

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
    obj_rec->is_visited = MLD_FALSE;
    obj_rec->is_root = is_root;

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

void mld_register_root_object(object_db_t* object_db, void* objptr, char* struct_name, unsigned int units)
{
    struct_db_rec_t* struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
    assert(struct_rec);

    /*Create a new object record and add to object database + root 라고 표시하기*/
    add_object_to_object_db(object_db, objptr, units, struct_rec, MLD_TRUE);
}

void set_mld_object_as_global_root(object_db_t* object_db, void* obj_ptr)
{
    object_db_rec_t* obj_rec = object_db_look_up(object_db, obj_ptr);

    assert(obj_rec);

    obj_rec->is_root = MLD_TRUE;
}

static object_db_rec_t*
get_next_root_object(object_db_t* object_db,
    object_db_rec_t* starting_from_here) {

    object_db_rec_t* first = starting_from_here ? starting_from_here->next : object_db->head;
    while (first) {
        if (first->is_root)
            return first;
        first = first->next;
    }
    return nullptr;
}


/* Level 2 Pseudocode : This function explore the direct childs of obj_rec and mark
 * them visited. Note that obj_rec must have already visted.*/
static void
mld_explore_objects_recursively(object_db_t* object_db,
    object_db_rec_t* parent_obj_rec) {

    unsigned int i, n_fields;
    char* parent_obj_ptr = NULL,
        * child_obj_offset = NULL;
    void* child_object_address = NULL;
    field_info_t* field_info = NULL;

    object_db_rec_t* child_object_rec = NULL;
    struct_db_rec_t* parent_struct_rec = parent_obj_rec->struct_rec;

    /*Parent object must have already visited*/
    assert(parent_obj_rec->is_visited);

    // 연속적으로 할당된 모든 object 를 순횐한다.
    for (i = 0; i < parent_obj_rec->units; i++) {

        // 현재 조사중인 object 의 메모리 주소에 접근한다.
        parent_obj_ptr = (char*)(parent_obj_rec->ptr) + (i * parent_struct_rec->ds_size);

        // 해당 object type 의 모든 field 를 순회한다. 
        for (n_fields = 0; n_fields < parent_struct_rec->n_fields; n_fields++) {

            field_info = &parent_struct_rec->fields[n_fields];

            /* 오직 포인터 field 에 대한 조사만 한다.
             * We are only concerned with fields which are pointer to
             * other objects*/
            switch (field_info->dtype) {
            case UINT8:
            case UINT32:
            case INT32:
            case CHAR:
            case FLOAT:
            case DOUBLE:
            case OBJ_STRUCT:
                break;
            case OBJ_PTR:
            default:
                ;

                /* 현재 parent object 가 직접 가리키고 있는 pointer object 가 바로 child object 이다.
                   ex) parent_obj_ptr = 0x1000 = parent object 의 메모리 주소 를 담기 위해 쓰인다.
                       그리고 parent object 가 이와 같은 형태를 취했다고 해보자. {[float][int][char[30]][0x2000]}
                    
                       이때 [0x2000] 은, chlid object pointer 변수이다. 
                       그리고 [0x2000] 이라는 데이터의 메모리 주소가 "0x1010" 이라고 해보자.

                       child_obj_offset = 0x1010 이 될 것이다.
                   
                 * child_obj_offset is the memory location inside parent object
                 * where address of next level object is stored*/
                child_obj_offset = parent_obj_ptr + field_info->offset;

                /*
                   child_object_address 변수에 담는 값은 무엇이 될까 ?
                
                   child_obj_offset 는 자세히 살펴보면 "이중 포인터" 이다.
                   *(child_obj_offset) == [0x2000] 이라는 데이터 == child object 를 가리키는 포인터
                   **(child_obj_offset) == *([0x2000] 이라는 데이터) == *(child object 를 가리키는 포인터) == child 객체
                
                   &child_object_address 라는 것은, child_object_address = ??, 를 수행한다는 것.
                   즉, 특정 값을 child_object_address 변수에 대입한다는 의미

                   child_obj_offset ? 
                   보통은 &child_obj_offset 형태로 쓰는데, 여기서는 & 를 빼고
                   child_obj_offset 를 사용했다.
                   child_obj_offset 는 이중포인터 라고 했다
                   child_obj_offset = &[0x2000]

                   memcpy(&child_object_address, &[0x2000], sizeof(void*)); 라는 의미는
                   child_object_address = 0x2000 을 한다는 것이고

                   이 말은 즉슨, child_object_address 에 chlid 객체의 메모리 주소. 값을 대입한다는 것이고
                   *(child_object_address) = child object. 가 된다는 것이다.
                */
                memcpy(&child_object_address, child_obj_offset, sizeof(void*));

                /*child_object_address now stores the address of the next object in the
                 * graph. It could be NULL, Handle that as well*/
                if (!child_object_address) continue;

                child_object_rec = object_db_look_up(object_db, child_object_address);

                assert(child_object_rec);

                /* Since we are able to reach this child object "child_object_rec"
                 * from parent object "parent_obj_ptr", mark this
                 * child object as visited and explore its children recirsively.
                 * If this child object is already visited, then do nothing - avoid infinite loops*/
                if (!child_object_rec->is_visited) {
                    child_object_rec->is_visited = MLD_TRUE;
                    mld_explore_objects_recursively(object_db, child_object_rec);
                }
                else {
                    continue; /*Do nothing, explore next child object*/
                }
            }
        }
    }
}

static void
init_mld_algorithm(object_db_t* object_db) {

    object_db_rec_t* obj_rec = object_db->head;
    while (obj_rec) {
        obj_rec->is_visited = MLD_FALSE;
        obj_rec = obj_rec->next;
    }
}

void run_mld_algorithm(object_db_t* object_db)
{
    /*Step 1 : Mark all objects in object databse as unvisited*/
    init_mld_algorithm(object_db);

    /* Step 2 : Get the "first root object" from the object db, it could be
     * present anywhere in object db. If there are multiple roots in object db
     * return the first one, we can start mld algorithm from any root object*/

    object_db_rec_t* root_obj = get_next_root_object(object_db, NULL);

    while (root_obj) {

        // 이미 방문한 root node 재귀 방지
        if (root_obj->is_visited) {
            /* It means, all objects reachable from this root_obj has already been
             * explored, no need to do it again, else you will end up in infinite loop.
             * Remember, Application Data structures are cyclic graphs*/
            root_obj = get_next_root_object(object_db, root_obj);
            continue;
        }

        /* Mark as Visited !!
         * root objects are always reachable since application holds the global
         * variable to it*/
        root_obj->is_visited = MLD_TRUE;

        /*Explore all reachable objects from this root_obj recursively*/
        mld_explore_objects_recursively(object_db, root_obj);

        root_obj = get_next_root_object(object_db, root_obj);
    }
}

void report_leaked_objects(object_db_t* object_db)
{
    int i = 0;
    object_db_rec_t* head;

    printf("Dumping Leaked Objects\n");

    for (head = object_db->head; head; head = head->next) {
        if (!head->is_visited) {
            print_object_rec(head, i++);
            mld_dump_object_rec_detail(head);
            printf("\n\n");
        }
    }
}

