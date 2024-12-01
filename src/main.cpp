#include <iostream>
#include <memory> 
using namespace std;
#include "RandomNameGenerator.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

class Pracownik
{
    protected:
    string imie_;

    public:
    virtual void print() const {
        cout<<imie_<<endl;
    }

    Pracownik(string a): imie_(a){}

    virtual ~Pracownik()=default; 

    virtual double wynagrodzenie() const = 0;
};

class Inz: public Pracownik
{
    string wydzial_;

    public:
    static constexpr double CI=50;//stała wydajność dla inżyniera
    static constexpr double wynagrodzenie_Inz=5500;//wynagrodzenie inżyniera

    Inz(string w): Pracownik(getRandomName()), wydzial_(w){}

    void print() const override{
        cout<<"Inzynier: ";
        Pracownik::print();
        cout<<"Wydzial: "<<wydzial_<<endl;
    }

    ~Inz() override {}

    double wynagrodzenie() const override{
        return wynagrodzenie_Inz;
    }
};

class Mkt: public Pracownik
{
    int follows_;

    public:
    static constexpr double CMkt=45;//stała wydajność dla marketera
    static constexpr double wynagrodzenie_Mkt=5200;//wynagrodzenie marketera

    Mkt(int f): Pracownik(getRandomName()), follows_(f){}

    void print() const override{
        cout<<"Marketer: ";
        Pracownik::print();
        cout<<"Liczba obserwujacych: "<<follows_<<endl;
    }

    ~Mkt() override {}

    double wynagrodzenie() const override{
        return wynagrodzenie_Mkt;
    }
};

class Mag: public Pracownik
{
    bool obsl_widl_;

    public:
    static constexpr double CMag=45;//stała wydajność dla magazyniera
    static constexpr double wynagrodzenie_Mag=5000;//wynagrodzenie magazyniera

    Mag(bool a=false): Pracownik(getRandomName()), obsl_widl_(a){}

    void print() const override{
        string odp;
        if(obsl_widl_)
        {
            odp="Tak";
        } else
        {
            odp="Nie";
        }

        cout<<"Magazynier: ";
        Pracownik::print();
        cout<<"Mozliwosc obslugi wozka widlowego: "<<odp<<endl;
    }

    ~Mag() override {}

    double wynagrodzenie() const override{
        return wynagrodzenie_Mag;
    }
};

class Rob: public Pracownik
{
    double but_;

    public:
    static constexpr double CR=40;//stała wydajność dla robotnika
    static constexpr double wynagrodzenie_Rob=4800;//wynagrodzenie robotnika
    
    Rob(double a=44.5): Pracownik(getRandomName()), but_(a){}

    void print() const override{
        cout<<"Robotnik: ";
        Pracownik::print();
        cout<<"Rozmiar buta: "<<but_<<endl;
    }

    ~Rob() override {}

    double wynagrodzenie() const override{
        return wynagrodzenie_Rob;
    }
};

class Kredyt
{
    public:
    double kwota_;//kwota kredytu
    int czas_splaty_;//czas spłaty w miesiącach
    double oprocentowanie_;//oprocentowanie kredytu
    double kwota_z_odsetkami_;//całkowita kwota do zapłaty (z odsetkami)
    int pozostale_raty_;//liczba pozostałych rat do zapłaty

    Kredyt(double kwota, int czas_splaty): kwota_(kwota), czas_splaty_(czas_splaty), pozostale_raty_(czas_splaty){
        // Odsetki zależą od czasu spłaty (im dłuższy, tym wyższe)
        oprocentowanie_= 0.03*pow(czas_splaty,0.5);
        kwota_z_odsetkami_=kwota_*(1+oprocentowanie_);
    }

    //miesięczna rata do zapłaty
    double miesieczna_rata() const{
        return kwota_z_odsetkami_/czas_splaty_;
    }

    void zaplata_raty()
    {
        if(pozostale_raty_>0)
        {
            pozostale_raty_--;
        }
    }

    int pozostale_raty() const
    {
        return pozostale_raty_;
    }

    void print() const{
        cout<<"Kwota kredytu: "<<kwota_<<" zl.\n";
        cout<<"Czas splaty: "<<czas_splaty_<<" miesiecy.\n";
        cout<<"Oprocentowanie: "<<oprocentowanie_*100<<"%.\n";
        cout<<"Do splaty: "<<kwota_z_odsetkami_<<" zl.\n";
    }


};

