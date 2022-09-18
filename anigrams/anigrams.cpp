#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>

std::map<std::string, std::vector<std::string> > valid_words;
std::map<std::string, int> word_index;
std::string word[200005] = {};
int dp1[200005] = {};
long long int dp1a[200005] = {};
long long int dp2[200005] = {};

std::string print_as_set(std::string str)
{
    std::string printable = "";
    printable += '{';
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (i != 0)
            printable += ", ";
        printable += str.at(i);
    }
    printable += "}";
    return printable;
}

std::string print_all_valid_words(std::string str)
{
    std::string printable = print_as_set(str) + ":";
    for (std::vector<std::string>::iterator it = valid_words[str].begin(); it != valid_words[str].end(); it++)
        printable += " " + *it;
    return printable;
}

std::string generate_new_str(std::string str, char chr)
{
    std::string g = str;
    g += chr;
    std::sort(g.begin(), g.end());
    return g;
}

int longest(int wd)
{
    if (dp1[wd] == 0) {
        dp1[wd] = 1;
        for (int i = 0; i < 26; i++)
        {
            std::string g = generate_new_str(word[wd], i+'a');
            if (word_index.find(g) != word_index.end())
                dp1[wd] = std::max(dp1[wd], longest(word_index[g])+1);
        }
    }
    return dp1[wd];
}

long long int count_longest(int wd)
{
    if (dp1a[wd] == -1) {
        dp1a[wd] = 0;
        for (int i = 0; i < 26; i++)
        {
            std::string g = generate_new_str(word[wd], i+'a');
            if (word_index.find(g) != word_index.end())
                if (longest(word_index[g]) == longest(wd)-1)
                    dp1a[wd] += count_longest(word_index[g]);
        }
        if (dp1a[wd] == 0) {
            dp1a[wd] = 1;
        }
    }
    return dp1a[wd];
}

void generate_a_longest_game(int wd)
{
    std::cout << print_all_valid_words(word[wd]) << '\n';
    for (int i = 0; i < 26; i++)
    {
        std::string g = generate_new_str(word[wd], i+'a');
        if (word_index.find(g) != word_index.end())
        {
            if (longest(word_index[g]) == longest(wd)-1)
            {
                generate_a_longest_game(word_index[g]);
                break;
            }
        }
    }
}

long long int number_of_paths_to_nine_letters(int wd)
{
    if (dp2[wd] == -1) {
        if (word[wd].length() == 9)
            dp2[wd] = 1;
        else if (word[wd].length() > 9)
            dp2[wd] = 0;
        else
        {
            dp2[wd] = 0;
            for (int i = 0; i < 26; i++)
            {
                std::string g = generate_new_str(word[wd], i+'a');
                if (word_index.find(g) != word_index.end())
                    dp2[wd] += number_of_paths_to_nine_letters(word_index[g]);
            }
        }
    }
    return dp2[wd];
}

int main()
{
    for (int i = 0; i < 200005; i++)
    {
        dp1a[i] = -1;
        dp2[i] = -1;
    }
    std::ifstream fin ("dictionary.txt");
    std::string str;
    while (fin >> str) {
        std::string orig = str;
        std::sort(str.begin(), str.end());
        word_index[str];
        valid_words[str].push_back(orig);
    }
    int c = 0;
    for (std::map<std::string, int>::iterator it = word_index.begin(); it != word_index.end(); it++)
    {
        word[c] = it->first;
        it->second = c++;
    }

    // Find the longest game and the number of longest games
    std::vector<int> bests;
    long long int number_of_longest_games = 0;
    for (int i = 0; i < c; i++)
    {
        if (word[i].length() != 4)
            continue;
        if (bests.empty() || longest(i) > longest(bests[0]))
        {
            bests.clear();
            bests.push_back(i);
            number_of_longest_games = count_longest(i);
        }
        else if (longest(i) == longest(bests[0]))
        {
            bests.push_back(i);
            number_of_longest_games += count_longest(i);
        }
    }
    std::cout << "Number of \"longest\" starting words: " << bests.size() << '\n';
    std::cout << "Total number of \"longest\" games: " << number_of_longest_games << '\n';
    for (std::vector<int>::iterator it = bests.begin(); it != bests.end(); it++)
    {
        std::cout << print_as_set(word[*it]) << ' ' << longest(*it) << '\n';
        //generate_a_longest_game(*it);
    }

    // Count the number of games
    unsigned long long int number_of_games = 0;
    for (int i = 0; i < c; i++)
    {
        if (word[i].length() == 4) {
            number_of_games += number_of_paths_to_nine_letters(i);
        }
    }
    std::cout << "Number of games: " << number_of_games << '\n';
}
