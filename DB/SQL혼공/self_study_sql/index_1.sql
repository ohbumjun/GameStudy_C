use market_db;

-- clustered index
create table table1
(
	col1 int primary key,
    col2 int,
    col3 int
);

show index from table1;

-- secondary index
