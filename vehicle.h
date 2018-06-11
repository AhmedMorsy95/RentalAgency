#include <string>

using namespace std;

class vehicle{

private:
  string model;
  string plate;
  int price24;
public:
  vehicle(string a,string b,string c){}
  string get_model();
  string get_plate();
  float get_price(int rentalDays);
  void set_price(float price);
  void print();
};

