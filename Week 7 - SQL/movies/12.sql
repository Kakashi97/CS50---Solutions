/*In 12.sql, write a SQL query to list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred.
Your query should output a table with a single column for the title of each movie.
You may assume that there is only one person in the database with the name Bradley Cooper.
You may assume that there is only one person in the database with the name Jennifer Lawrence.*/
SELECT title
  FROM movies
 WHERE movies.id IN
       (SELECT tab1.movie_id
          FROM
               (SELECT *
                  FROM people
                  JOIN stars
                    ON people.id = stars.person_id
                 WHERE people.name = 'Bradley Cooper') AS tab1
          JOIN
               (SELECT *
                  FROM people
                  JOIN stars
                    ON people.id = stars.person_id
                 WHERE people.name = 'Jennifer Lawrence') AS tab2
            ON tab1.movie_id = tab2.movie_id)
