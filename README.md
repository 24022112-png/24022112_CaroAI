================================================================================
HƯỚNG DẪN SỬ DỤNG VÀ CHẠY CHƯƠNG TRÌNH - DỰ ÁN CỜ CARO AI
================================================================================

Chương trình này được viết hoàn toàn bằng ngôn ngữ C++ tiêu chuẩn (STL), áp dụng hai thuật toán 
tìm kiếm cây trò chơi là Minimax và Cắt tỉa Alpha-Beta (Alpha-Beta Pruning). 

Dự án được tối ưu hóa đặc biệt với cấu trúc sắp xếp nước đi ổn định (stable_sort) 
và hàm đánh giá chiến thuật nhạy bén, giúp AI có khả năng phòng thủ khắt khe và tấn 
công sắc bén. Ngoài ra, chương trình hỗ trợ Chế độ mô phỏng thực nghiệm ở Level 3 giúp
tự động chạy mô phỏng và đối chứng hiệu năng thuật toán một cách trực quan.

--------------------------------------------------------------------------------
1. CẤU TRÚC CÁC FILE TRONG DỰ ÁN
--------------------------------------------------------------------------------

Mã nguồn của dự án được module hóa thành các file rõ ràng để dễ quản lý:
- main.cpp     : Chứa hàm khởi tạo, cấu hình thiết lập ban đầu và vòng lặp chính.
- LogicGame.h  : Quản lý ma trận bàn cờ, hiển thị giao diện CLI, kiểm tra nước cờ 
                 hợp lệ, kiểm tra điều kiện thắng/hòa và tạo các kịch bản mẫu.
- DanhGia.h    : "Bộ não" chấm điểm thế cờ (Heuristic), phân loại mức độ đe dọa 
                 của chuỗi quân.
- AI.h         : Triển khai lõi thuật toán Minimax, Alpha-Beta, bộ lọc tìm nước đi 
                 ưu tiên và hàm xuất dữ liệu bảng kiểm thử so sánh.
- requirements.txt : File đặc tả kỹ thuật mô tả chi tiết về môi trường và hệ thống.

--------------------------------------------------------------------------------
2. YÊU CẦU HỆ THỐNG
--------------------------------------------------------------------------------

Do chương trình chỉ sử dụng thư viện chuẩn C++ (STL), bạn không cần cài đặt thêm 
bất kỳ thư viện bên thứ ba nào. Hệ thống chỉ yêu cầu:
- Trình biên dịch C++ hỗ trợ tiêu chuẩn C++17 hoặc cao hơn 
- Công cụ dòng lệnh: Terminal (Linux/macOS) hoặc CMD/PowerShell (Windows).

--------------------------------------------------------------------------------
3. HƯỚNG DẪN BIÊN DỊCH (COMPILATION)
--------------------------------------------------------------------------------

Mở Terminal hoặc CMD tại thư mục chứa mã nguồn và chạy lệnh:

* Trên hệ điều hành Linux / macOS (Sử dụng trình biên dịch g++):
  g++ main.cpp -o CaroAI -O3 -std=c++17

* Trên hệ điều hành Windows (Sử dụng MinGW g++):
  g++ main.cpp -o CaroAI.exe -O3 -std=c++17

* Trên Microsoft Visual Studio (MSVC):
  1. Tạo một dự án C++ trống (Empty Project) và add cả 4 file vào dự án.
  2. Trên thanh công cụ, chuyển cấu hình từ "Debug" sang "Release" (Tính năng này 
     sẽ tự động kích hoạt chế độ tối ưu hóa hiệu năng tương đương -O3).
  3. Bấm tổ hợp phím Ctrl + F5 để biên dịch và chạy ván cờ.

--------------------------------------------------------------------------------
4. HƯỚNG DẪN CHẠY CHƯƠNG TRÌNH (EXECUTION)
--------------------------------------------------------------------------------

Sau khi biên dịch thành công, bạn gọi file thực thi bằng lệnh sau:

* Trên Linux / macOS:
  ./CaroAI

* Trên Windows:
  CaroAI.exe

--------------------------------------------------------------------------------
5. CẤU HÌNH CÁC CHẾ ĐỘ CHƠI (LEVELS)
--------------------------------------------------------------------------------

Khi chương trình khởi chạy, bạn sẽ tiến hành cấu hình lần lượt 3 thông số:

Bước 1: Nhập kích thước map
- Nhập một số nguyên đại diện cho cạnh bàn cờ vuông (Ví dụ: nhập 10 tương đương 
  bàn cờ kích thước 10x10). Khuyến khích chọn kích thước từ 10 trở lên để các 
  trạng thái kiểm thử hoạt động tối ưu nhất.

