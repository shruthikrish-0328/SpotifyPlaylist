#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include <queue>
#include <iomanip>
#include <math.h>
#include <chrono>
#include_next <wchar.h>

using namespace std;

// Attributes assumed to start at 0 below, SHOULD NOT CONTAIN artists, id, name

map<string, int> ATTRIBUTES = {{"valence",0}, {"year",1}, {"acousticness",2},  {"danceability",3}, {"duration",4}, {"energy",5},
                               {"instrumentalness",6}, {"key",7}, {"liveness",8}, {"loudness",9}, {"mode",10},
                               {"popularity",11}, {"speechiness",12}, {"tempo",13}};

int NUM_SONGS = 20;


struct Song {
    double valence;
    int year;
    double acousticness;
    double danceability;
    int duration;
    double energy;
    bool xplicit;
    double instrumentalness;
    int key;
    double liveness;
    double loudness;
    int mode;
    int popularity;
    int release_date;
    double speechiness;
    double tempo;
    string name;
    vector<string> artists;
    string id;
    double score = 0.0;
    vector<double> values;
    void print_song();
    void pretty_print_song();
    void pretty_print_song_comparison(Song& input);
    void set_score(Song& input, unordered_map<int, double>& attr_to_stddev, map<int,int>& pref_map);
    void populate_song_values();
    bool operator()(const Song& s1, const Song& s2) const {
        return s1.score > s2.score;
    };
};

void Song::print_song() {
    cout << "valence: " << valence << endl;
    cout << "year: " << year << endl;
    cout << "acousticness: " << acousticness << endl;
    cout << "artists: " << endl;
    for (string artist: artists) {
        cout << artist << " ";
    }
    cout << endl;
    cout << "danceability: " << danceability << endl;
    cout << "duration: " << duration << endl;
    cout << "energy: " << energy << endl;
    cout << "xplicit: " << xplicit << endl;
    cout << "id: " << id << endl;
    cout << "instrumentalness: " << instrumentalness << endl;
    cout << "key: " << key << endl;
    cout << "liveness: " << liveness << endl;
    cout << "loudness: " << loudness << endl;
    cout << "mode: " << mode << endl;
    cout << "name: " << name << endl;
    cout << "popularity: " << popularity << endl;
    cout << "release_date: " << release_date << endl;
    cout << "speechiness: " << speechiness << endl;
    cout << "tempo: " << tempo << endl;
}

void Song::pretty_print_song_comparison(Song& input) {
    cout << "Song name: " << name << endl;
    cout << "Artists: " << endl;
    for (string artist: artists) {
        cout << artist << " ";
    }
    cout << endl;
    cout << "Song score: " << score << endl;
    cout << "Song valence: " << valence << " Input valence: " << input.valence << endl;
    cout << "Song year: " << year << " Input year: " << input.year << endl;
    cout << "Song acousticness: " << acousticness << " Input acousticness: " << input.acousticness << endl;
    cout << "Song id: " << id << " Input id: " << input.id << endl;
    cout << "Song popularity: " << popularity << " Input popularity: " << input.popularity << endl;
}

void Song::pretty_print_song() {
    cout << "Song name: " << name << endl;
    cout << "Artists: " << endl;
    for (string artist: artists) {
        cout << artist << " ";
    }
    cout << endl;
    cout << "Song score: " << score << endl << endl;

}

void Song::populate_song_values() { //pushes all 14 attribute values to a vector
    values.push_back(valence);
    values.push_back(year);
    values.push_back(acousticness);
    values.push_back(danceability);
    values.push_back(duration);
    values.push_back(energy);
    values.push_back(instrumentalness);
    values.push_back(key);
    values.push_back(liveness);
    values.push_back(loudness);
    values.push_back(mode);
    values.push_back(popularity);
    values.push_back(speechiness);
    values.push_back(tempo);
}




