#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// 1. Nuskaito prekes is failo ir grazina kiek prekiu nuskaite (0, jei nepavyko)
int nuskaitytiFaila(string failoVardas,
                    int maxPrekiu,
                    string kategorijos[],
                    string pavadinimai[],
                    double kainos[],
                    int kiekiai[])
{
    ifstream failas(failoVardas); // atidarome duomenu faila skaitymui

    if (!failas.is_open())

    {
        cout << "Klaida: nepavyko atidaryti failo " << failoVardas << endl;
        return 0;
    }

    int kiekIsViso;
    if (!(failas >> kiekIsViso))
    {
        cout << "Klaida: neteisingas prekiu skaicius pirmoje eiluteje." << endl;
        failas.close();
        return 0;
    }

    if (kiekIsViso < 1 || kiekIsViso > maxPrekiu)
    {
        cout << "Klaida: prekiu skaicius turi buti nuo 1 iki " << maxPrekiu << endl;
        failas.close();
        return 0;
    }

    string likusiEilute;
    getline(failas, likusiEilute); // praleidziame eilutes gala po pirmojo skaiciaus

    for (int i = 0; i < kiekIsViso; i++)
    {
        string kategorija;
        string pavadinimas;
        double kaina;
        int kiekis;
        char kabliataskis;

        // Skaitome kategorija ir pavadinima iki kabliataskio ';'
        if (!getline(failas, kategorija, ';') || !getline(failas, pavadinimas, ';'))
        {
            cout << "Klaida: nepavyko nuskaityti teksto " << i + 1 << " eiluteje." << endl;
            failas.close();
            return 0;
        }

        // Skaitome kaina, kabliataski ir kieki
        if (!(failas >> kaina >> kabliataskis >> kiekis))
        {
            cout << "Klaida: neteisinga kaina arba kiekis " << i + 1 << " eiluteje." << endl;
            failas.close();
            return 0;
        }

        getline(failas, likusiEilute); // pereiname i kita eilute

        if (kategorija != "Dviraciai" && kategorija != "Dalys" && kategorija != "Aksesuarai")
        {
            cout << "Klaida: neteisinga kategorija " << i + 1 << " eiluteje." << endl;
            failas.close();
            return 0;
        }

        if (pavadinimas == "" || kaina <= 0 || kiekis <= 0)
        {
            cout << "Klaida: neteisingi duomenys " << i + 1 << " eiluteje." << endl;
            failas.close();
            return 0;
        }

        kategorijos[i] = kategorija;
        pavadinimai[i] = pavadinimas;
        kainos[i] = kaina;
        kiekiai[i] = kiekis;
    }

    failas.close();
    return kiekIsViso; // graziname kiek prekiu sekmingai nuskaiteme
}

// 2. Suskaiciuoja vienos prekes eilutes suma pries nuolaida
    double skaiciuotiEilutesSuma(double kaina, int kiekis)
{
    return kaina * kiekis;
}

// 3. Suskaiciuoja nuolaida (10 proc., jei kiekis >= 10)
    double skaiciuotiNuolaida(double sumaPriesNuolaida, int kiekis)
    {
    if (kiekis >= 10) {
        return sumaPriesNuolaida * 0.10;
    }
    return 0.0;
}

// 4. Suskaiciuoja pasirinktos kategorijos bendra suma po nuolaidu
    double skaiciuotiKategorijosSuma(string pasirinktaKategorija, const string kategorijos[], const double kainos[], const int kiekiai[], int prekiuSkaicius)
{
    double suma = 0.0;
    for (int i = 0; i < prekiuSkaicius; i++)
    {
        if (kategorijos[i] == pasirinktaKategorija)
        {
            double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
            double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
            suma += (sumaPries - nuolaida);
        }
    }
    return suma;
}

// Pagalbine funkcija: leidzia pasirinkti kategorija meniu
    string pasirinktiKategorija()
{
    int pasirinkimas;
    do {
        cout << "Pasirinkite kategorija (1 - Dviraciai, 2 - Dalys, 3 - Aksesuarai): ";
        cin >> pasirinkimas;
        if (pasirinkimas < 1 || pasirinkimas > 3)
        {
            cout << "Neteisingas pasirinkimas, bandykite dar karta." << endl;
        }
    }
    while (pasirinkimas < 1 || pasirinkimas > 3);

    if (pasirinkimas == 1) return "Dviraciai";
    if (pasirinkimas == 2) return "Dalys";
    return "Aksesuarai";
}

