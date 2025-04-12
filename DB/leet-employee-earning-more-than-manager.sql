# Write your MySQL query statement below
# https://leetcode.com/problems/employees-earning-more-than-their-managers/ 
select 
    name as Employee
from Employee as E1
where salary >
    (
        select salary
        from Employee E2
        where E2.id = E1.managerID
    )
