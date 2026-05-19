#ifndef AI_H
#define AI_H

#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include "DanhGia.h"
#include "LogicGame.h" 

using namespace std;

//====================================
struct KetQuaAI {
    int hang;
    int cot;
    int diemDanhGia;
    double thoiGianChay;
    long long soTrangThai;
    long long soLaKetQua;
    int diemMaxThayDuoc;
};

//===========================
struct NuocDiGoiY {
    int hang;
    int cot;
    int diemUuTien;
};

//================================================================================================================
bool soSanhNuocDi(const NuocDiGoiY& a, const NuocDiGoiY& b) {
    return a.diemUuTien > b.diemUuTien;
}

vector<pair<int, int>> layCacNuocDiHopLe(const vector<vector<char>>& banCo, int kichThuoc, char oTrong) {
    vector<pair<int, int>> danhSachNuocDi;
    vector<vector<bool>> daThem(kichThuoc, vector<bool>(kichThuoc, false));
    bool banCoTrong = true;

    for (int hang = 0; hang < kichThuoc; hang++) {
        for (int cot = 0; cot < kichThuoc; cot++) {
            if (banCo[hang][cot] != oTrong) {
                banCoTrong = false;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int hangMoi = hang + dx;
                        int cotMoi = cot + dy;
                        if (hangMoi >= 0 && hangMoi < kichThuoc && cotMoi >= 0 && cotMoi < kichThuoc && 
                            banCo[hangMoi][cotMoi] == oTrong && !daThem[hangMoi][cotMoi]) {
                            danhSachNuocDi.push_back({hangMoi, cotMoi});
                            daThem[hangMoi][cotMoi] = true;
                        }
                    }
                }
            }
        }
    }
    if (banCoTrong) {
        danhSachNuocDi.push_back({kichThuoc / 2, kichThuoc / 2});
    }
    return danhSachNuocDi;
}

//==================================================================================================================
int tinhDiemUuTien(const vector<vector<char>>& banCo, int r, int c, char mayTinh, char nguoiChoi, int kichThuoc) {
    int diem = 0;
    int huongX[] = {1, 0, 1, 1, -1, 0, -1, -1};
    int huongY[] = {0, 1, 1, -1, 0, -1, -1, 1};
    
    for (int h = 0; h < 8; h++) {
        int ta = 0, dich = 0;
        for (int buoc = 1; buoc <= 2; buoc++) {
            int nx = r + buoc * huongX[h];
            int ny = c + buoc * huongY[h];
            if (nx >= 0 && nx < kichThuoc && ny >= 0 && ny < kichThuoc) {
                if (banCo[nx][ny] == mayTinh) ta++;
                else if (banCo[nx][ny] == nguoiChoi) dich++;
                else break;
            } else break;
        }
        diem += ta * 10 + dich * 15; 
    }
    int tam = kichThuoc / 2;
    diem += (kichThuoc - abs(r - tam) - abs(c - tam));
    return diem;
}

//==========================================================================================================================================================
int minimax(vector<vector<char>>& banCo, int doSau, bool laLuotMayTinh, int hangVuaDanh, int cotVuaDanh, int kichThuoc, char mayTinh, char nguoiChoi, char oTrong, long long& soTrangThai, long long& soLaKetQua, int& diemMaxThayDuoc) {
    soTrangThai++;

    if (hangVuaDanh != -1 && cotVuaDanh != -1) {
        char pheVuaDanh = laLuotMayTinh ? nguoiChoi : mayTinh; 
        if (kiemTraThang(banCo, hangVuaDanh, cotVuaDanh, pheVuaDanh, kichThuoc)) {
            soLaKetQua++;
            int diemThang = (pheVuaDanh == mayTinh) ? 1000000 + 10*doSau : -1000000 - 10*doSau;
            if (diemThang > diemMaxThayDuoc) diemMaxThayDuoc = diemThang;
            return diemThang;
        }
    }

    if (doSau == 0) {
        soLaKetQua++; 
        int diem = danhGiaBanCo(banCo, kichThuoc, mayTinh, nguoiChoi, oTrong);
        if (diem > diemMaxThayDuoc) diemMaxThayDuoc = diem;
        return diem;
    }

    vector<pair<int, int>> cacNuocRaw = layCacNuocDiHopLe(banCo, kichThuoc, oTrong);
    if (cacNuocRaw.empty()) {
        soLaKetQua++; 
        if (0 > diemMaxThayDuoc) diemMaxThayDuoc = 0;
        return 0; 
    }

    if (laLuotMayTinh) {
        int giaTriTotNhat = -2000000;
        for (auto nuocDi : cacNuocRaw) {
            banCo[nuocDi.first][nuocDi.second] = mayTinh;
            int giaTri = minimax(banCo, doSau - 1, false, nuocDi.first, nuocDi.second, kichThuoc, mayTinh, nguoiChoi, oTrong, soTrangThai, soLaKetQua, diemMaxThayDuoc);
            banCo[nuocDi.first][nuocDi.second] = oTrong;
            giaTriTotNhat = max(giaTriTotNhat, giaTri);
        }
        return giaTriTotNhat;
    } else {
        int giaTriTotNhat = 2000000;
        for (auto nuocDi : cacNuocRaw) {
            banCo[nuocDi.first][nuocDi.second] = nguoiChoi;
            int giaTri = minimax(banCo, doSau - 1, true, nuocDi.first, nuocDi.second, kichThuoc, mayTinh, nguoiChoi, oTrong, soTrangThai, soLaKetQua, diemMaxThayDuoc);
            banCo[nuocDi.first][nuocDi.second] = oTrong;
            giaTriTotNhat = min(giaTriTotNhat, giaTri);
        }
        return giaTriTotNhat;
    }
}