Song parse_song(string line) {
    Song song;
    stringstream ss(line);
    string val;
    getline(ss, val, ','); //parses file by looking for commas
    song.valence = stod(val);

    getline(ss, val, ',');
    song.year = stoi(val);

    getline(ss, val, ',');
    song.acousticness = stod(val);

    getline(ss, val, '[');
    getline(ss, val, ']');

    vector<string> artists;
    string val2;
    stringstream ss2(val);
    while (getline(ss2, val2, ',')) { //adds artists to a vector excluding apostrophies
        string artist = val2.substr(1, val2.length() - 2);
        if (artist[0] == '\'') {
            artist = artist.substr(1, artist.length() - 1);
        }
        artists.push_back(artist);
    }

    getline(ss, val, ',');
    getline(ss, val, ',');
    song.danceability = stod(val);

    getline(ss, val, ',');
    song.duration = stoi(val);

    getline(ss, val, ',');
    song.energy = stod(val);

    getline(ss, val, ',');
    song.xplicit = stoi(val);

    getline(ss, val, ',');
    song.id = val;

    getline(ss, val, ',');
    song.instrumentalness = stod(val);

    getline(ss, val, ',');
    song.key = stoi(val);

    getline(ss, val, ',');
    song.liveness = stod(val);

    getline(ss, val, ',');
    song.loudness = stod(val);

    getline(ss, val, ',');
    song.mode = stoi(val);

    string name;
    if (ss.peek() == '"') { //takes in the name of the song excluding quotation marks
        getline(ss, val, '"');

        while (getline(ss, val, '"')) {
            name += val;
            if (ss.peek() == '"') {
                getline(ss, val, '"');
            }
            else {
                break;
            }
            name += '"';
        }

        song.name = name;
        getline(ss, val, ',');
    }

    else {
        getline(ss, val, ',');
        song.name = val;
    }

    getline(ss, val, ',');
    song.popularity = stoi(val);

    getline(ss, val, ',');
    song.release_date = stoi(val);

    getline(ss, val, ',');
    song.speechiness = stod(val);

    getline(ss, val, ',');
    song.tempo = stod(val);


    song.artists = artists;
    return song;
}

void printGreeting() {
    cout << "Welcome to the Spotify Playlist Generator! You can use this tool to generate a playlist of recommended songs based on your preferences!" << endl;
    cout << "You may either search for a song, or try to find songs that you may enjoy." << endl;
}

void printMenu(){
    cout << endl;
    cout << "Please rank each of the following factors with an integer from 0-10. ";
    cout << "10 being the most important and 1 being the least important. Entering 0 will imply that the attribute does not matter at all." << endl;
    cout << endl;
    cout << "1. Acousticness: The relative metric of the track being acoustic" << endl;
    cout << "2. Danceability: The relative measurement of the track being danceable" << endl;
    cout << "3. Duration: The length of the track in milliseconds (ms)" << endl;
    cout << "4. Energy: The Energy of the track" << endl;
    cout << "5. Instrumentalness: The relative ratio of the track being instrumental" << endl;
    cout << "6. Key: The primary key of the track encoded as integers in between 0 and 11" << endl;
    cout << "7. Liveness: The relative duration of the track sounding as a live performance" << endl;
    cout << "8. Loudness: Relative loudness of the track in the typical range [-60, 0] in decibel (dB)" << endl;
    cout << "9. Mode: The binary value representing whether the track starts with a major (1) chord progression or not (0)" << endl;
    cout << "10. Popularity: The popularity of the song lately, default country = US" << endl;
    cout << "11. Year: The year the track was released" << endl;
    cout << "12. Speechiness: The relative length of the track containing any kind of human voice" << endl;
    cout << "13. Tempo: The tempo of the track in Beat Per Minute (BPM)" << endl;
    cout << "14. Valence: The positiveness of the track" << endl;
    cout << endl;
}


