#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <string>

using namespace std;

int licznikstopnia1=0;
int licznikcalosciowy=0;

double R_val;
float c_const;
double D_prob;
double rozklad[256];
const int MAX_K=255;
double suma_dist=0;

int kontrol;
char** inputs;
int liczbaPakietow;
unsigned char* degre;
unsigned char* encodedpakiet;
unsigned char* temporary;
unsigned char* sumach;

int licznika;

int dlugosc(ifstream& c_stream,int div){
    char znak;
    int liczbaznakow=0;
    while(c_stream.get(znak)){
        liczbaznakow++;
    }
    c_stream.clear();
    c_stream.seekg(0);
    kontrol=liczbaznakow%div;
    return(liczbaznakow+div-1)/div;
}

void wczytajpakiety(int pakietln){
    ifstream file("test.txt",ios::in|ios::binary);
    if(!file){
        cout<<"Nie udalo sie otworzyc pliku test.txt!"<<endl;
        return;
    }
    liczbaPakietow=dlugosc(file,pakietln);
    inputs=new char*[liczbaPakietow];
    for(int i=0;i<liczbaPakietow;i++){
        inputs[i]=new char[pakietln];
        memset(inputs[i],0,pakietln);
    }
    for(int j=0;j<liczbaPakietow;j++){
        file.read(inputs[j],pakietln);
    }
    file.close();
}

void xorowanie(int a,int pakietlenght){
    for(int l=0;l<pakietlenght;l++){
        encodedpakiet[l]^=inputs[a-1][l];
    }
}

double calculateR(int inp){
    return(c_const*log(inp/D_prob)*sqrt(inp));
}

void tablicasoliton(int inp){
    double rho[MAX_K+1]={0.0};
    rho[1]=1.0/inp;
    for(int i=2;i<=inp;++i){
        rho[i]=1.0/(i*(i-1));
    }
    for(int i=1;i<=inp;++i){
        rozklad[i]=rho[i];
    }
}

void tablica_robust_soliton(int inp,double R){
    double rho[MAX_K+1]={0.0};
    rho[1]=R/(inp);
    if(1<=((inp/R)-1)){
        for(int i=2;i<((inp/R)-1);i++){
            if((i==(inp/R))&&(i!=1))
                rho[i]=(R*log(R/D_prob));
            else
                rho[i]=R/(i*inp);
        }
    }
    for(int i=1;i<=inp;++i){
        rozklad[i]=rozklad[i]+rho[i];
        suma_dist=suma_dist+rozklad[i];
    }
    for(int i=1;i<=inp;++i){
        rozklad[i]=rozklad[i]/suma_dist;
    }
    for(int i=2;i<=inp;i++){
        rozklad[i]=rozklad[i]+rozklad[i-1];
    }
}

int randrobustsoliton(int inp,unsigned int seed){
    static mt19937 gen(seed);
    uniform_real_distribution<> dist(0.0,1.0);
    double r=dist(gen);
    for(int i=1;i<=inp;++i){
        if(r<rozklad[i])return i;
    }
    return 1;
}

void degree(int pakietlenght,int seed){
    bool aktywacja=true;
    srand(seed);
    int table=randrobustsoliton(liczbaPakietow,seed);
    if(table==1)licznikstopnia1+=1;
    licznikcalosciowy+=1;
    degre=new unsigned char[table+1];
    degre[0]=(unsigned char)table;
    encodedpakiet=new unsigned char[pakietlenght];
    memset(encodedpakiet,0,pakietlenght);
    int i=1;
    while(i<=table){
        int temp=(rand()%liczbaPakietow)+1;
        bool powtorka=false;
        for(int j=1;j<i;j++){
            if(degre[j]==temp){
                powtorka=true;
                break;
            }
        }
        if(!powtorka){
            degre[i]=(unsigned char)temp;
            if(aktywacja){
                memcpy(encodedpakiet,inputs[temp-1],pakietlenght);
                aktywacja=false;
            }else{
                xorowanie(temp,pakietlenght);
            }
            i++;
        }
    }
}