//======================================================================================================================================
int alphaBeta(vector<vector<char>>& banCo, int doSau, int alpha, int beta, bool laLuotMayTinh, int hangVuaDanh, int cotVuaDanh, int kichThuoc, char mayTinh, char nguoiChoi, char oTrong, long long& soTrangThai, long long& soLaKetQua, int& diemMaxThayDuoc) {
    soTrangThai++;

    if (hangVuaDanh != -1 && cotVuaDanh != -1) {
        char pheVuaDanh = laLuotMayTinh ? nguoiChoi : mayTinh;
        if (kiemTraThang(banCo, hangVuaDanh, cotVuaDanh, pheVuaDanh, kichThuoc)) {
            soLaKetQua++; 
            int diemThang = (pheVuaDanh == mayTinh) ? 1000000 + 10*doSau : -1000000 - 10*doSau;
            if (diemThang > diemMaxThayDuoc) diemMaxThayDuoc = diemThang;
            return diemThang;
        }
    }

    if (doSau == 0) {
        soLaKetQua++; 
        int diem = danhGiaBanCo(banCo, kichThuoc, mayTinh, nguoiChoi, oTrong);
        if (diem > diemMaxThayDuoc) diemMaxThayDuoc = diem;
        return diem;
    }

    vector<pair<int, int>> cacNuocRaw = layCacNuocDiHopLe(banCo, kichThuoc, oTrong);
    if (cacNuocRaw.empty()) {
        soLaKetQua++; 
        if (0 > diemMaxThayDuoc) diemMaxThayDuoc = 0;
        return 0;
    }

    vector<NuocDiGoiY> danhSachSapXep;
    for (auto nd : cacNuocRaw) {
        int diem = tinhDiemUuTien(banCo, nd.first, nd.second, mayTinh, nguoiChoi, kichThuoc);
        danhSachSapXep.push_back({nd.first, nd.second, diem});

    stable_sort(danhSachSapXep.begin(), danhSachSapXep.end(), soSanhNuocDi);

    if (laLuotMayTinh) {
        int giaTriTotNhat = -2000000;
        for (auto nuocDi : danhSachSapXep) {
            banCo[nuocDi.hang][nuocDi.cot] = mayTinh;
            int giaTri = alphaBeta(banCo, doSau - 1, alpha, beta, false, nuocDi.hang, nuocDi.cot, kichThuoc, mayTinh, nguoiChoi, oTrong, soTrangThai, soLaKetQua, diemMaxThayDuoc);
            banCo[nuocDi.hang][nuocDi.cot] = oTrong;
            
            giaTriTotNhat = max(giaTriTotNhat, giaTri);
            alpha = max(alpha, giaTriTotNhat);
            if (beta < alpha) break; 
        }
        return giaTriTotNhat;
    } else {
        int giaTriTotNhat = 2000000;
        for (auto nuocDi : danhSachSapXep) {
            banCo[nuocDi.hang][nuocDi.cot] = nguoiChoi;
            int giaTri = alphaBeta(banCo, doSau - 1, alpha, beta, true, nuocDi.hang, nuocDi.cot, kichThuoc, mayTinh, nguoiChoi, oTrong, soTrangThai, soLaKetQua, diemMaxThayDuoc);
            banCo[nuocDi.hang][nuocDi.cot] = oTrong;
            
            giaTriTotNhat = min(giaTriTotNhat, giaTri);
            beta = min(beta, giaTriTotNhat);
            if (beta < alpha) break; 
        }
        return giaTriTotNhat;
    }
}
     return 0;
}

