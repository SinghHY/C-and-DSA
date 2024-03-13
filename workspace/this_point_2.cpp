/*Example 2: function chaining calls using this pointer
*https://beginnersbook.com/2017/08/cpp-this-pointer*/
#include <iostream>
using namespace std;

class Demo {
  private:
    int num;
    char ch;
  public:
    Demo &setNum(int num)
    {
      this->num =num;
      return *this;
    }
    Demo &setCh(char ch){
      this->num++;
      this->ch =ch;
      return *this;
    }
  void displayMyValues(){
    cout<<num<<endl;
    cout<<ch;
  }
};


int main(){
  Demo obj;
  //Chaining calls
  obj.setNum(100).setCh('A');
  obj.displayMyValues();
  return 0;
}