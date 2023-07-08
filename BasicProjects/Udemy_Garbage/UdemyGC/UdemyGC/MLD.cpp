
#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

typedef struct _struct_db_rec_ struct_db_rec_t;

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

typedef struct struct_field_info
{
	// name of field
	char fname[MAX_FIELD_NAME_SIZE];
	unsigned int size;
	unsigned int offset;
	data_type_t dtype;
	// 해당 필드가 다른 data 에 대한 포인터일 경우
	// 포인터가 가리키는 필드에 대한 name
	char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

struct _struct_db_rec_
{
	// 연결리스트 형태
	struct _struct_db_rec_* next;
	
	// key
	char struct_name[MAX_STRUCTURE_NAME_SIZE];

	unsigned int ds_size;
	unsigned int n_fields;
	field_info_t* fields;
};

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
	fields[2] : ~~

*/