class Firma
{
    protected:
    double stan_konta_;//stan konta firmy
    vector<unique_ptr<Pracownik>> pracownicy;//wektor pracowników
    int liczba_pracownikow_;//aktualna liczba pracowników
    int liczba_magazynierow_;
    int liczba_inzynierow_;
    int liczba_marketerow_;
    int liczba_robotnikow_;
    static const int limit_pracownikow_=40;//limit liczby pracowników
    static const int N=12;//Liczba miesięcy do obliczania średniego przychodu
    vector<unique_ptr<double>> przychody_historia;
    int obecny_miesiac_=0;//indeks na bieżący miesiąc
    double magazyn_ = 0; // Liczba produktów w magazynie
    vector<unique_ptr<Kredyt>> kredyty;  // lista zaciągniętych kredytów
    static const int limit_kredytow_=1;//limit liczby kredytów
    static constexpr double M=5.0; // mnożnik dla zadłużenia
    static const int max_czas_splaty_=24; //maksymalny czas spłaty kredytu (w miesiacach)

    public:
    Firma(double stan=10000.0): stan_konta_(stan), liczba_pracownikow_(4), liczba_magazynierow_(1),
    liczba_inzynierow_(1),liczba_marketerow_(1),liczba_robotnikow_(1){
        pracownicy.push_back(make_unique<Inz>("Mechaniczny Energetyki i Lotnictwa"));
        pracownicy.push_back(make_unique<Mkt>(100));
        pracownicy.push_back(make_unique<Mag>(true));
        pracownicy.push_back(make_unique<Rob>(44.5));

        for(int i=0;i<N;i++)
        {
            przychody_historia.push_back(make_unique<double>(0.0));// Wartość początkowa przychodu = 0
        }
        // Przypisanie po jednym pracowniku każdego rodzaju na start
    }//stan domyślny, na start

    void drukuj_pracownikow() const {
        cout << "Lista wszystkich pracownikow:\n";
        for(const auto& osoba : pracownicy)
        {
            osoba->print();
            cout<<endl;
        }   
    }

    void zatrudnij(const unique_ptr<Pracownik> pracownik)
    {
        if(liczba_pracownikow_>=limit_pracownikow_)
        {
            cout<<"Limit liczby pracownikow osiagniety, nie mozna zatrudnic wiecej osob\n";
            return;
        }

        if (auto inzynier=dynamic_cast<Inz*>(pracownik.get())) 
        {
            pracownicy.push_back( make_unique<Inz>(*inzynier));
            liczba_inzynierow_++;
        }
        else if(auto magazynier=dynamic_cast<Mag*>(pracownik.get()))
            {
            pracownicy.push_back(make_unique<Mag>(*magazynier));
            liczba_magazynierow_++;
            }
            else if(auto marketer=dynamic_cast<Mkt*>(pracownik.get()))
                {
                    pracownicy.push_back(make_unique<Mkt>(*marketer));
                    liczba_marketerow_++;
                }
                else
                {   
                    auto robotnik=dynamic_cast<Rob*>(pracownik.get());
                    pracownicy.push_back(make_unique<Rob>(*robotnik));
                    liczba_robotnikow_++;
                }  
        liczba_pracownikow_++;
    }

    double wynagrodzenie() const
    {
        double suma_wynagrodzen=0.0;
        for(const auto& osoba : pracownicy)
        {
            suma_wynagrodzen+=osoba->wynagrodzenie();
        }   
        return suma_wynagrodzen;
    }

    void zaplac_wynagrodzenie() const
    {
        cout<<"Laczna wyplata wynagrodzen wynosi: "<<wynagrodzenie()<<endl;
    }

    double pojemnosc_magazynu() const
    {
        double pojemnosc_magazynu=liczba_magazynierow_ * Mag::CMag;
        return pojemnosc_magazynu;
    }

    double cena_produktu() const
    {   
        double cena=liczba_inzynierow_*Inz::CI;
        return cena;
    }

    double popyt() const
    {
        double pop=liczba_marketerow_*Mkt::CMkt;
        return pop;
    }

    double get_stan_konta_() const {
        return stan_konta_;
    }

