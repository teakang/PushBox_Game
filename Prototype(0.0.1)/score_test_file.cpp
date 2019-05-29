#include <fstream>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

void saveScore(int stage, int step, int push) {
    string file = "/home/khw56184/work-space/CLionProjects/CPP/push_box/scores/stage" + to_string(stage) + ".txt";
    ifstream fin(file);
    string name; // name of user, need graphical implement
    cout << "name: ";
    cin >> name;
    if(name.empty()) name = "unknown";
    if(fin.fail() || fin.peek() == ifstream::traits_type::eof()) {
        ofstream fout(file);
        fout << name << " " << step << " " << push << " \n";
        fout.close();
    } else {
        vector<string> v;
        vector<string>::iterator it;
        string str;
        for(int i = 0; fin >> str && i < 15; i++)
            v.emplace_back(str);
        for(it = v.end(); it != v.begin() && step + push < stoi(*(it - 2)) + stoi(*(it - 1)); it -= 3) {}
        v.insert(it, {name, to_string(step), to_string(push)});
        ofstream fout(file);
        int c = 5, i = 0;
        while(c-- && i < v.size()) {
            fout << v[i] << " " << v[i + 1] << " " << v[i + 2] << " \n";
            i += 3;
        }
        fout.close();
    }
    fin.close();
}

int main() {
    saveScore(2, 0, 1);
}