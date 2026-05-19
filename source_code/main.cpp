#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include "LogicGame.h"
#include "DanhGia.h"
#include "AI.h"

using namespace std;

int main() {
    int kichThuoc;      
    int luaChonLevel;   
    int doSauTimKiem;   
    
    cout << "----------THIET LAP TRO CHOI--------------" << endl;
    cout << "Nhap kich thuoc map:" << endl;
    cin >> kichThuoc;
    cout << "Nhap level ( 1- Minimax ; 2- Alpha-Beta ; 3- Bang Mo Phong Kiem ThU ):" << endl;
    cin >> luaChonLevel;

    if (luaChonLevel == 3) {
        xoaManHinh();
        vector<pair<string, vector<vector<char>>>> danhSachBanCo = taoCacTrangThaiKiemThu(kichThuoc);

        cout << string(89, '-') << "\n";
        cout << left << setw(15) << "State"
             << setw(15) << "Algorithm"
             << setw(8)  << "Depth"
             << setw(12) << "Move"
             << right << setw(12) << "Score"
             << setw(12) << "Nodes"
             << setw(15) << "Time (Ms)" << "\n";
        cout << string(89, '-') << "\n";

        for (int i = 0; i < danhSachBanCo.size(); i++) {
            chayKiemThuSoSanh(danhSachBanCo[i].first, danhSachBanCo[i].second, kichThuoc);
        }
        
        cout << "\n[!] Da hoan thanh kiem thu tren " << danhSachBanCo.size() << " trang thai voi do sau 1->4." << endl;
        system("pause"); 
        return 0;
    }

    cout << "Nhap do sau tim kiem:" << endl;
    cin >> doSauTimKiem;
    
    xoaManHinh();
    vector<vector<char>> banCo;
    khoiTaoBanCo(banCo, kichThuoc);

    nLuot aiThongTin, nguoiThongTin;

    int tongSoNuocDaDi = 0;
    int maxNuocDi = kichThuoc * kichThuoc;

    inGiaoDien(banCo, kichThuoc, luaChonLevel, doSauTimKiem, aiThongTin, nguoiThongTin);

    while (true) {
        
        int hangNhap, cotNhap;
        
        auto batDauNguoi = chrono::high_resolution_clock::now();
        
        while (true) {
            cout << "Luot cua ban -> Nhap Hang va Cot (VD: 5 6): ";
            cin >> hangNhap >> cotNhap;
            if (datQuanCo(banCo, hangNhap - 1, cotNhap - 1, NGUOI_CHOI, kichThuoc)) {
                break;
            }
            cout << "[!] Nuoc di khong hop le, danh lai.\n";
        }

        auto ketThucNguoi = chrono::high_resolution_clock::now();
        
        nguoiThongTin.hang = hangNhap - 1;
        nguoiThongTin.cot = cotNhap - 1;
        nguoiThongTin.thoiGian = chrono::duration<double, std::milli>(ketThucNguoi - batDauNguoi).count();
        
        tongSoNuocDaDi++;

        inGiaoDien(banCo, kichThuoc, luaChonLevel, doSauTimKiem, aiThongTin, nguoiThongTin);

        if (kiemTraThang(banCo, nguoiThongTin.hang, nguoiThongTin.cot, NGUOI_CHOI, kichThuoc)) {
            cout << "\n=================================================================\n";
            cout << "                AI CHA! AN DUOC TAO CO DAY            \n";
            break;
        }
        if (tongSoNuocDaDi == maxNuocDi) {
            cout << "\n=================================================================\n";
            cout << "                    HOA! LUYEN THEM DI ROI AN TAO              \n";
            break;
        }

        cout << "\ntao dang suy nghi...\n";
        
        auto batDauAI = chrono::high_resolution_clock::now();

        KetQuaAI kq = timNuocDiTotNhat(banCo, luaChonLevel, doSauTimKiem, kichThuoc, MAY_TINH, NGUOI_CHOI, O_TRONG);
        
        datQuanCo(banCo, kq.hang, kq.cot, MAY_TINH, kichThuoc);

        auto ketThucAI = chrono::high_resolution_clock::now();
        
        aiThongTin.hang = kq.hang;
        aiThongTin.cot = kq.cot;
        aiThongTin.thoiGian = chrono::duration<double, std::milli>(ketThucAI - batDauAI).count();
        aiThongTin.diem = kq.diemDanhGia; 
        
        aiThongTin.soTrangThai = kq.soTrangThai;
        aiThongTin.soLaKetQua = kq.soLaKetQua;
        aiThongTin.diemMaxThayDuoc = kq.diemMaxThayDuoc;

        tongSoNuocDaDi++; 

        inGiaoDien(banCo, kichThuoc, luaChonLevel, doSauTimKiem, aiThongTin, nguoiThongTin);

        if (kiemTraThang(banCo, aiThongTin.hang, aiThongTin.cot, MAY_TINH, kichThuoc)) {
            cout << "\n=================================================================\n";
            cout << "                 UI XOI OI! NON.              \n";
            break;
        }

        if (tongSoNuocDaDi == maxNuocDi) {
            cout << "\n=================================================================\n";
            cout << "                    HOA! CO AN DUOC TAO VOI.           \n";
            break;
        }
    }

    return 0;
}
