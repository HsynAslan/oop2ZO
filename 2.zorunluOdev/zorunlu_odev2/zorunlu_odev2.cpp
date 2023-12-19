// Kevser Aslan - 417016
// Elif Özbay - 394764
// OOP 2. Zorunlu Ödev


#include <iostream>
#include <locale.h>
#include <fstream>
#include <string>
#include <algorithm> // sort fonksiyonu için gerekli başlık dosyası
#include <sstream>
#include <numeric> // accumulate için gerekli başlık dosyası
int main() {
    using namespace std;
    setlocale(LC_ALL, "Turkish"); // türkçe karakter için 

    ifstream adayDosya("candDat.txt"); // dosya okuma
    ifstream oyDosya("voteDat.txt"); // dosya okuma
    string aday;
    string oy;

    // aday sayisi hem vector kullanim izni olmadığından hem de dinamik bellek yönetimine uygun olmadığından sabit olarak belirlenmek durumunda kalınmıştır
    int adaySayisi = 6; 

    string candidatesName[6]; // aday dizisi

    // dosya okuma
    int sayac = 0;
    if (adayDosya.is_open()) {
        while (getline(adayDosya, aday)) {
            candidatesName[sayac] = aday;
            sayac++;
        }
        adayDosya.close();

        //cout << "Dosya " << adaySayisi << " satırdan oluşuyor." << endl;

        // adayları alfabetik olarak sırala
        sort(candidatesName, candidatesName + adaySayisi);

        // adayları bastır
        /*for (size_t i = 0; i < adaySayisi; i++) {
            cout << i + 1 << ". eleman: " << candidatesName[i] << endl;
        }*/
    }
    else {
        cout << "Dosya açılamadı." << endl;
    }

    const int bolge_sayisi = 4;
    const int aday_sayisi = 6;
    const int sutun_sayisi = bolge_sayisi + 1; // Bölge sayısı + 1 (Bölge + totalVote)

    // aday sutünü belirleme
    string adayIsimleri[aday_sayisi];
    for (size_t i = 0; i < aday_sayisi; i++) { 
        adayIsimleri[i] = candidatesName[i];
    }

    // bölge isimleri
    string bolgeIsimleri[bolge_sayisi] = { "Bölge1", "Bölge2", "Bölge3", "Bölge4" };

    // iki boyutlu dizi oluştur
    int votesByRegion[bolge_sayisi][aday_sayisi] = { 0 };

    // tabloyu 0 ile doldur
    for (size_t j = 0; j < adaySayisi; j++) {
        for (int i = 0; i < bolge_sayisi; i++) {
            votesByRegion[i][j] = 0;
        }
    }


    // toplam oy oluştur
    int totalVotes[aday_sayisi] = { 0 }; 

    // parse etme işlemi için gerekli değişkenler
    string name = "";
    int bolgeNumarasi = 0;
    int aldigiOyMiktari = 0;

    string kelime;
    if (oyDosya.is_open()) {
        while (getline(oyDosya, oy)) {
            // oy -> Balto 4 272
            istringstream iss(oy);
            while (iss >> kelime) { // 3 adet kelimeye ayırıp gerekli yerlerde değişkenlere yazdık
                if (name.empty()) {
                    // isim boşsa kelime aday ismidir
                    name = kelime;
                }
                else if (bolgeNumarasi == 0) {
                    // İsim dolu, ancak bölge numarası henüz 0 ise, bu kelime bölge numarasıdır
                    bolgeNumarasi = stoi(kelime);
                }
                else {
                    // İsim ve bölge numarası dolu, bu kelime oy miktarıdır
                    aldigiOyMiktari = stoi(kelime);

                    // Aday ismini ve bölge numarasını bul
                    for (int i = 0; i < adaySayisi; i++) {
                        if (name == candidatesName[i]) {
                            votesByRegion[bolgeNumarasi - 1][i] += aldigiOyMiktari;
                            totalVotes[i] += aldigiOyMiktari;
                            // isim eşleşti ve tabloya değerleri ekledik
                            break; // isim bulunduğunda döngüden çık
                        }
                    }

                    // değişkenleri çıkarken sıfırlıyoruz
                    name = "";
                    bolgeNumarasi = 0;
                    aldigiOyMiktari = 0;
                }
            }
        }
        oyDosya.close();
    }
    else {
        cout << "Dosya açılamadı." << endl;
    }

    // en fazla oy alan adayı bulma işlemi
    int kazananIndex = distance(totalVotes, max_element(totalVotes, totalVotes + aday_sayisi));
    string kazananIsim = adayIsimleri[kazananIndex];

    // tabloyu istenen formatta gösterme işlemi
    cout << "\t";
    for (int i = 0; i < bolge_sayisi; i++) {
        cout << bolgeIsimleri[i] << "\t";
    }
    cout << "Toplam Oy\t\n";

    for (int i = 0; i < aday_sayisi; i++) {
        cout << adayIsimleri[i] << "\t";
        for (int j = 0; j < bolge_sayisi; j++) {
            cout << votesByRegion[j][i] << "\t";
        }
        cout << totalVotes[i] << "\n";
    }

    // kazanan kişiyi istenen formatta ekrana bastırma işlemi
    cout << "Winner: " << kazananIsim << ", Votes Received: " << totalVotes[kazananIndex] << endl;
    cout << "Total votes polled: " << accumulate(totalVotes, totalVotes + aday_sayisi, 0) << endl;

    return 0;
}
