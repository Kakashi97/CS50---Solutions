SELECT COUNT(movies.id)
  FROM movies
       INNER JOIN ratings
       on movies.id = ratings.movie_id
 WHERE rating = 10.0;
