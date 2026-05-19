#ifndef LOGICGAME_H
#define LOGICGAME_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>   
#include <sstream>  

using namespace std;

const char NGUOI_CHOI = 'X';
const char MAY_TINH = 'O';
const char O_TRONG = '.';
//===================================
struct nLuot {
    int hang = -1;
    int cot = -1;
    double thoiGian = 0.0;
    int diem = 0;
    long long soTrangThai = 0;     
    long long soLaKetQua = 0;       
    int diemMaxThayDuoc = -2000000; 
};
//=====================================
void xoaManHinh() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
//================================================================
string tinhTyLeThang(int diem) {
    if (diem >= 900000) return "100.0% (Thang)";
    if (diem <= -900000) return "0.0% (Thua)";

    double phanTram = 50.0 + 50.0 * tanh(diem / 6000.0);
    
    if (phanTram > 99.9) phanTram = 99.9;
    if (phanTram < 0.1) phanTram = 0.1;

    stringstream ss;
    ss << fixed << setprecision(1) << phanTram << "%";

    if (phanTram >= 85.0) ss << " (Ap dao)";
    else if (phanTram >= 60.0) ss << " (Loi the)";
    else if (phanTram <= 15.0) ss << " (Nguy kich)";
    else if (phanTram <= 40.0) ss << " (Bat loi)";
    else ss << " (Can bang)";

    return ss.str();
}
//=========================================================================================================
void khoiTaoBanCo(vector<vector<char>>& banCo, int kichThuoc) {
    banCo.assign(kichThuoc, vector<char>(kichThuoc, O_TRONG));
}
//=========================================================================================================
void inGiaoDien(const vector<vector<char>>& banCo, int kichThuoc, int level, int doSau, nLuot ai, nLuot nguoi) {
    xoaManHinh();
    cout << "AI: ( Loi The: " << tinhTyLeThang(ai.diem) << " )\t\tLevel: " << level << "\n";
    if (ai.hang != -1)
        cout << "Nuoc vua danh: (" << ai.hang + 1 << ", " << ai.cot + 1 << ")\t\t\tDo sau tim kiem: " << doSau << "\n";
    else
        cout << "Nuoc vua danh: \t\t\t\tDo sau tim kiem: " << doSau << "\n";
    cout << "Thoi gian suy nghi: " << fixed << setprecision(2) << ai.thoiGian << " ms\n";
    
    if (ai.soTrangThai > 0) {
        cout << "Tong so nut duyet qua: " << ai.soTrangThai << " \tTong so la duyet qua: " << ai.soLaKetQua << "\n";
        cout << "Diem toi da nhan duoc: " << ai.diem << "\n";
    }
    
    cout << "-----------------------------------------------------------------\n\n";

    cout << "     ";
    for (int i = 1; i <= kichThuoc; i++) {
        cout << setw(2) << i << " ";
    }
    cout << "\n";

    for (int i = 0; i < kichThuoc; i++) {
        cout << setw(2) << i + 1 << " | ";
        for (int j = 0; j < kichThuoc; j++) {
            cout << setw(2) << banCo[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n-----------------------------------------------------------------\n";
    cout << "Nguoi choi: ( Loi The: " << tinhTyLeThang(-ai.diem) << " )\n"; 
    
    if (nguoi.hang != -1) {
        cout << "Nuoc vua danh: (" << nguoi.hang + 1 << ", " << nguoi.cot + 1 << ")\n";
    } else {
        cout << "Nuoc vua danh: \n";
    }
        
    cout << "Thoi gian suy nghi: " << fixed << setprecision(2) << nguoi.thoiGian << " ms\n";
    cout << "=================================================================\n";
}
//=============================================================================================
bool kiemTraNuocDiHopLe(const vector<vector<char>>& banCo, int hang, int cot, int kichThuoc) {
    if (hang < 0 || hang >= kichThuoc || cot < 0 || cot >= kichThuoc) return false;
    return banCo[hang][cot] == O_TRONG;
}

bool datQuanCo(vector<vector<char>>& banCo, int hang, int cot, char phe, int kichThuoc) {
    if (kiemTraNuocDiHopLe(banCo, hang, cot, kichThuoc)) {
        banCo[hang][cot] = phe;
        return true;
    }
    return false;
}
//=========================================================================================================
bool kiemTraThang(const vector<vector<char>>& banCo, int hangVuaDanh, int cotVuaDanh, char phe, int kichThuoc) {
    if (hangVuaDanh == -1 || cotVuaDanh == -1) return false;

    int huongX[] = {1, 0, 1, 1};
    int huongY[] = {0, 1, 1, -1};
    
    for (int h = 0; h < 4; h++) {
        int dem = 1; 
        
        for (int buoc = 1; buoc < 4; buoc++) {
            int nx = hangVuaDanh + buoc * huongX[h];
            int ny = cotVuaDanh + buoc * huongY[h];
            if (nx >= 0 && nx < kichThuoc && ny >= 0 && ny < kichThuoc && banCo[nx][ny] == phe) {
                dem++;
            } else {
                break; 
            }
        }
        
        for (int buoc = 1; buoc < 4; buoc++) {
            int nx = hangVuaDanh - buoc * huongX[h];
            int ny = cotVuaDanh - buoc * huongY[h];
            if (nx >= 0 && nx < kichThuoc && ny >= 0 && ny < kichThuoc && banCo[nx][ny] == phe) {
                dem++;
            } else {
                break;
            }
        }
        if (dem >= 4) return true;
    }
    
    return false;
}
//=============================================================================================
vector<pair<string, vector<vector<char>>>> taoCacTrangThaiKiemThu(int kichThuoc) {
    vector<pair<string, vector<vector<char>>>> cacTrangThai;
    vector<vector<char>> banCo;

    int giua = (kichThuoc - 1) / 2; 

    khoiTaoBanCo(banCo, kichThuoc);
    banCo[giua][giua] = NGUOI_CHOI;
    cacTrangThai.push_back({"KHAI CUOC", banCo});

    khoiTaoBanCo(banCo, kichThuoc);
    if (kichThuoc >= 5) {
        banCo[giua][giua] = NGUOI_CHOI; banCo[giua][giua+1] = MAY_TINH;
        banCo[giua-1][giua] = NGUOI_CHOI; banCo[giua+1][giua] = MAY_TINH;
    }
    cacTrangThai.push_back({"TRUNG CUOC", banCo});

    khoiTaoBanCo(banCo, kichThuoc);
    if (kichThuoc >= 5) {
        banCo[0][0] = MAY_TINH; banCo[0][1] = MAY_TINH; banCo[0][2] = MAY_TINH;
        banCo[1][0] = NGUOI_CHOI; banCo[1][1] = NGUOI_CHOI; banCo[1][2] = NGUOI_CHOI;
    }
    cacTrangThai.push_back({"AI SAP WIN", banCo});

    khoiTaoBanCo(banCo, kichThuoc);
    if (kichThuoc >= 7) {
        banCo[giua][giua] = NGUOI_CHOI; banCo[giua][giua+1] = NGUOI_CHOI; banCo[giua][giua+2] = NGUOI_CHOI;
        banCo[giua+1][giua] = MAY_TINH; banCo[giua+2][giua] = MAY_TINH;
    }
    cacTrangThai.push_back({"AI SAP THUA", banCo});

    khoiTaoBanCo(banCo, kichThuoc);
    if (kichThuoc >= 5) {
        banCo[giua-1][giua-1] = NGUOI_CHOI; banCo[giua-1][giua] = MAY_TINH; banCo[giua-1][giua+1] = NGUOI_CHOI;
        banCo[giua][giua-1] = MAY_TINH; banCo[giua][giua] = NGUOI_CHOI; banCo[giua][giua+1] = MAY_TINH;
    }
    cacTrangThai.push_back({"DOI CONG", banCo});
    
    khoiTaoBanCo(banCo, kichThuoc);
    if (kichThuoc >= 5) {
        banCo[0][0] = NGUOI_CHOI;
        banCo[kichThuoc-1][kichThuoc-1] = MAY_TINH;
    }
    cacTrangThai.push_back({"ROI RAC", banCo});

    return cacTrangThai;
}
#endif // LOGICGAME_H