map<int,int> get_pref_map() {
    printMenu();
    map<int,int> pref_map; //defines a map holding the input rank for each attribute

    int input;
    for (auto iter = ATTRIBUTES.begin(); iter != ATTRIBUTES.end(); iter++) { //iterates through attribute map
        cout << "Enter desired rank for the " << iter->first << ": ";
        // Input validation from https://stackoverflow.com/questions/16934183/integer-validation-for-input
        while((!(cin >> input  && input <= 10 && input >= 0))) {
            cout << "Enter desired rank for the " << iter->first << " (int from 0 to 10): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }
        pref_map[iter->second] = input; //adds rank value to the map
    }

    return pref_map;
}

Song get_song_choice(unordered_map<string, vector<Song>>& song_map) {
    string input;
    cout << endl << "Please enter the exact title of any song of your choice!" << endl;
    cout << "Input: ";

    getline(cin,input);
    getline(cin,input);
    while (song_map.find(input) == song_map.end()) { //if the song isnt find this statement will keep printing
        cout << "Song not found. Try again or enter another song: " << endl;
        cout << "Input: ";
        getline(cin,input);
    }
    vector<Song>& song_vec = song_map[input];
    if (song_vec.size() == 1) {
        return song_vec[0];
    }

    map<vector<string>, Song> artist_map;
    for (Song& song : song_vec) {
        if (artist_map.find(song.artists) == artist_map.end()) { //if artist vector is found it will be skipped
            artist_map[song.artists] = song;
        }
    }

    cout << endl << "There are " << artist_map.size() << " songs with the name: " << input << endl << endl;


    map<int, vector<string>> num_to_artists; //used to print artists in a 1. 2. 3. format
    int counter = 1;
    for (auto iter = artist_map.begin(); iter != artist_map.end(); iter++) {
        num_to_artists[counter] = iter->first;
        counter++;
    }

    for (auto iter = num_to_artists.begin(); iter != num_to_artists.end(); iter++) {
        cout << iter->first << ". ";

        int count = 0;
        for (string artist : iter->second) {
            count++;
            cout << artist;

            if(count != iter->second.size()){ //prints commas between artists excluding the last one
                cout << ", ";
            }
        }
        cout << endl;
    }

    cout << endl << "Please enter the number associated with the artist(s) you are looking for." << endl;
    cout << "Input: ";

    int num;
    while((!(cin >> num  && num <= num_to_artists.size() && num > 0))) {
        cout << "Input (integer): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }

    return artist_map[num_to_artists[num]]; //returns the song the user is looking for
}


// CHANGE HARD CODED INDICES
void Song::set_score(Song& input, unordered_map<int, double>& attr_to_stddev, map<int,int>& pref_map){
    double score = 0;

    for (int i = 0; i < values.size(); i++) {
        score += abs(values[i] - input.values[i]) / attr_to_stddev[i] * (pref_map[i]); //calculation of the score
    }
    this->score = score;
}


vector<Song> parse_songs(ifstream& file) { //parses songs in the files and creates the song vector
    vector<Song> songs;
    string line;
    getline(file, line);

    while (getline(file, line)) {

        Song song = parse_song(line);
        songs.push_back(song); //pushes to vector
    }
    return songs; //returns vector with all songs
}


void populate_values(vector<Song>& songs) {
    for (Song& song : songs) {
        song.populate_song_values();
    }
}

unordered_map<int, double> get_means(vector<Song>& songs) {
    unordered_map<int, double> attr_to_mean;

    for (int i = 0; i < songs[0].values.size(); i++) {
        double sum = 0;
        for (int j = 0; j < songs.size(); j++) { //sums up all attribute values
            sum += songs[j].values[i];
        }
        attr_to_mean[i] = sum / songs.size(); //diving sum by the size gives us the mean/average
    }
    return attr_to_mean;
}

unordered_map<int, double> get_stddevs(vector<Song>& songs, unordered_map<int, double>& attr_to_mean) {
    unordered_map<int, double> attr_to_stddev;

    for (int i = 0; i < songs[0].values.size(); i++) {
        double sum_sq = 0;
        for (int j = 0; j < songs.size(); j++) {
            double diff = attr_to_mean[i] - songs[j].values[i];
            sum_sq += pow(diff, 2);
        }
        double stddev = pow((sum_sq / (songs.size() - 1)), 0.5);
        attr_to_stddev[i] = stddev;
    }
    return attr_to_stddev;
}

vector<Song> clean_songs(vector<Song>& songs) {
    map<vector<string>, Song> song_map;
    for (Song& song : songs) {
        vector<string> vec = song.artists;
        vec.push_back(song.name);
        if (song_map.find(vec) == song_map.end()) {
            song_map[vec] = song;
        }
    }
    vector<Song> return_vec;
    for (auto iter = song_map.begin(); iter != song_map.end(); iter++) {
        return_vec.push_back(iter->second);
    }
    return return_vec;
}



vector<Song> mergeSort(vector<Song>& currVector){
/****************************************************************************************************
*    Title: "7.7. The Merge Sort"
*    Page: Runestone Academy
*    Authors: Brad Miller and David Ranum, Luther College, and Jan Pearce, Berea College
*    Date Accessed: 12/07/2020
*    Availability: https://runestone.academy/runestone/books/published/cppds/Sort/TheMergeSort.html
****************************************************************************************************/
    if(currVector.size() > 1){
        int mid = currVector.size() / 2;

        vector<Song> leftSide (currVector.begin(), currVector.begin() + mid);
        vector<Song> rightSide (currVector.begin() + mid, currVector.begin() + currVector.size());

        leftSide = mergeSort(leftSide); //separates the vector into two sides
        rightSide = mergeSort(rightSide);

        int leftPos = 0, rightPos = 0, mainPos = 0;
        //if elements on both sides
        while(leftPos < leftSide.size() && rightPos < rightSide.size()){
            //if left is bigger
            if(leftSide[leftPos].score < rightSide[rightPos].score){
                currVector[mainPos] = leftSide[leftPos];
                leftPos++;
            }
                //if right is bigger
            else{
                currVector[mainPos] = rightSide[rightPos];
                rightPos++;
            }
            mainPos++;
        }

        //if elements only on left
        while(leftPos < leftSide.size()){
            currVector[mainPos] = leftSide[leftPos];
            leftPos++;
            mainPos++;
        }

        //if elements only on right
        while(rightPos < rightSide.size()){
            currVector[mainPos] = rightSide[rightPos];
            rightPos++;
            mainPos++;
        }
    }

    return currVector;
}

double time_merge_sort(vector<Song>& songs){
/***************************************************************************************
*    Title: "Using chrono::high_resolution_clock in C++"
*    Author: Vivek Kumar
*    Date: 07/03/2019
*    Availability: https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/?ref=rp
*
***************************************************************************************/

    auto startTime = chrono::high_resolution_clock::now();
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    songs = mergeSort(songs);
    auto endTime = chrono::high_resolution_clock::now();

    // Calculating total time taken by the program.
    double time_taken =
            chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();

    time_taken *= 1e-9;

    return time_taken;
}



void heapify_down(vector<Song>& arr, int size, int root) {
    int max = root;
    int left = 2*root + 1;
    int right = 2*root + 2;

    if (left < size && arr[left].score > arr[max].score) { //updates max to left child is conditions are et
        max = left;
    }
    if (right < size && arr[right].score > arr[max].score) { //updates max to right child if conditions are met
        max = right;
    }
    if (max != root) { //swaps the root with the new max if the max is changed from the root
        swap(arr[root], arr[max]);
        heapify_down(arr, size, max); //recursively calls the function
    }
}

void heap_sort(vector<Song>& arr) {
    // Uses code from Professor Lisha Zhou's lecture linked in the Stepik section for Module 6 at
    // https://mediasite.video.ufl.edu/Mediasite/Play/9cddca9408664cd89d02650d1ecacd581d
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) { //builds the heap
        heapify_down(arr, n, i);
    }
    for (int i=n-1; i>0; i--) { //sorts vector by heapifying
        swap(arr[0], arr[i]);
        heapify_down(arr, i, 0);
    }
}

double time_heap_sort(vector<Song>& songs){
/***************************************************************************************
*    Title: "Using chrono::high_resolution_clock in C++"
*    Author: Vivek Kumar
*    Date: 07/03/2019
*    Availability: https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/?ref=rp
*
***************************************************************************************/

    auto startTime = chrono::high_resolution_clock::now();
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    heap_sort(songs);
    auto endTime = chrono::high_resolution_clock::now();

    // Calculating total time taken by the program.
    double time_taken =
            chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();

    time_taken *= 1e-9;

    return time_taken;
}


int get_user_option() {
    int num;
    cout << endl;
    cout << "Type '1' to search for a song by name." << endl; //gives user two options to choose from
    cout << "Type '2' to find songs you might like." << endl;
    while (true) {
        cout << "Input: ";
        string input;
        getline(cin,input);
        if (!isdigit(input[0])) {
            cout << "Error: Enter '1' or '2'" << endl;
            continue;
        }
        num = input[0] - '0';
        if (!((num == 1) || (num == 2))) {  //if a number other than 1 or 2 is entered there will be an error statement printed
            cout << "Error: Enter '1' or '2'" << endl;
            continue;
        }
        break;
    }
    return num;
}


string get_search() {
    cout << endl << "Enter a string, then all songs containining that substring in their name will be displayed (case sensitive)." << endl;
    cout << "Input: ";
    string input;
    getline(cin,input);
    return input;
}



void search_song(string search, vector<Song>& songs) {
    bool found = false;
    for (Song song : songs) {
        if (song.name.find(search) != string::npos) {
            found = true;
            cout << song.name << ", by "; //prints song name if it is found
            string delim = "";
            for (string artist : song.artists) { //prints all artists next to the song name on the same line
                cout << delim << artist;
                delim = ", ";
            }
            cout << endl;
        }
    }
    if (!found) {
        cout << endl << "No songs were found containing that search string." << endl;
    }
}

void print_song_list(vector<Song>& songs, int num_recommendations) {
    for (int i=0; i<num_recommendations ; i++) {
        cout << i+1 << ". " << songs[i].name << ", by "; //prints song name
        string delim = "";
        for (string artist : songs[i].artists) { //prints all artists
            cout << delim << artist;
            delim = ", ";
        }
        cout << "  Score: " << songs[i].score << endl; //prints score associated with the song
    }
}

int main() {

    ifstream file;
    file.open("data.csv");

    vector<Song> songs = parse_songs(file); //parses all songs in the file

    // Removes duplicates from the data set
    songs = clean_songs(songs);

    unordered_map<string, vector<Song>> song_map; //creates a map of songs
    for (int i = 0; i < songs.size(); i++) {
        if (song_map.find(songs[i].name) == song_map.end()) {
            vector<Song> song_vec;
            song_map[songs[i].name] = song_vec;
        }
        song_map[songs[i].name].push_back(songs[i]);
    }

    printGreeting();

    int option;
    while (true) { //everything put into while loop so program loops
        option = get_user_option();
        if (option == 1) {
            string search = get_search();
            search_song(search, songs);
            continue;
        }



        map<int,int> pref_map = get_pref_map();

        Song input_song = get_song_choice(song_map);
        string id = input_song.id;




        populate_values(songs);

        unordered_map<int, double> attr_to_mean = get_means(songs);


        unordered_map<int, double> attr_to_stddev = get_stddevs(songs, attr_to_mean);

        cout << endl << "How many song recommendations would you like? (Enter an integer from 1 to 50)" << endl;
        cout << "Input: ";
        // Input validation from https://stackoverflow.com/questions/16934183/integer-validation-for-input
        int num_recommendations;
        while((!(cin >> num_recommendations  && num_recommendations <= 50 && num_recommendations >= 1))) {
            cout << "How many song recommendations would you like? (Enter an integer from 1 to 50)" << endl;
            cout << "Input: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }

        // Get back input_song, because it wasn't updated by populate_values.
        for (Song song : songs) {
            if (song.id.compare(id) == 0) {
                input_song = song;
            }
        }


        for (Song& song : songs) {
            song.set_score(input_song, attr_to_stddev, pref_map);
        }

        // Make a copy of the songs, to perform a separate sorting algorithm on the data
        vector<Song> heap_songs;

        for (Song song : songs) {
            if (song.id != id) {
                heap_songs.push_back(song);
            }
        }

        vector<Song> merge_songs;

        for (Song song : songs) {
            if (song.id != id) {
                merge_songs.push_back(song);
            }
        }


        double heap_sort_time = time_heap_sort(heap_songs);

        double merge_sort_time = time_merge_sort(merge_songs);


        cout << endl << "Here are some songs you might enjoy. Songs with lower numbers indicates stronger adherence to your preferences." << endl;
        cout << "The score of a song is the sum of the products of the standard scores of each of the song's attributes and that attribute's corresponding weight." << endl << endl;
        cout << "Recommended song list generated using Heapsort: " << endl;
        print_song_list(heap_songs, num_recommendations); //prints heap sort list

        cout << endl;
        cout << "Recommended song list generated using Merge sort: " << endl;
        print_song_list(merge_songs, num_recommendations); //prints merge sort list

        cout << endl;
        cout << "For a vector of songs of size " << songs.size() << ":" << endl;
        cout << "Time taken by Heapsort is: " << fixed << setprecision(5) << heap_sort_time << " sec" << endl;
        cout << "Time taken by Merge sort is: " << fixed << setprecision(5) << merge_sort_time << " sec" << endl;
        if (heap_sort_time < merge_sort_time) { //shows how much faster one sort algorithm was when compared to the other for a specific input
            cout << fixed << setprecision(2) << "Merge sort took " << merge_sort_time / heap_sort_time * 100 << "% longer to complete." << endl;
        }
        else if(merge_sort_time < heap_sort_time) {
            cout << fixed << setprecision(2) << "Heapsort took " << heap_sort_time / merge_sort_time * 100 << "% longer to complete." << endl;
        }
    }
}