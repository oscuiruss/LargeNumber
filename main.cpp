#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <cstdio>
#include "LN.h"


int main(int argc, char **argv) {
    if (argc != 3) {
        printf("%s", "Usage: name of programme, input file, output file,");
        return 1;
    }
    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        printf("%s", "InputFile open error");
        in.close();
        return 1;
    }
    std::ofstream out(argv[2]);
    if (!out.is_open()) {
        printf("%s", "OutputFile open error");
        in.close();
        return 1;
    }
    std::vector<std::string> bin = {"==", "-", "*", "+", "/", ">=", "<=", "!=", ">", "<", "%"};
    std::vector<std::string> un = {"~", "_"};
    std::stack<LN> st;
    try {
        while (!in.eof()) {
            std::string s;
            in >> s;
            if (find(un.begin(), un.end(), s) != un.end()) {
                LN first = st.top();
                st.pop();
                if (s == "~") {
                    st.push(~first);
                } else if (s == "_") {
                    st.push(-first);
                }
            } else if (find(bin.begin(), bin.end(), s) != bin.end()) {
                LN second = st.top();
                st.pop();
                LN first = st.top();
                st.pop();

                if (s == "<=") {
                    st.push(LN(first <= second));
                } else if (s == ">=") {
                    st.push(LN(first >= second));
                } else if (s == "==") {
                    st.push(LN(first == second));
                } else if (s == "!=") {
                    st.push(LN(first != second));
                } else if (s == "<") {
                    st.push(LN(first < second));
                } else if (s == ">") {
                    st.push(LN(first > second));
                } else if (s == "+") {
                    st.push(LN(first + second));
                } else if (s == "-") {
                    st.push(LN(first - second));
                } else if (s == "*") {
                    st.push(LN(first * second));
                } else if (s == "/") {
                    st.push(LN(first / second));
                } else if (s == "%") {
                    st.push(LN(first % second));
                }
            } else {
                LN p = LN(s);
                st.push(p);
            }
        }
    } catch (char *s) {
        std:: cout << s << std::endl;
        out.close();
        return 2;
    }
    while (!st.empty()) {
        out << st.top().string_() << std::endl;
        st.pop();
    }
    out.close();
    in.close();
    return 0;
}