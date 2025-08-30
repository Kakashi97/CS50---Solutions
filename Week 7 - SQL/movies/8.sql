SELECT stars_people.name AS name
  FROM movies
       JOIN (SELECT * FROM stars JOIN people on stars.person_id = people.id) AS stars_people
       on movies.id = stars_people.movie_id
 WHERE movies.title = 'Toy Story';
