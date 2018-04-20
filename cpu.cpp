#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "split.cpp"

#define STAT_PATH "/proc/stat"
string get_stat_line(int index) {
    ifstream infile(STAT_PATH);
    string line;
    int p = 0;
    while (getline(infile, line)) {
        if (infile.eof()) break;
        //cout << "Line " << p << ": " << line << endl;
        if (p == index) {
            infile.close();
            return line;
            break;
        }
        p++;
    }
    infile.close();
    cerr << "File out of boundaries" << endl;
    exit(1);
}

class Cpu {
    private:
        int cpu_index;
        int total_jiffies_0;
        int work_jiffies_0;
        int total_jiffies_1;
        int work_jiffies_1;

    public:
        Cpu(int cpu_index) {
            this->cpu_index = cpu_index;
            string statrowname = split(get_stat_line(cpu_index+1), ' ')[0];
            if (statrowname.length() < 4 && statrowname.substr(0,3) != "cpu") {
                cerr << "Requested CPU number not available";
                exit(1);
            }
            total_jiffies_0 = 0;
            work_jiffies_0 = 0;
            total_jiffies_1 = 0;
            work_jiffies_1 = 0;
        }

        int get_used() {
            total_jiffies_0 = total_jiffies_1;
            work_jiffies_0 = work_jiffies_1;
            total_jiffies_1=0;
            work_jiffies_1=0;
            vector<string> split_statline = split(get_stat_line(cpu_index+1), ' ');
            for (int i=1; i<split_statline.size(); i++) {
                total_jiffies_1 += stoi(split_statline[i]);
                if (i==1 || i==2 || i==3) {
                    work_jiffies_1 += stoi(split_statline[i]);
                }
            }
            return (int)(((float)(work_jiffies_1 - work_jiffies_0) / (float)(total_jiffies_1 - total_jiffies_0)) * 100);
        }
};
