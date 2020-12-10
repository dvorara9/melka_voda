#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>


using namespace std;

//definice promennych
int ndx; // pocet uzlu na x
double Termi; // doba reseni
double Cour; // courantovo c. ulohy (1 = mez stability pro linearni ulohu, zde je stabilita cca na 0,1)
double h_prehrady; // pocatecni vyska hladiny prehrady
double h_reky; // pocatecni vyska hladiny reky za prehradou
double l_prehrady; // delka prehrady
double l_reky; // delka reky za prehradou
double u_prehrady; // horizontalni rychlost prehrady
double u_reky; // horizontalni rychlost reky

const double g = 9.81; // tihove zrychleni



int main(int argc, char **argv) {
// nacteni promennych z prikazoveho radku
    if (argc != 10) return -1;
    ndx = stoi(argv[1]);
    Termi = stod(argv[2]);
    Cour = stod(argv[3]);
    h_prehrady = stod(argv[4]);
    h_reky = stod(argv[5]);
    l_prehrady = stod(argv[6]);
    l_reky = stod(argv[7]);
    u_prehrady = stod(argv[8]);
    u_reky = stod(argv[9]);
    std::cout << "Zadali jste nasledujici parametry: " << ndx << " " << Termi << " " << Cour << " " << h_prehrady << " " << h_reky << " " << l_prehrady << " " << l_reky << " " << " " << u_prehrady << " " << u_reky << std::endl;

//______________________________SOLVER______________________________________
// urceni casoveho kroku s ohledem na stabilitu
    double dx = (l_prehrady + l_reky)/ndx;
    double dt = Cour * dx;
// pocet casovych kroku
   int ndt = round(Termi / dt);
// pole reseni konzerv. promennych
    long double fi[ndt][ndx]={0};
    long double m[ndt][ndx]={0};
// pocatecni podminky
    double ndx_prehrady = round((l_prehrady)/(l_prehrady + l_reky) * ndx);
    double ndx_reky = ndx - ndx_prehrady;
// zapis pocatecnich podminek
    for (int i=0; i < ndx; i++) {
        if (i<ndx_prehrady) {
            fi[0][i] = g * h_prehrady;
            m[0][i] = g * h_prehrady * u_prehrady;
        }
        else {
            fi[0][i] = g * h_reky;
            m[0][i] = g * h_reky * u_reky;
        }
    }

// RESENI
   for (int n=0; n < ndt-1; n++) {     //cyklus pres cas
       fi[n + 1][0] = fi[n][0];
       fi[n + 1][ndx-1] = fi[n][ndx-1];
       m[n + 1][0] = m[n][0];
       m[n + 1][ndx-1] = m[n][ndx-1];

       for (int k = 1; k < ndx - 1; k++) {
        fi[n + 1][k] = (1./2)*(fi[n][k + 1] + fi[n][k - 1]) - (Cour/2.)*(m[n][k + 1] - m[n][k - 1]);
        long double Fp = (m[n][k + 1]*m[n][k + 1]/fi[n][k + 1]) + (fi[n][k + 1]*fi[n][k + 1]/2);
        long double Fl = (m[n][k - 1]*m[n][k - 1]/fi[n][k - 1]) + (fi[n][k - 1]*fi[n][k - 1]/2);
        m[n + 1][k] = (1./2)*(m[n][k + 1] + m[n][k - 1]) - (Cour/2)*(Fp - Fl);
       }
   }

   ofstream soubor1("vyska_hladiny.txt");
   ofstream soubor2("horizontalni_rychlost.txt");

    for(int i = 0; i < ndt; i++) {
        for (int j = 0; j < ndx; j++) {
            soubor1 << std::right << std::setw(8) << fi[i][j]/g; // " "
            soubor2 << std::right << std::setw(13) << m[i][j]/fi[i][j]; // " "
        }
        soubor1 << std::endl;
        soubor2 << std::endl;
   }
    soubor1.close();
    soubor2.close();
    ofstream soubor3("konec.txt");
    soubor3.close();

    return 0;
}
