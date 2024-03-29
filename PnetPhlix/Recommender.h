#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <set>

class UserDatabase;
class MovieDatabase;
class Movie;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase* user_data;
    const MovieDatabase* movie_data;
    
    int calcScore(Movie* movie, const std::set<std::string>& relevant_directors, const std::set<std::string>& relevant_actors, const std::vector<std::string>& relevant_genres) const;
};

#endif // RECOMMENDER_INCLUDED