    double oblicz_przychody(){
        double teor_produkty, fakt_produkty, liczba_sprzedanych, przychod;
        teor_produkty=liczba_robotnikow_*Rob::CR;
        if(teor_produkty<pojemnosc_magazynu())
        {
            fakt_produkty=teor_produkty;
        }
        else
        {
            fakt_produkty=pojemnosc_magazynu();
        }

        magazyn_+=fakt_produkty;

        if(popyt()<fakt_produkty)
        {
            liczba_sprzedanych=popyt();
        }
        else
        {
            liczba_sprzedanych=fakt_produkty;
        }

        magazyn_-=liczba_sprzedanych;

        przychod=liczba_sprzedanych*cena_produktu();

        *przychody_historia[obecny_miesiac_]=przychod;

        obecny_miesiac_+=1;

        if(obecny_miesiac_==N+1) obecny_miesiac_=1;

        return przychod;
    }

    double wartosc_firmy() const
    {
        double wartosc_spolki=0.0;
        for(const auto& przychod:przychody_historia )
        {
            wartosc_spolki+=*przychod;
        }

        return wartosc_spolki/N;
    }

    bool zaciagnij_kredyt(double kwota, int czas_splaty)
    {
        if(czas_splaty>max_czas_splaty_)
        {
            cout<<"Nie mozna zaciagnac kredytu na dluzej niz "<<max_czas_splaty_<<"miesiec.\n";
            return false;
        }

        double zadluzenie=0;
        for(const auto& kredyt :kredyty)
        {
            zadluzenie+=kredyt->kwota_z_odsetkami_;
        }

        if(zadluzenie+kwota>M*wartosc_firmy())
        {
            cout<<"Nie mozna przekroczyc limitu zadluzenia.\n";
            return false;
        }
        
        if(kredyty.size()>limit_kredytow_)
        {
            cout<<"Limit kredytow zostal osiagniety.\n";
            return false;
        }

        kredyty.push_back(make_unique<Kredyt>(kwota, czas_splaty));
        stan_konta_+=kwota;
        cout<<"Zaciagnieto kredyt o wysokosc: "<<kwota<<" na "<<czas_splaty<<" miesiecy.\n";
        return true;
    }

    void print_kredyty() const
    {
        if(kredyty.empty())
        {
            cout<<"Nie zaciagnieto zadnych kredytow.\n";
            return;
        }

        for(const auto& kredyt:kredyty)
        {
            kredyt->print();
            cout<<"\n";
        }
    }
    
    void zaplac_kredyt()
    {
        bool kredyt_splacony=false;

        for(const auto& kredyt :kredyty)
        {
            double rata=kredyt->miesieczna_rata();
            if(stan_konta_>=rata && kredyt->pozostale_raty()>0)
            {
                stan_konta_-=rata;
                kredyt->zaplata_raty();
                cout<<"Zaplacono rate kredytu: "<< rata<<" zl.\n";
            } else if(kredyt->pozostale_raty()==0 && !kredyt_splacony){
                cout<<"Kredyt zostal splacony.\n";
                kredyt_splacony=true;
            } else{
                cout<<"Brak wystarczajacych srodkow na koncie, aby zaplacic rate.\n";
            }
        }
    }

    void wyswietl_limit_zadluzenia() const
    {
        double zadluzenie=0.0; 
        for (const auto& kredyt : kredyty)
        {
            zadluzenie += kredyt->kwota_z_odsetkami_;
        }

        double limit_zadluzenia = M * wartosc_firmy();

        cout << "Aktualne zadluzenie firmy wynosi: " << zadluzenie << " zl.\n";
        cout << "Limit zadluzenia to: " << limit_zadluzenia << " zl.\n";
        
        if (zadluzenie > limit_zadluzenia)
        {
            cout << "Ostrzezenie: Przekroczono limit zadluzenia!\n";
        }

    }

    void oblicz_dochod()
    {
        double dochod;
        double rata_kredytow = 0.0;
        for (const auto& kredyt : kredyty) {
            rata_kredytow += kredyt->miesieczna_rata();
            kredyt->zaplata_raty(); // zmniejszenie liczby pozostałych rat
        }

        dochod=oblicz_przychody()-wynagrodzenie()-rata_kredytow;

        stan_konta_+=dochod;

        cout<<"Dochod za obecny miesiac wynosi: "<<dochod<<" zl.\n";
        cout<<"Nowy stan konta wynosi: "<<stan_konta_<<" zl.\n";
        return;
    }

    void stan_magazynu() const {
        cout << "Aktualny stan magazynu: " << magazyn_ << " produktow.\n";
    }

};

class Gra
{
    private:
    Firma firma;
    double wartosc_docelowa_;
    bool trwanie_gry_;

