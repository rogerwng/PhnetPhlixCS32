#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

MovieDatabase::MovieDatabase()
{
    loaded = false;
}

MovieDatabase::~MovieDatabase()
{
    for (list<Movie*>::iterator it = movieStorage.begin(); it != movieStorage.end(); it = it)
        delete *it++;
    
}

vector<string> MovieDatabase::getVector(string str)
{
    vector<string> result;
    stringstream ss(str);
    string parse;
    
    while (getline(ss,parse,','))
        result.push_back(toLower(parse));
        
    return result;
}

string MovieDatabase::toLower(const string& str) const
{
    string result = str;
    for (size_t i = 0; i < str.size(); i++)
    {
        result[i] = std::tolower(str[i]);
    }
    
    return result;
}

bool MovieDatabase::load(const string& filename)
{
    if (loaded)
        return false;
    
    ifstream file(filename);
    while (!file.eof())
    {
        string ID,name,date,directors,actors,genres,num;
        stringstream ss;
        float rating;
        getline(file, ID);
        getline(file, name);
        getline(file, date);
        getline(file, directors);
        getline(file, actors);
        getline(file, genres);
        getline(file, num);
        
        ss << num;
        ss >> rating;
        
        Movie* temp = new Movie(toLower(ID),name,date,getVector(directors),getVector(actors),getVector(genres),rating);
        movieStorage.push_back(temp);
        movieByID.insert(temp->get_id(), temp);
        
        for (size_t i = 0; i < temp->get_directors().size(); i++)
            movieByDirector.insert(temp->get_directors()[i], temp);
        for (size_t i = 0; i < temp->get_actors().size(); i++)
            movieByActor.insert(temp->get_actors()[i], temp);
        for (size_t i = 0; i < temp->get_genres().size(); i++)
            movieByGenre.insert(temp->get_genres()[i], temp);
        
        //
        // TESTING ONLY
        //cout << "Loaded: " << temp->get_title() << " - " << temp->get_id() << endl;
        // TESTING ONLY
        //
        
        getline(file, name);
    }
    
    
    loaded = true;
    return true;  // Replace this line with correct code.
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie*>::Iterator it = movieByID.find(toLower(id));
    
    if (it.is_valid())
        return it.get_value();
    
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> result;
    TreeMultimap<string, Movie*>::Iterator it = movieByDirector.find(toLower(director));
    
    while (it.is_valid())
    {
        result.push_back(it.get_value());
        it.advance();
    }
    
    return result;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> result;
    TreeMultimap<string, Movie*>::Iterator it = movieByActor.find(toLower(actor));
    
    while (it.is_valid())
    {
        result.push_back(it.get_value());
        it.advance();
    }
    
    return result;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> result;
    TreeMultimap<string, Movie*>::Iterator it = movieByGenre.find(toLower(genre));
    
    while (it.is_valid())
    {
        result.push_back(it.get_value());
        it.advance();
    }
    
    return result;
}
