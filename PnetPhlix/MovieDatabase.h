#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>
#include <list>

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    std::vector<std::string> getVector(std::string str);
    std::string toLower(const std::string& str) const; 
    
    bool loaded;
    std::list<Movie*> movieStorage;
    TreeMultimap<std::string, Movie*> movieByID;
    TreeMultimap<std::string, Movie*> movieByDirector;
    TreeMultimap<std::string, Movie*> movieByActor;
    TreeMultimap<std::string, Movie*> movieByGenre;
};

#endif // MOVIEDATABASE_INCLUDED
