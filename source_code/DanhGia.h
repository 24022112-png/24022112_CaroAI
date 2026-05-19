#ifndef DANHGIA_H
#define DANHGIA_H

#include <vector>
#include <cmath>

using namespace std;

//=====================================================================
inline int chamDiemChuoi(int soQuan, int soDauBiChan, bool laMay) {
    // Nếu bị chặn cả 2 đầu và chưa đủ 4 quân để thắng -> Chuỗi "chết", không có giá trị
    if (soDauBiChan == 2 && soQuan < 4) return 0; 
    
    int diem = 0;

    switch (soQuan) {
        case 4: 
        case 5: // Từ 4 quân trở lên là chiến thắng tuyệt đối
            diem = 10000000; // 10 triệu điểm (Cao nhất)
            break;
            
        case 3:
            // 3 quân mở 2 đầu : Chắc chắn sẽ thành 4 quân 
            if (soDauBiChan == 0) diem = 10000000; 
            // 3 quân bị chặn 1 đầu : ép đối thủ BẮT BUỘC phải chặn
            else diem = 80000; 
            break;
            
        case 2:
            // 2 quân mở 2 đầu: Tiềm năng phát triển lớn
            if (soDauBiChan == 0) diem = 5000; 
            // 2 quân bị chặn 1 đầu : Tiềm năng trung 
            else diem = 500;
            break;
            
        case 1:
            // 1 quân mở 2 đầu: Chiếm khoảng trống tốt
            if (soDauBiChan == 0) diem = 50;
            // 1 quân rìa/bị chặn: Ít giá trị
            else diem = 10;
            break;
            
        default:
            if (soQuan > 4) diem = 10000000;
            break;
    }

    if (laMay) {
        return diem; 
    } else {
        return -(diem * 1.15); 
    }
}
//=============================================================================================================
int danhGiaBanCo(const vector<vector<char>>& banCo, int kichThuoc, char mayTinh, char nguoiChoi, char oTrong) {
    int tongDiem = 0;
    
    // 4 hướng quét: Ngang, Dọc, Chéo chính, Chéo phụ
    int huongX[] = {1, 0, 1, 1};
    int huongY[] = {0, 1, 1, -1};
    
    int tam = kichThuoc / 2;

    for (int r = 0; r < kichThuoc; r++) {
        for (int c = 0; c < kichThuoc; c++) {
            if (banCo[r][c] == oTrong) continue;
            
            char phe = banCo[r][c];
            bool laMay = (phe == mayTinh);

            int khoangCachDenTam = abs(r - tam) + abs(c - tam);
            int diemViTri = (kichThuoc - khoangCachDenTam) * 5;
            tongDiem += laMay ? diemViTri : -(diemViTri * 1.2);

            // Đánh giá các chuỗi quân
            for (int h = 0; h < 4; h++) {
                int soQuan = 1;
                int soDauBiChan = 0;
                
                // Kiểm tra lùi lại 1 bước xem đầu phía sau có bị chặn không?
                int rx = r - huongX[h], ry = c - huongY[h];
                bool chanSau = (rx < 0 || rx >= kichThuoc || ry < 0 || ry >= kichThuoc || (banCo[rx][ry] != oTrong && banCo[rx][ry] != phe));
                if (chanSau) {
                    soDauBiChan++;
                }

                if (rx >= 0 && rx < kichThuoc && ry >= 0 && ry < kichThuoc && banCo[rx][ry] == phe) {
                    continue; 
                }

                // Đếm số quân liên tiếp tiến về phía trước
                int nx = r + huongX[h], ny = c + huongY[h];
                while (nx >= 0 && nx < kichThuoc && ny >= 0 && ny < kichThuoc && banCo[nx][ny] == phe) {
                    soQuan++;
                    nx += huongX[h];
                    ny += huongY[h];
                }

                // Kiểm tra xem đầu phía trước có bị chặn không
                bool chanTruoc = (nx < 0 || nx >= kichThuoc || ny < 0 || ny >= kichThuoc || (banCo[nx][ny] != oTrong && banCo[nx][ny] != phe));
                if (chanTruoc) {
                    soDauBiChan++;
                }

                tongDiem += chamDiemChuoi(soQuan, soDauBiChan, laMay);
            }
        }
    }
    return tongDiem;
}

#endif // DANHGIA_H
