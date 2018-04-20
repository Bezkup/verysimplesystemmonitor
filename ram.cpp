#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "split.cpp"

#define MEMINFO_PATH "/proc/meminfo"
#define TOTAL_LINE_I 0
#define AVAIL_LINE_I 2
int get_meminfo_total() {
    ifstream infile(MEMINFO_PATH);
    string line;
    int p = 0;
    while (getline(infile, line)) {
        if (infile.eof()) break;
        //cout << "Line " << p << ": " << line << endl;
        if (p == TOTAL_LINE_I) {
            infile.close();
            vector<string> splitline = split(line, ' ');
            int total = stoi(splitline[splitline.size()-2]);
            return total;
            break;
        }
        p++;
    }
    infile.close();
    cerr << "File out of boundaries" << endl;
    exit(1);
}
int get_meminfo_available() {
    ifstream infile(MEMINFO_PATH);
    string line;
    int p = 0;
    while (getline(infile, line)) {
        if (infile.eof()) break;
        //cout << "Line " << p << ": " << line << endl;
        if (p == AVAIL_LINE_I) {
            infile.close();
            vector<string> splitline = split(line, ' ');
            int avail = stoi(splitline[splitline.size()-2]);
            return avail;
            break;
        }
        p++;
    }
    infile.close();
    cerr << "File out of boundaries" << endl;
    exit(1);
}


class Ram {
    private:
        int total;
        int avail;

    public:
        Ram() {
            total = get_meminfo_total();
            avail = get_meminfo_available();
        }

        void refresh() {
            avail = get_meminfo_available();
        }

        int get_used() {
            refresh();
            return total-avail;
        }

        int get_total() {
            return total;
        }

        int get_used_percent() {
            refresh();
            return (100-((avail*100)/total));
        }
};
