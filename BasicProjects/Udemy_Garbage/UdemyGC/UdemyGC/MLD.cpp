
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
	// �ش� �ʵ尡 �ٸ� data �� ���� �������� ���
	// �����Ͱ� ����Ű�� �ʵ忡 ���� name
	char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

struct _struct_db_rec_
{
	// ���Ḯ��Ʈ ����
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

�ش� �ڷᱸ���� MLD structure database �� ����Ѵٰ� �� ��

�ϳ��� _struct_db_rec_ �� ������� ���̰�
�� _struct_db_rec_ �� �Ʒ��� ���� �޸� ������ ������ �� ���̴�.
[NULL]["emp_t"][sizeof(emp_t)][3 = field ����][fields �������� ��� ���� field array �� ���� ������]

ex) fields[0] : field_info_t �ϳ� == ["a"][sizeof(int)][0 = offset][INT = Ÿ��][NULL = nested structure ����] ...
    fields[1] : ~~
	fields[2] : ~~

*/