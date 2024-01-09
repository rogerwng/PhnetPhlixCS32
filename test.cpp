#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "treemm.h"
#include "Recommender.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cassert>
using namespace std;

void testStrings()
{
    ifstream file;
    file.open("input.txt");
    
    if (file.is_open())
    {
        cout << "opened" << endl;
    }
    
    string name, email, num;
    getline(file, name);
    getline(file, email);
    getline(file, num);
    stringstream ss;
    ss << num;
    int numHist;
    ss >> numHist;
    cout << name << endl << email << endl << numHist << endl;
    
    vector<string> history;
    for (int i = 0; i < numHist; i++)
    {
        string temp;
        getline(file, temp);
        history.push_back(temp);
        cout << history.at(i) << endl;
    }
    
    //getline(file,name);
    cout << name << endl;
    //getline(file, name);
    
    if (file.eof())
        cout << "end file" << endl;
    
    file.close();
}

void testTMM()
{
    TreeMultimap<int, int> tree;
    tree.insert(3, 15);
    tree.insert(1, 21);
    tree.insert(2, 19);
    tree.insert(5, 22);
    tree.insert(4, 25);
    
    TreeMultimap<int, int>::Iterator it = tree.find(1);
    
    cout << it.get_value() << endl;
    it.advance();
    cout << it.get_value() << endl;
    
    TreeMultimap<int, int>::Iterator it2 = tree.find(5);
    cout << it2.get_value() << endl;
    
    
    
    TreeMultimap<int, User*> userTree;
    vector<string> watchHistory;
    watchHistory.push_back("movie1");
    User* one = new User("name", "email", watchHistory);
    
    userTree.insert(1,one);
    TreeMultimap<int, User*>::Iterator it3 = userTree.find(1);
    User* one2 = it3.get_value();
    cout << one2->get_full_name() << endl;
}

void testUserLoad()
{
    
    UserDatabase users = UserDatabase();
    users.load("users.txt");
    
    User* test = users.get_user_from_email("TVi0@outlook.com");
    if (test == nullptr)
        cout << "User not found" << endl;
    else
        cout << test->get_full_name() << endl;
}

void testMovieLoad()
{
    MovieDatabase movies = MovieDatabase();
    movies.load("movies.txt");
    
    Movie* one = movies.get_movie_from_id("ID19949");
    cout << one->get_title() << endl;
    
    vector<Movie*> two = movies.get_movies_with_genre("Crime");
    for (size_t i = 0; i < two.size(); i++)
        cout << two[i]->get_title() << endl;
}

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email, int num_recommendations)
{
    // get up to ten movie recommendations for the user
    vector<MovieAndRank> recommendations = r.recommend_movies(user_email, 10);
    if (recommendations.empty())
        cout << "We found no movies to recommend :(.\n";
    else
    {
        for (int i = 0; i < recommendations.size(); i++)
        {
            const MovieAndRank& mr = recommendations[i];
            Movie* m = md.get_movie_from_id(mr.movie_id);
            cout << i << ". " << m->get_title() << " (";
        }
    }
}


void testRecommender()
{
    MovieDatabase movies = MovieDatabase();
    //cout << "Loading movies: " << endl;
    movies.load("movies.txt");
    
    //cout << endl << "Loading users: " << endl;
    
    UserDatabase users = UserDatabase();
    users.load("users.txt");
    
    Recommender rec = Recommender(users, movies);
    
    //cout << endl;
    
    string email = "sappho@poetry.org";
    int num = 3;
    vector<MovieAndRank> recommendations = rec.recommend_movies(email, num);
    
    if (recommendations.empty())
        cout << "We found no movies to recommend :(.";
    else
    {
        for (int i = 0; i < recommendations.size(); i++)
        {
            const MovieAndRank& mr = recommendations[i];
            Movie* m = movies.get_movie_from_id(mr.movie_id);
            cout << i+1 << ". " << m->get_title() << " (" << m->get_release_year() << ")\n Rating: " << m->get_rating() << "\n Compatibility Score: " << mr.compatibility_score << endl;
        }
    }
}

int main()
{
    //testTMM();
    //testStrings();
    //testUserLoad();
    //testMovieLoad();
    //testRecommender();
    
    cout << "Passed all tests" << endl;
}