// 5. Isveda visa ataskaita i ekrana
    void isvestiAtaskaita(int kurIsvesti, string rezultatuFailas, const string kategorijos[], const string pavadinimai[], const double kainos[], const int kiekiai[], int prekiuSkaicius)
{
    double visoPriesNuolaidu = 0.0;
    double visoNuolaidu = 0.0;
    double visoMoketina = 0.0;

    for (int i = 0; i < prekiuSkaicius; i++)
    {
        double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
        double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
        visoPriesNuolaidu += sumaPries;
        visoNuolaidu += nuolaida;
        visoMoketina += (sumaPries - nuolaida);
    }

    double sumaDviraciai = skaiciuotiKategorijosSuma("Dviraciai", kategorijos, kainos, kiekiai, prekiuSkaicius);
    double sumaDalys = skaiciuotiKategorijosSuma("Dalys", kategorijos, kainos, kiekiai, prekiuSkaicius);
    double sumaAksesuarai = skaiciuotiKategorijosSuma("Aksesuarai", kategorijos, kainos, kiekiai, prekiuSkaicius);

    if (kurIsvesti == 0) {
        cout << fixed << setprecision(2);
        cout << endl << "Uzsakymo ataskaita" << endl;
        for (int i = 0; i < prekiuSkaicius; i++)
        {
            double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
            double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
            cout << i + 1 << ". " << kategorijos[i] << " | " << pavadinimai[i]
                 << " | Kaina: " << kainos[i] << " Eur | Kiekis: " << kiekiai[i]
                 << " | Suma: " << sumaPries << " Eur | Nuolaida: " << nuolaida
                 << " Eur | Moketi: " << (sumaPries - nuolaida) << " Eur" << endl;
        }
        cout << endl << "Suma pries nuolaidas: " << visoPriesNuolaidu << " Eur" << endl;
        cout << "Bendra nuolaida: " << visoNuolaidu << " Eur" << endl;
        cout << "Galutine moketina suma: " << visoMoketina << " Eur" << endl;

        cout << endl << "Sumos pagal kategorijas:" << endl;
        cout << "Dviraciai: " << sumaDviraciai << " Eur" << endl;
        cout << "Dalys: " << sumaDalys << " Eur" << endl;
        cout << "Aksesuarai: " << sumaAksesuarai << " Eur" << endl;
    }
    else
    {
        ofstream isvestiesFailas(rezultatuFailas); // atidarome faila ataskaitos irasymui
        if (!isvestiesFailas.is_open())
        {
            cout << "Klaida: nepavyko sukurti failo " << rezultatuFailas << endl;
            return;
        }

        isvestiesFailas << fixed << setprecision(2);
        isvestiesFailas << "Uzsakymo ataskaita" << endl;
        for (int i = 0; i < prekiuSkaicius; i++)
        {
            double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
            double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
            isvestiesFailas << i + 1 << ". " << kategorijos[i] << " | " << pavadinimai[i]
                            << " | Kaina: " << kainos[i] << " Eur | Kiekis: " << kiekiai[i]
                            << " | Suma: " << sumaPries << " Eur | Nuolaida: " << nuolaida
                            << " Eur | Moketi: " << (sumaPries - nuolaida) << " Eur" << endl;
        }
        isvestiesFailas << endl << "Suma pries nuolaidas: " << visoPriesNuolaidu << " Eur" << endl;
        isvestiesFailas << "Bendra nuolaida: " << visoNuolaidu << " Eur" << endl;
        isvestiesFailas << "Galutine moketina suma: " << visoMoketina << " Eur" << endl;

        isvestiesFailas << endl << "Sumos pagal kategorijas:" << endl;
        isvestiesFailas << "Dviraciai: " << sumaDviraciai << " Eur" << endl;
        isvestiesFailas << "Dalys: " << sumaDalys << " Eur" << endl;
        isvestiesFailas << "Aksesuarai: " << sumaAksesuarai << " Eur" << endl;

        if (!isvestiesFailas.good())
        {
            cout << "Klaida irasant duomenis i faila." << endl;
            isvestiesFailas.close();
            return;
        }

        isvestiesFailas.close();
        cout << "Ataskaita issaugota faile " << rezultatuFailas << endl;
    }
}