Bước 2: Nhập Level (Chế độ hoạt động)
- Nhập 1 (Minimax): AI dùng thuật toán duyệt vét cạn truyền thống. Duyệt qua tất 
  cả các trạng thái có thể xảy ra.
- Nhập 2 (Alpha-Beta): AI dùng thuật toán cắt tỉa Alpha-Beta kết hợp sắp xếp nước 
  đi ưu tiên. Giúp loại bỏ các nhánh cờ yếu từ sớm, tăng tốc độ phản hồi đáng kể.
- Nhập 3 (Bảng Mô Phỏng Kiểm Thử): Chương trình sẽ chuyển sang chế độ tự động chạy 
  thực nghiệm. Máy sẽ tự nạp 6 kịch bản thế trận mẫu bao gồm:
    + KHAI CUOC   : Người chơi đánh 1 quân lẻ ở trung tâm bàn cờ.
    + TRUNG CUOC  : Giai đoạn giữa trận với vài nước đi đan xen phức tạp.
    + AI SAP WIN  : Máy tính đã có chuỗi 3 quân liên tiếp, chuẩn bị giành chiến thắng.
    + AI SAP THUA : Người chơi có chuỗi nguy hiểm, ép AI bắt buộc phải nhảy vào chặn.
    + DOI CONG    : Thế trận giằng co đôi công gay cấn ở khu vực trung tâm.
    + ROI RAC     : Các quân cờ nằm xa nhau để thử nghiệm khả năng mở rộng node.
  Với mỗi kịch bản, máy sẽ chạy cả 2 thuật toán từ Độ sâu 1 đến Độ sâu 4 để in ra 
  một bảng thống kê so sánh chi tiết.

Bước 3: Nhập độ sâu tìm kiếm (Chỉ áp dụng khi bạn chọn Level 1 hoặc Level 2)
- Độ sâu (Depth) quyết định số bước cờ AI sẽ "nhìn trước" về tương lai để tính toán.
- Khuyến khích nhập độ sâu từ 1 đến 4 để đảm bảo AI suy nghĩ mượt mà (< 1 giây).
<img width="926" height="289" alt="image" src="https://github.com/user-attachments/assets/9fa42ea2-3c4e-4888-ba9d-80ab254a0f02" />

--------------------------------------------------------------------------------
6. HƯỚNG DẪN TƯƠNG TÁC VÀ LUẬT CHƠI
--------------------------------------------------------------------------------

- Ký hiệu: Bạn là quân 'X' (luôn đi trước), Máy tính là quân 'O', ô trống là dấu '.'.
- Cách đi quân: Khi màn hình hiển thị "Luot cua ban -> Nhap Hang va Cot (VD: 5 6):", 
  bạn gõ số hàng, gõ dấu cách, gõ số cột rồi ấn Enter. 
  * Ví dụ: Gõ "5 6" nghĩa là bạn muốn đánh vào Hàng 5, Cột 6 trên bàn cờ.
- Luật chiến thắng: Bên nào tạo được một chuỗi liên tiếp từ 4 quân cờ trở lên theo 
  hàng dọc, hàng ngang hoặc các đường chéo chính/phụ mà không bị đối phương chặn 
  ở cả 2 đầu sẽ giành chiến thắng tuyệt đối.
<img width="978" height="646" alt="image" src="https://github.com/user-attachments/assets/66042e91-fa9c-452b-8f00-747a9cbc58f1" />

--------------------------------------------------------------------------------
7. Ý NGHĨA CÁC CHỈ SỐ TRONG BẢNG THỐNG KÊ (LEVEL 3)
--------------------------------------------------------------------------------

Khi chạy Level 3, bảng dữ liệu kết quả sẽ cung cấp cho bạn các chỉ số:
- State     : Tên của kịch bản thế trận mẫu đang được kiểm thử.
- Algorithm : Thuật toán đang chạy đối chứng (Minimax hoặc Alpha-Beta).
- Depth     : Độ sâu tìm kiếm hiện tại (từ 1 đến 4).
- Move      : Tọa độ nước đi (Hàng, Cột) tối ưu mà thuật toán lựa chọn.
- Score     : Điểm số đánh giá trạng thái mà hàm Heuristic chấm cho nước đi đó.
- Nodes     : Tổng số lượng trạng thái mà thuật toán phải duyệt qua trên cây.
- Time (Ms) : Thời gian xử lý tính bằng mili-giây (giúp thấy rõ tốc độ vượt trội 
              của Alpha-Beta so với Minimax khi ở độ sâu lớn).
================================================================================
<img width="468" height="322" alt="image" src="https://github.com/user-attachments/assets/538b291f-8ddf-44b3-b64d-97b180bb31da" />

