//Search and Replace Porject
//Brandt Konik
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
using namespace std;

//Make sure both file opens correctly
//If one doesn't open then program will stop
void file_open(string *f_n, ifstream *i, fstream *f, bool *f_c) {
    cout << "Enter your file name: ";
    cin >> *f_n;
    
    i->open((*f_n).c_str());
    f->open((*f_n).c_str());
    //checks both files because I open the file two different ways one for reading and the other for reading and writing
    if (!i->is_open() || !f->is_open()) {
        cout << "\nNo matching file found. Bye.\n";
        *f_c = false;
    }
    i->close(); //close file so it is only opened when needed in the program
    f->close(); //close file because it does not need to be opened unless case 2 is called
}

//Diplays the menu
void print_menu(int *c) {
    cout << string(26, '-') << " Menu " << string(26, '-') << endl;
    cout << left << setw(15) << "1. Search " << left << setw(15) << "2. Replace" << endl;
    cout << left << setw(15) << "0. Exit " << endl;
    cout << string(61, '-') << endl;
    cout << "Enter your choie = ";
    cin >> *c;
}

//This function reads the file and keeps track of how many times the item appears and on what line
//Then it return whether it found the item or not and prints the output
void find_item_print(ifstream *i, string *ite, int *l_i, int *c, size_t *ind, string *l, string *f_n, set <int> m_s) {
    i->open((*f_n).c_str());
    cout << "\nEnter you seach item = ";
    cin >> *ite;
    *l_i = 0;
    *c = 0;
    *ind = 0;
    
    while(getline(*i, *l)) {
        *ind = (*l).find(*ite);
        ++(*l_i); //tracks what line we find the item at
        //This is a check to make sure the find functions finds the the item being searched
        while (*ind != string::npos) {
            m_s.insert(*l_i); //stores each line we find the item on without duplicates
            ++(*c);
            *ind = (*l).find(*ite, *ind + 1);
        }
    }
    //This check determines if the item was ever found and then bases the output off of the check
    if (*c == 0) {
        cout << "No matching found for " << *ite << endl;
    } else {
        cout << *ite << " appeared " << *c << " times in the following line(s)" << endl;
        //This allows me to print all the varibles in my set using a iterator
        for(set <int>::iterator it = m_s.begin(); it != m_s.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }
    i->close();
}

//Thie function takes input and reads the file to see if the word is there and then replaces it with the input if it is
//Then it writes the new replaced words to the file
void replace_word_print(ifstream *i, fstream *f, string *f_n, string *t, string *n_t, int *c, bool *ch, string *l, string *w) {
    i->open((*f_n).c_str());
    cout << "\nEnter you search text = ";
    cin >> *t;
    cout << "\nEnter new text = ";
    cin >> *n_t;
    *c = 0;
    *ch = false;
    string new_line;
    ostringstream oss; //long string to store changed words from the file;
    ostringstream line_oss; //get file line by line for check

    //Gets each line from the file
    while (getline(*i, *l)) {
        size_t pos = (*l).find(*t); //finds the first position of the searched word
                //Runs as long as it can find the position of the searched word
                while (pos != string::npos) {
                    (*ch) = true;
                    ++(*c); //This adds to the count of how many times wer replace the word
                    //This line replaces the old word with the new word
                    (*l).replace(pos, (*t).length(), *n_t);
                    //After repalcing it finds the next index of the word it is trying to replace
                    pos = (*l).find(*t, pos + (*n_t).length());
                }
                oss << *l << endl; //Adds the new line and endl so the file does not become one long string
            }
    i->close(); //Done reading file so I close it here
    
    //Check to see if the word was found in the file or not
    if(*ch) {
        f->open((*f_n).c_str(), ios::out | ios::trunc);
        //This line opens the file to write and clears its contents so then I don't duplicate the file content
        f->seekp(0); //brings pointer back to the start of the file for when I write
        (*f) << (oss).str(); //writes new words with replaced text in there
    }

    cout << (!*ch? "No matching found for " + *t + "\n": "Succesfully replaced \"" + *t + "\" by \"" + *n_t + "\" in " + to_string(*c) + " different places.\n");
    f->close();
}

int main(int argc, const char * argv[]) {
    string file_name, item, text, new_text, line, word;
    size_t index;
    int choice;
    int line_index;
    int count;
    bool check;
    bool file_check = true;
    set <int> my_set;
    fstream fs;
    ifstream ifs;
    
    file_open(&file_name, &ifs, &fs, &file_check);
    if (!file_check) {
        return 0;
    }
    
    print_menu(&choice);
    
    while(true) {
        switch (choice) {
            case 0:
                cout << "\nGood Bye\n";
                //All files are already closed so can end program
                return 0;
            case 1:
                find_item_print(&ifs, &item, &line_index, &count, &index, &line, &file_name, my_set);
                print_menu(&choice);
                break;
            case 2:
                replace_word_print(&ifs, &fs, &file_name, &text, &new_text, &count, &check, &line, &word);
                print_menu(&choice);
                break;
            default:
                cout << "\nWrong input enter enter 0, 1, or 2: \n";
                print_menu(&choice);
        }
    }
    //All files are already closed so can end program
    return 0;
}