void combine(int len1){
    int len2=((int)degre[0]+1);
    temporary=new unsigned char[len1+len2];
    licznika=len1+len2;
    for(int l=0;l<len1;l++)temporary[l]=encodedpakiet[l];
    for(int l=0;l<len2;l++)temporary[len1+l]=degre[l];
}

void combine2(int len_sum,int current_len){
    unsigned char* new_buf=new unsigned char[current_len+len_sum];
    for(int l=0;l<current_len;l++)new_buf[l]=temporary[l];
    for(int l=0;l<len_sum;l++)new_buf[current_len+l]=sumach[l];
    delete[] encodedpakiet;
    encodedpakiet=new_buf;
    licznika=current_len+len_sum;
}

void inttochar(int suma_val,int pakietln){
    int i=1;
    int counterofch=1;
    do{
        i=i*10;
        if(i<suma_val)
            counterofch+=1;
    }while(i<suma_val);
    sumach=new unsigned char[counterofch];
    for(int j=0;j<counterofch;j++){
        sumach[j]=suma_val%10;
        suma_val=suma_val/10;
    }
    combine2(counterofch,pakietln);
}

void calcsum(int pakietln){
    int suma1=0;
    int len_to_sum=pakietln+(int)degre[0]+1;
    for(int i=0;i<len_to_sum;i++){
        suma1+=temporary[i];
    }
    inttochar(suma1,len_to_sum);
}

void deletemem(){
    delete[] degre;
    delete[] encodedpakiet;
    delete[] temporary;
    delete[] sumach;
    degre=nullptr;
    encodedpakiet=nullptr;
    temporary=nullptr;
    sumach=nullptr;
}

void deleteinputs(){
    if(inputs!=nullptr){
        for(int k=0;k<liczbaPakietow;k++){
            delete[] inputs[k];
        }
        delete[] inputs;
        inputs=nullptr;
    }
}

int calculateNumOfEncoding(){
    if(liczbaPakietow<=0||D_prob<=0){
        return liczbaPakietow;
    }
    double k=static_cast<double>(liczbaPakietow);
    double d=D_prob;
    double ln_part=log(k/d);
    double ln_squared=pow(ln_part,2);
    double result=k+(c_const*(sqrt(k)*ln_squared));
    return static_cast<int>(ceil(result));
}

int main(){
    int pakietlenght=3;
    wczytajpakiety(pakietlenght);
    cout<<"Podaj prawdopodobienstwo niepowodzenia (np. 0.1): ";
    cin>>D_prob;
    cout<<"Podaj stala c (np. 0.01): ";
    cin>>c_const;
    ofstream plik_expected("example/expected.txt",ios::out|ios::trunc);
    if(plik_expected){
        plik_expected<<calculateNumOfEncoding();
        plik_expected.close();
    }
    cout<<calculateNumOfEncoding();
    string a;
    cin>>a;
    tablicasoliton(liczbaPakietow);
    tablica_robust_soliton(liczbaPakietow,calculateR(liczbaPakietow));

    for(int k=1;k<=10000;k++){
        string name="example/"+to_string(k)+".txt";
        ofstream plik(name,ios::out|ios::binary|ios::trunc);
        if(!plik)continue;
        plik<<liczbaPakietow<<";";
        plik<<kontrol<<";";
        for(int m=0;m<(liczbaPakietow*5);m++){
            degree(pakietlenght,(int)time(0)+k+m);
            combine(pakietlenght);
            calcsum(pakietlenght);
            for(int j=0;j<licznika;j++){
                plik<<encodedpakiet[j];
            }
            plik<<";";
            deletemem();
        }
        plik.close();
        if(k%100==0)cout<<"Wygenerowano "<<k<<" plikow..."<<endl;
    }
    deleteinputs();
    cout<<"Zakonczono generowanie. Statystyki: "<<endl;
    cout<<"Liczba pakietow stopnia 1: "<<licznikstopnia1<<endl;
    cout<<"Liczba operacji: "<<licznikcalosciowy<<endl;
    return 0;
}
