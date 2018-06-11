#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

///
class vehicle{

private:
  string model;
  string plate;
  float price24;
public:
  vehicle(string a,string b,float c){
     model=a;
     plate=b;
     price24=c;
  }
  string get_model();
  string get_plate();
  float get_price(int rentalDays);
  void set_price(float price);
  virtual void print(){
    cout<<model<<' '<<plate<<' '<<price24<<endl;
  }
  /// there is a virtual function to be added
};
string vehicle::get_model(){
  return model;
}
string vehicle::get_plate(){
  return model;
}
float vehicle::get_price(int rental_days){
  return price24*float(rental_days);
}
void vehicle::set_price(float price){
  price24=price;
}
///
class Scooter :public vehicle{
   public:
       Scooter(string a,string b):vehicle(a,b,9.99){;}
       virtual void print(){
         cout<<"\"Scooter "<<get_model()<<"\""<<' '<<"\"plate: "<<get_plate()<<" \""<<endl;
       }
};
///
class Car :public vehicle{/// astract class
   private:
       bool navigation;
   public:
       Car(string a,string b,float c=29.99,bool d=true):vehicle(a,b,c){
          navigation=d;
       }
       virtual bool has_aircondition()=0;
       bool has_navigation(){};
       virtual void print(){
         cout<<"\"plate: "<<get_plate()<<' ';
         if(has_aircondition())cout<<", air condition ";
         if(has_navigation())cout<<" , navigation ";
         cout<<endl;
       }
};
///
class Cabrio : public Car{
  public:
      Cabrio(string a,string b,float c=99.99):Car(a,b,c){}
      virtual bool has_aircondition(){
        return false;
      }
      virtual void print(){
        cout<<"Cabrio\n";
        Car::print();
      }
};
///
class Limousine : public Car{
   private:
      bool aircondition;
   public:
       Limousine(string a,string b,float c=149.9,bool d=true):Car(a,b,c){
            aircondition=d;
       }
       virtual bool has_aircondition(){
        return aircondition;
       }
       virtual void print(){
        cout<<"Limousine\n";
        Car::print();
      }
};
///
class Timepoint{
  private:
      unsigned short int dayofyear,year,time;
      unsigned short int days[13]={0,31,59,90,120,151,181,212,243,273,304,334,365};
  public:
      Timepoint(int day,int month,int Year,int hour,int minutes){
          dayofyear=days[month-1]+day;
          year=Year;
          time=hour*60+minutes;
      }
      int getdayofyear(){
        return dayofyear;
      }
      Timepoint operator ++(){
          dayofyear++;
          if(dayofyear==366){
            year++;
            dayofyear=1;
          }
      }
      friend ostream & operator << (ostream &out,Timepoint &obj){
            int a=obj.dayofyear,month=0;
            for(int i=0;i<13;i++){
                int prev=0;
                if(i)prev=obj.days[i]-obj.days[i-1];
                if(a-prev<=0)break;
                month++;
                a-=prev;
            }
            cout<<a<<"-"<<month<<"-"<<obj.year<<" "<<obj.time/60<<":"<<obj.time%60<<endl;
      }
      friend istream & operator >> (istream &in,  Timepoint &obj){
           int a,b,c,d,e;
           char z;
           cout<<"enter date ";
           in >> a >>z >> b>>z >>c >> d >>z>> e;
           obj.year=c;
           obj.time=d*60+e;
           obj.dayofyear=obj.days[b-1]+a;
      }
};
///
class License{
   private:
       int no;
       string issuer;
       Timepoint *validUntil;
   public:
       License(int a,string b,Timepoint &c){
          no=a;
          issuer=b;
          validUntil=&c;
       }
       int getno(){return no;}
       string getissuer(){return issuer;}
       Timepoint getvalidUntil(){return *validUntil;}
       friend ostream & operator << (ostream &out,License &obj){
            cout<<"no : "<<obj.getno()<<' '<<"issues : "<<obj.getissuer()<<" valid until ,"<<obj.validUntil<<endl;
      }
};
///
class Driver{
  private:
      string name;
      License *license;
  public:
      Driver(string a,License &b){
           name=a;
           license=&b;
      }
      string get_name(){
        return name;
      }
      License get_license(){
        return *license;
      }
      friend ostream & operator << (ostream &out,Driver &obj){
            cout<<"name : "<<obj.get_name()<<' ';
            License a=obj.get_license();
            cout<<a<<endl;
      }
};
///
class Rental{
   private:
       int no=1;
       Timepoint *from;
       int days;
       vehicle *Vehicle;
       Driver *driver;
       Driver *additionalDriver=NULL;
       int last_no=1;
   public:
       Rental(vehicle &a,Timepoint &b,Driver &d,Driver &e,int c=1){
          Vehicle=&a;
          from=&b;
          driver=&d;
          additionalDriver=&e;
          days=c;
       }
       Timepoint getfrom(){
        return *from;
       }
       Timepoint getUntil(){
         Timepoint c=*from;
         for(int i=0;i<days;i++)++c;
         return c;
       }
      void print(){
         vehicle tmp=*Vehicle;
         cout<<"Vehicle:  ";
         tmp.print();
         cout<<"VEHICLE RENTAL"<<endl;
         cout<<"rental no: "<<no<<endl;
         cout<<"from: "<<*from;
         Timepoint until=getUntil();
         cout<<"until: "<<until;
         cout<<"driver: "<<driver->get_name()<<endl;
         if(additionalDriver!=NULL){
            cout<<"additional driver: "<<additionalDriver->get_name()<<endl;
         }
         cout<<"Rental price : "<<Vehicle->get_price(days)<<endl;
      }
};
///
class Schedule{
  private:
       vehicle *Vehicle;
       Rental *year[365];
  public:
       Schedule(vehicle &a){
        Vehicle=&a;
        for(int i=0;i<365;i++)
            year[i]=NULL;
       }
       vehicle *get_vehicle(){
         return Vehicle;
       }
       bool isFree(Timepoint a,int days){
              int now=a.getdayofyear();
              for(int i=now;i<now+days;i++){
                if(year[i]!=NULL)return 0;
              }
              return 1;
       }
       void book(Timepoint &a,int days,Driver *b,Driver *c){
             int now=a.getdayofyear();
             Rental *obj=new Rental(*Vehicle,a,*b,*c,days);
             for(int i=now;i<now+days;i++){
                  year[i%365]=obj;
             }
       }
       void print(){
          cout<<"SCHEDULE\n";
          for(int i=0;i<365;i++){
             if(year[i]==NULL)continue;
             bool same=0;
             for(int j=0;j<i;j++){
                if(year[i]==year[j])same=1;
             }
             if(!same){
                year[i]->print();
             }
          }
       }

};
///
int main()
{
    /*Scooter a("ahjed","aasda");
    a.print();

    Cabrio a("ahmed","mohamed");
    a.print();
    */
    //freopen("test.in","r",stdin);
    Schedule *arr[4];
    Scooter *a=new Scooter("BMW C 650 GT","DU-BC 65");
    Cabrio *b=new Cabrio("BMW i8 Roadstar","DU-BR 8");
    Limousine *c=new Limousine("Mercedes C 350 e","DU-MC 1",129);
    Limousine *d=new Limousine("Mercedes E 350 e","DU-ME 2",189,0);
    arr[0]=new Schedule(*a);
    arr[1]=new Schedule(*b);
    arr[2]=new Schedule(*c);
    arr[3]=new Schedule(*d);
    cout<<"Vehicle Rental Agency\n";
    while(1){
        cout<<"0 end\n"<<"1 new rental\n"<<"2 print rentals\n";
        int in;
        cin>>in;
        if(in==0)break;
        cout<<"choose vehicle\n";
        cout<<"1 Scooter  BMW C 650 GT plate(DU-BC 65)\n";
        cout<<"2 Cabrio   BMW i8 Roadstar plate(DU-BR 8)\n";
        cout<<"3 Limousine Mercedes C 350 e plate(DU-MC 1)\n";
        cout<<"4 Limousine Mercedes E 350 e plate(DU-ME 2)\n";
        int chosen;
        cin>>chosen;
        chosen--;
        if(in==1){
            cout<<"enter day of rental in format day.month.year hr:min -> ";
            int a,b,c,d,e,days,no;
            char z;
            string dr,iss,dr2="";
            cin>>a >>z >> b>>z >>c >> d >>z>> e;
            Timepoint *t=new Timepoint(a,b,c,d,e);
            cout<<"number of days : ";
            cin>>days;
            cout<<"input data vehicle renter and driver : \n";
            cout<<"name of the driver : ";
            cin>>dr;
            cout<<"issuer of the license : ";
            cin>>iss;
            cout<<"number of license : ";
            cin>>no;
            cout<<"valid until in format day.month.year hr:min ->";
            cin>>a >>z >> b>>z >>c >> d >>z>> e;
            Timepoint *tvalid=new Timepoint(a,b,c,d,e);
            cout<<"additional driver (y/n)? ";
            string answer;
            cin>>answer;
            License *L=new License(no,iss,*tvalid);
            cout<<endl;
            Driver *D=new Driver(dr,*L);
            Driver *D2=NULL;
            if(answer[0]=='y'){
                        cout<<"name of the driver : ";
                        cin>>dr2;
                        cout<<"issuer of the license : ";
                        cin>>iss;
                        cout<<"number of license : ";
                        cin>>no;
                        cout<<"valid until format day.month.year hr:min ->";
                        cin>>a >>z >> b>>z >>c >> d >>z>> e;
                        Timepoint *tvalid2=new Timepoint(a,b,c,d,e);
                        License *L2=new License(no,iss,*tvalid2);
                        D2=new Driver(dr2,*L2);
            }
            Rental *R=new Rental(*(arr[chosen]->get_vehicle()),*t,*D,*D2,days);
            if(arr[chosen]->isFree(*t,days)){
                 cout<<"booked\n";
                 arr[chosen]->book(*t,days,D,D2);
            }
            else{
                cout<<"sorry but we are busy\n";
            }
        }
        else{
            arr[chosen]->print();
            break;
            cout<<endl;
        }

    }
    return 0;
}