int main()
{
    const int MAX_PREKIU = 100;
    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];

    // Nuskaitome prekes is failo ir issaugome kiek ju nuskaityta
    int prekiuSkaicius = nuskaitytiFaila("uzsakymas.txt", MAX_PREKIU, kategorijos, pavadinimai, kainos, kiekiai);

    if (prekiuSkaicius == 0) {
        cout << "Duomenu nuskaitymo klaida." << endl;
        return 1;
    }

    int pasirinkimas;
    do {
        cout << endl << "Meniu" << endl;
        cout << "1. Perziureti visas uzsakymo prekes" << endl;
        cout << "2. Perziureti pasirinktos kategorijos prekes" << endl;
        cout << "3. Perziureti viso uzsakymo sumas" << endl;
        cout << "4. Perziureti sumas pagal kategorijas" << endl;
        cout << "5. Parodyti visa uzsakymo ataskaita" << endl;
        cout << "6. Issaugoti visa ataskaita i faila" << endl;
        cout << "7. Iseiti" << endl;
        cout << "Jusu pasirinkimas: ";
        cin >> pasirinkimas;

        if (pasirinkimas == 1)
        {
            cout << fixed << setprecision(2);
            cout << endl << "Visos prekes" << endl;
            for (int i = 0; i < prekiuSkaicius; i++)
            {
                double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
                double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
                cout << i + 1 << ". [" << kategorijos[i] << "] " << pavadinimai[i]
                     << " | Kaina: " << kainos[i] << " Eur | Kiekis: " << kiekiai[i]
                     << " | Suma: " << sumaPries << " Eur | Nuolaida: " << nuolaida
                     << " Eur | Moketi: " << (sumaPries - nuolaida) << " Eur" << endl;
            }
        }
        else if (pasirinkimas == 2)
        {
            string pasirinktaKat = pasirinktiKategorija();
            int rasta = 0;
            cout << fixed << setprecision(2);
            cout << endl << "Kategorija: " << pasirinktaKat << " ---" << endl;
            for (int i = 0; i < prekiuSkaicius; i++)
            {
                if (kategorijos[i] == pasirinktaKat)
                {
                    double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
                    double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
                    cout << "- " << pavadinimai[i] << " | Kaina: " << kainos[i]
                         << " Eur | Kiekis: " << kiekiai[i]
                         << " | Moketi: " << (sumaPries - nuolaida) << " Eur" << endl;
                    rasta++;
                }
            }
            if (rasta == 0)
            {
                cout << "Sios kategorijos prekiu nera." << endl;
            }
            cout << "Viso uz kategorija: " << skaiciuotiKategorijosSuma(pasirinktaKat, kategorijos, kainos, kiekiai, prekiuSkaicius) << " Eur" << endl;
        }
        else if (pasirinkimas == 3)
        {
            double visoPries = 0.0, visoNuol = 0.0, visoGalutine = 0.0;
            for (int i = 0; i < prekiuSkaicius; i++)
            {
                double sumaPries = skaiciuotiEilutesSuma(kainos[i], kiekiai[i]);
                double nuolaida = skaiciuotiNuolaida(sumaPries, kiekiai[i]);
                visoPries += sumaPries;
                visoNuol += nuolaida;
                visoGalutine += (sumaPries - nuolaida);
            }
            cout << fixed << setprecision(2);
            cout << endl << "Uzsakymo sumos" << endl;
            cout << "Suma pries nuolaidas: " << visoPries << " Eur" << endl;
            cout << "Bendra nuolaida: " << visoNuol << " Eur" << endl;
            cout << "Galutine moketina suma: " << visoGalutine << " Eur" << endl;
        }
        else if (pasirinkimas == 4)
        {
            cout << fixed << setprecision(2);
            cout << endl << "Sumos pagal kategorijas" << endl;
            cout << "Dviraciai: " << skaiciuotiKategorijosSuma("Dviraciai", kategorijos, kainos, kiekiai, prekiuSkaicius) << " Eur" << endl;
            cout << "Dalys: " << skaiciuotiKategorijosSuma("Dalys", kategorijos, kainos, kiekiai, prekiuSkaicius) << " Eur" << endl;
            cout << "Aksesuarai: " << skaiciuotiKategorijosSuma("Aksesuarai", kategorijos, kainos, kiekiai, prekiuSkaicius) << " Eur" << endl;
        }
        else if (pasirinkimas == 5)
        {
            isvestiAtaskaita(0, "ataskaita.txt", kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius);
        }
        else if (pasirinkimas == 6)
        {
            isvestiAtaskaita(1, "ataskaita.txt", kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius);
        }
        else if (pasirinkimas == 7)
        {
            cout << "Programa baigia darba." << endl;
        }
        else
        {
            cout << "Neteisingas skaicius, pasirinkite nuo 1 iki 7." << endl;
        }

    }
    while (pasirinkimas != 7);

    return 0;
}