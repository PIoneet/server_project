#include <iostream>
#include <string>
using namespace std;

int main() {
    bool change = true;
    string name =  "Hong Jae";
    cout << boolalpha;
    cout << "Hello, Linux Server! My name is ";
    cout << name << endl<<endl;

    for(int i=0;i<6;i++){
        cout<<i<<": number >> ";
        cout << change << endl<<endl;
        if(change == true)
            change = false;
        else
            change = true;

    }

    int num;
    cin >> num;

    return 0;
}