//=============================================================================================================================
KetQuaAI timNuocDiTotNhat(vector<vector<char>>& banCo, int level, int maxDoSau, int kichThuoc, char mayTinh, char nguoiChoi, char oTrong) {
    int giaTriTotNhat = -2000000;
    pair<int, int> nuocDiToiUu = { -1, -1 };
    
    long long soTrangThai = 0;
    long long soLaKetQua = 0;
    int diemMaxThayDuoc = -2000000;
    
    vector<pair<int, int>> cacNuocRaw = layCacNuocDiHopLe(banCo, kichThuoc, oTrong);
    
    vector<NuocDiGoiY> danhSachSapXep;
    for (auto nd : cacNuocRaw) {
        int diem = tinhDiemUuTien(banCo, nd.first, nd.second, mayTinh, nguoiChoi, kichThuoc);
        danhSachSapXep.push_back({nd.first, nd.second, diem});
    }

    stable_sort(danhSachSapXep.begin(), danhSachSapXep.end(), soSanhNuocDi);
    
    auto thoiGianBatDau = chrono::high_resolution_clock::now();

    for (auto nuocDi : danhSachSapXep) {
        banCo[nuocDi.hang][nuocDi.cot] = mayTinh; 
        
        int giaTri;
        if (level == 1) {
            giaTri = minimax(banCo, maxDoSau - 1, false, nuocDi.hang, nuocDi.cot, kichThuoc, mayTinh, nguoiChoi, oTrong, soTrangThai, soLaKetQua, diemMaxThayDuoc);
        } else if(level == 2) {
            giaTri = alphaBeta(banCo, maxDoSau - 1, -2000000, 2000000, false, nuocDi.hang, nuocDi.cot, kichThuoc, mayTinh, nguoiChoi, oTrong, soTrangThai, soLaKetQua, diemMaxThayDuoc);
        }
        
        banCo[nuocDi.hang][nuocDi.cot] = oTrong; 

        if (giaTri > giaTriTotNhat) {
            giaTriTotNhat = giaTri;
            nuocDiToiUu = {nuocDi.hang, nuocDi.cot};
        }
    }

    auto thoiGianKetThuc = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> thoiGianChay = thoiGianKetThuc - thoiGianBatDau;

    return { nuocDiToiUu.first, nuocDiToiUu.second, giaTriTotNhat, thoiGianChay.count(), soTrangThai, soLaKetQua, diemMaxThayDuoc };
}

//=============================================================================================================================
void chayKiemThuSoSanh(string tenTrangThai, const vector<vector<char>>& banCo, int kichThuoc) {
    vector<int> cacDoSau = {1, 2, 3, 4};
    
    for (int ds : cacDoSau) {

        vector<vector<char>> banCoCopy1 = banCo;
        KetQuaAI kqMinimax = timNuocDiTotNhat(banCoCopy1, 1, ds, kichThuoc, MAY_TINH, NGUOI_CHOI, O_TRONG);
        string ndMinimax = "(" + to_string(kqMinimax.hang + 1) + ", " + to_string(kqMinimax.cot + 1) + ")";

        vector<vector<char>> banCoCopy2 = banCo;
        KetQuaAI kqAlphaBeta = timNuocDiTotNhat(banCoCopy2, 2, ds, kichThuoc, MAY_TINH, NGUOI_CHOI, O_TRONG);
        string ndAlphaBeta = "(" + to_string(kqAlphaBeta.hang + 1) + ", " + to_string(kqAlphaBeta.cot + 1) + ")";

        cout << left << setw(15) << (ds == 1 ? tenTrangThai : "")
             << setw(15) << "Minimax"
             << setw(8)  << ds
             << setw(12) << ndMinimax
             << right << setw(12) << kqMinimax.diemDanhGia
             << setw(12) << kqMinimax.soTrangThai
             << setw(15) << fixed << setprecision(4) << kqMinimax.thoiGianChay  << "\n"; 

        cout << left << setw(15) << ""
             << setw(15) << "Alpha-Beta"
             << setw(8)  << ds
             << setw(12) << ndAlphaBeta
             << right << setw(12) << kqAlphaBeta.diemDanhGia
             << setw(12) << kqAlphaBeta.soTrangThai
             << setw(15) << fixed << setprecision(4) << kqAlphaBeta.thoiGianChay  << "\n";
    }
    cout << string(89, '-') << "\n";
}

#endif // AI_H