    void menu()
    {
        cout<<"MENU GRY\n"
        <<"Dostepne komendy:\n"
        <<"lp - wylistuj pracownikow (imiona i wynagrodzenia)\n"
        <<"zinz - zatrudnij inzyniera\n"
        <<"zmag - zatrudnij magazyniera\n"
        <<"zmkt - zatrudnij marketera\n"
        <<"zrob - zatrudnij robotnika\n"
        <<"kred - wez kredyt (w nastepnym kroku podaj kwote i czas splaty)\n"
        <<"lz - wyswietl aktualne zadluzenie i limit\n"
        <<"kt - zakoncz ture i wyswietl stan firmy na poczatku nastepnego miesiaca\n"
        <<"exit - zakonczenie gry\n"
        <<"\n";
    }

    void zakoncz_ture()
    {
        cout<<"Koniec tury...\n";
        firma.oblicz_dochod();

        double wartosc_spolki=firma.wartosc_firmy();
        cout<<"Aktualna wartosc firmy wynosi: "<<wartosc_spolki<<" zl.\n";
        firma.stan_magazynu();

        if(firma.get_stan_konta_()<0)
        {
            cout<<"Twoja firma zbankrutowala! Gra zakonczona.\n";
            trwanie_gry_=false;
        } else if(wartosc_spolki>=wartosc_docelowa_)
        {
            cout<<"Gratulacje! Osiagnales wartosc docelowa firmy i wygrales gre!\n";
            trwanie_gry_=false;
        }
    }

    void akcja(const string& akcja)
    {
        if (akcja == "lp") 
        {
            firma.drukuj_pracownikow();
        } else if (akcja == "zinz") 
        {
            string wydzial;
            cout<<"Podaj nazwe wydzialu, ktory ukonczyles: ";
            cin>>wydzial;
            firma.zatrudnij(make_unique<Inz>(wydzial));
        } else if (akcja == "zmag") 
        {
            bool obsluga;
            string odp;
            int dalej=0;
            while(dalej==0)
            {
            cout<<"Powiedz czy mozesz obslugiwac wozek widlowy (tak lub nie): ";
            cin>>odp;
            if(odp=="tak")
                {
                    obsluga=true;
                    dalej=1;
                } else if (odp=="nie")
                {
                    obsluga=false;
                    dalej=1;
                } else
                {
                    cout<<"Odpowiedziales niezgodnie z wymaganiem.\n";
                }
            }
            firma.zatrudnij(make_unique<Mag>(obsluga));
        } else if (akcja == "zmkt") 
        {
            int liczba;
            cout<<"Podaj liczbe ile masz followerosw: ";
            cin>>liczba;
            firma.zatrudnij(make_unique<Mkt>(liczba));
        } else if (akcja == "zrob") 
        {
            double nr_buta;
            cout<<"Podaj swoj numer buta: ";
            cin>>nr_buta;
            firma.zatrudnij(make_unique<Rob>(nr_buta));
        } else if (akcja == "kred") 
        {
            double kwota;
            int czas_splaty;
            cout<<"Podaj kwote kredytu: ";
            cin>>kwota;
            cout<<"Podaj czas splaty (w miesiacach): ";
            cin>>czas_splaty;
            firma.zaciagnij_kredyt(kwota,czas_splaty);
        } else if(akcja=="lz")
        {
            firma.wyswietl_limit_zadluzenia();
        } else if (akcja == "kt") 
        {
            zakoncz_ture();
        } else if(akcja=="exit")
        {
            trwanie_gry_=false;
        }
         else 
        {
            cout << "Nieznana komenda. Wybierz jedna z dostepnych opcji.\n";
        }
    }

    public:

    Gra(): firma(100000.0), wartosc_docelowa_(100000.0), trwanie_gry_(true){}

    void start()
    {
        cout<<"Witaj w grze ekonowicznej!\n";
        cout<<"Twoim celem jest osiagniecie wartosci firmy na poziomie "<< wartosc_docelowa_ << " zl.\n";
        cout<<"Rozpoczynasz z poczatkowym stanem konta 100 000 zl.\n";

        menu();
        while(trwanie_gry_)
        {
            string wybor;
            cout<<"Wykonaj akcje: ";
            cin>>wybor;
            akcja(wybor);
            cout<<endl;
        }
        cout<<"Zakonczyles gre.\n";
    }
};


int main()
{
    clearScreen();
    Gra gra;
    gra.start();

    return 0;
}
