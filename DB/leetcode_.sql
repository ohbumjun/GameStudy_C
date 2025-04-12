# Write your MySQL query statement below

# 1st : time limit

select 
    id,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Jan') as Jan_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Feb') as Feb_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Mar') as Mar_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Apr') as Apr_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'May') as May_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Jun') as Jun_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Jul') as Jul_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Aug') as Aug_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Sep') as Sep_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Oct') as Oct_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Nov') as Nov_Revenue,
    (select revenue from Department D2 
        where D1.id = D2.id and D2.month = 'Dec') as Dec_Revenue
from Department as D1
order by id
group by id


# 2nd : group by + case + sum
select 
    id,
    sum(case when month = 'Jan' then revenue end) as Jan_Revenue,
    sum(case when month = 'Feb' then revenue end) as Feb_Revenue,
    sum(case when month = 'Mar' then revenue end) as Mar_Revenue,
    sum(case when month = 'Apr' then revenue end) as Apr_Revenue,
    sum(case when month = 'May' then revenue end) as May_Revenue,
    sum(case when month = 'Jun' then revenue end) as Jun_Revenue,
    sum(case when month = 'Jul' then revenue end) as Jul_Revenue,
    sum(case when month = 'Aug' then revenue end) as Aug_Revenue,
    sum(case when month = 'Sep' then revenue end) as Sep_Revenue,
    sum(case when month = 'Oct' then revenue end) as Oct_Revenue,
    sum(case when month = 'Nov' then revenue end) as Nov_Revenue,
    sum(case when month = 'Dec' then revenue end) as Dec_Revenue
from Department as D1
group by id
order by id

