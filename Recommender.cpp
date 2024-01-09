#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    user_data = &user_database;
    movie_data = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = user_data->get_user_from_email(user_email);
    
    // idea:
    // create set of all movies, only unique movies in set and a movie only added to this set if it's not in vector of movies alr watched
    // once we identify a movie to place in unordered set, calc its compatibility score and insert into set
    
    
    vector<string> user_hist = user->get_watch_history();   // vector of movie IDs
    set<string> moviesIgnore;
    
    set<string> relevant_directors;
    set<string> relevant_actors;
    vector<string> relevant_genres;
    
    multimap<int, Movie*, greater<int>> ranked_movies;
    
    // for every movie in watch history, add its ID to moviesIgnore so it is not included in further searches
    // add its directors, actors, and genres to relevant sets, no duplicates
    for (size_t i = 0; i < user_hist.size(); i++)
    {
        Movie* tempMovie = movie_data->get_movie_from_id(user_hist.at(i));
        for (size_t d = 0; d < tempMovie->get_directors().size(); d++)
            relevant_directors.insert(tempMovie->get_directors()[d]);
        for (size_t a = 0; a < tempMovie->get_actors().size(); a++)
            relevant_actors.insert(tempMovie->get_actors()[a]);
        for (size_t g = 0; g < tempMovie->get_genres().size(); g++)
            relevant_genres.push_back(tempMovie->get_genres()[g]);
        
        moviesIgnore.insert(tempMovie->get_id());
    }
    
    // for every director, actor, and genre in set, add all movies that include at least one of these to myMap
    // every movie that isn't in moviesIgnore has its score calculated and then is inserted to myMap
    for (set<string>::iterator it = relevant_directors.begin(); it != relevant_directors.end(); it++)
    {
        vector<Movie*> movies = movie_data->get_movies_with_director(*it);
        for (size_t i = 0; i < movies.size(); i++)
        {
            if (!moviesIgnore.insert(movies.at(i)->get_id()).second)
               continue;
            
            int score = calcScore(movies.at(i), relevant_directors, relevant_actors, relevant_genres);
            ranked_movies.insert(pair<int,Movie*>(score, movies[i]));
        }
    }
    
    for (set<string>::iterator it = relevant_actors.begin(); it != relevant_actors.end(); it++)
    {
        vector<Movie*> movies = movie_data->get_movies_with_actor(*it);
        for (size_t i = 0; i < movies.size(); i++)
        {
            if (!moviesIgnore.insert(movies.at(i)->get_id()).second)
               continue;
            
            int score = calcScore(movies.at(i), relevant_directors, relevant_actors, relevant_genres);
            ranked_movies.insert(pair<int,Movie*>(score, movies[i]));
        }
    }
    
    for (vector<string>::iterator it = relevant_genres.begin(); it != relevant_genres.end(); it++)
    {
        vector<Movie*> movies = movie_data->get_movies_with_genre(*it);
        for (size_t i = 0; i < movies.size(); i++)
        {
            if (!moviesIgnore.insert(movies.at(i)->get_id()).second)
               continue;
            
            int score = calcScore(movies.at(i), relevant_directors, relevant_actors, relevant_genres);
            ranked_movies.insert(pair<int,Movie*>(score, movies[i]));
        }
    }
    // rankedMovies now contains unique movies w/ compatibility score > 1 in sorted, descending order by score
    
    // need to break ties if top recommendations have same score
    vector<MovieAndRank> finalRecomendations;
    
    multimap<int, Movie*, greater<int>>::iterator it;
    while (finalRecomendations.size() < movie_count)
    {
        it = ranked_movies.begin();
        
        if (it == ranked_movies.end()) break;  // if num_movies > amt of scored movies, just return what we have
        
        int score = it->first;
        Movie* best = it->second;
        multimap<int, Movie*, greater<int>>::iterator deleteMe = it;
        
        it++;
        while (it != ranked_movies.end() && it->first == score)
        {
            if (it->second->get_rating() > best->get_rating())
            {
                best = it->second;
                deleteMe = it;
            }
            
            if (it->second->get_title() > best->get_title() && it->second->get_rating() > best->get_rating())
            {
                best = it->second;
                deleteMe = it;
            }
            
            it++;
        }
        
        // TESTING ONLY
        //cout << "Adding recommendation: " << best->get_title() << " - " << score << endl;
        // TESTING
    
        finalRecomendations.push_back(MovieAndRank(best->get_id(), score));
        ranked_movies.erase(deleteMe);
    }
    
    return finalRecomendations;
}

int Recommender::calcScore(Movie *movie, const std::set<std::string> &relevant_directors, const std::set<std::string> &relevant_actors, const std::vector<std::string> &relevant_genres) const
{
    int score = 0;
    
    for (set<string>::iterator it = relevant_directors.begin(); it != relevant_directors.end(); it++)
    {
        for (size_t i = 0; i < movie->get_directors().size(); i++)
            if (*it == movie->get_directors()[i]) score += 20;
    }
    
    for (set<string>::iterator it = relevant_actors.begin(); it != relevant_actors.end(); it++)
    {
        for (size_t i = 0; i < movie->get_actors().size(); i++)
            if (*it == movie->get_actors()[i]) score += 30;
    }
    
    for (size_t i = 0; i < relevant_genres.size(); i++)
    {
        for (size_t t = 0; t < movie->get_genres().size(); t++)
            if (relevant_genres[i] == movie->get_genres()[t]) score++;
    }
    
    return score;
}
