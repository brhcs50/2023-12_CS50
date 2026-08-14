SELECT title FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN ratings ON ratings.movie_id = movies.id
JOIN people on stars.person_id = people.id
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC
LIMIT 5;
