# select 
#     departmentId,
#     max(salary) 
# from Employee 
# group by departmentId

# https://leetcode.com/problems/department-highest-salary/ 

select
    (select 
            name 
        from Department D 
        where D.id = E.departmentId) as Department,
    name as Employee,
    salary as Salary
from Employee E
where (E.departmentId, E.salary) in 
(
    select 
        departmentId,
        max(salary) 
    from Employee 
    group by departmentId
)
