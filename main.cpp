#include <iostream>
#include <fstream>


using namespace std;


void ReadTwoSymbols(ifstream &first, ifstream &second, char &symbol_f, char &symbol_s, size_t &position) {
    first >> symbol_f;
    second >> symbol_s;
    position++;
}


void WriteSymbolFromTo(ifstream &from, ofstream &to, ssize_t &position) {
    char symbol;
    from >> symbol;
    to << symbol;
    position++;
}


void Difference(ifstream &old_file, ifstream &new_file) {
    ofstream difference_file("difference.txt");

    char symbol_old;
    char symbol_new;
    size_t position = 0;
    while (!new_file.eof()) {
        ReadTwoSymbols(old_file, new_file, symbol_old, symbol_new, position);

        if (symbol_old != symbol_new) {
            difference_file << position - 1 << " ";

            while (symbol_old != symbol_new && !new_file.eof()) {
                difference_file << symbol_new;
                ReadTwoSymbols(old_file, new_file, symbol_old, symbol_new, position);
            }

            difference_file << '\n';

            if (new_file.eof() && old_file.eof()) {
                difference_file << -1;
            }
        }
    }

    difference_file.close();
}


void Restore(ifstream &old_file, ifstream &difference_file) {
    ofstream new_file("new.bin", std::ios::binary);

    ssize_t position = 0;
    ssize_t last_position = 0;
    while (!difference_file.eof() && position != -1) {
        difference_file >> position;

        for (ssize_t i = last_position; i < position; i++) {
            WriteSymbolFromTo(old_file, new_file, last_position);
        }

        string str;
        difference_file >> str;
        new_file << str;

        for (ssize_t i = last_position; i < last_position + str.length(); i++) {
            char symbol;
            old_file >> symbol;
        }
        last_position += str.length();
    }

    if (position != -1 && last_position != old_file.tellg()) {
        for (ssize_t i = last_position; i < old_file.tellg(); i++) {
            WriteSymbolFromTo(old_file, new_file, last_position);
        }
    }

    new_file.close();
}


int main() {
    size_t command = 1;
    while (command) {
        cout << "What command to execute?\n";
        cin >> command;

        if (command == 2) {
            cout << "Enter the names of the old and new file:\n";
            string name_old, name_new;
            cin >> name_old >> name_new;

            ifstream old_file(name_old, std::ios::binary);
            ifstream new_file(name_new, std::ios::binary);
            Difference(old_file, new_file);

            old_file.close();
            new_file.close();

            cout << "Done\n";
        } else if (command == 1) {
            cout << "Enter the names of the old and difference file:\n";
            string name_old, name_difference;
            cin >> name_old >> name_difference;

            ifstream old_file(name_old, std::ios::binary);
            ifstream difference_file;
            difference_file.open(name_difference);

            Restore(old_file, difference_file);

            old_file.close();
            difference_file.close();

            cout << "Done\n";
        } else {
            cout << "Goodbye!\n";
        }
    }

    return 0;
}