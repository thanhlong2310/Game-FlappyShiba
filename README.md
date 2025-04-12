	ĐẠI HỌC QUỐC GIA HÀ NỘI	CỘNG HÒA XÃ HỘI CHỦ NGHĨA VIỆT NAM
	TRƯỜNG ĐẠI HỌC CÔNG NGHỆ	Độc lập - Tự do - Hạnh phúc
	🙥🕮🙧	====================
BÁO CÁO BÀI TẬP LỚP MÔN LẬP TRÌNH NÂNG CAO
Họ và Tên : Nguyễn Thành Long
Mã số Sinh viên : 24020210                       
Lớp Môn học : INT 2215 - 3
1. Thông tin về game
Tên game: Flappy Shiba
Link GitHub: https://github.com/thanhlong2310/Game-FlappyShiba
Game tham khảo: Lấy cảm hứng từ Flappy Bird với các cải tiến về gameplay và đồ họa ( https://flappybird.io/)
2. Các tính năng cải tiến trong game
Hệ thống level với độ khó khác nhau:
Level 1: Dễ (tốc độ bình thường)
Level 2: Trung bình (tốc độ tăng 1.5x)
Level Asian: Khó (tốc độ tăng 3x)
Nhiều chướng ngại vật đa dạng:
Ống nước
Bom
Tên lửa
Hiệu ứng đặc biệt:
Hiệu ứng nổ khi va chạm
Hiệu ứng rung màn hình khi nổ
Âm thanh khi nhảy và nổ, nhạc nền
Giao diện người dùng:
Menu chọn level
Màn hình game over với điểm số
Nút replay và exit
3. Cấu trúc project game
Tổ chức lớp
AudioManager: Quản lý âm thanh (nhạc nền, hiệu ứng)
Bird: Nhân vật chính (chú chó Shiba)
Pipe: Chướng ngại vật ống nước
Bomb: Chướng ngại vật bom
Rocket: Chướng ngại vật tên lửa
Game: Lớp chính quản lý toàn bộ trò chơi
Screen: Quản lý màn hình 
Luồng chương trình
Khởi tạo SDL, tạo cửa sổ và renderer
Hiển thị màn hình menu với nút Start
Chọn level (1, 2 hoặc Asian)
Bắt đầu game với đếm ngược 3-2-1
Người chơi điều khiển chú chó bay qua các chướng ngại vật
Khi va chạm, hiệu ứng nổ xuất hiện và chuyển sang màn hình game over
Người chơi có thể chọn replay hoặc exit
4. Nguồn tham khảo và dữ liệu
Đồ họa: (Nguồn : https://pngtree.com/)
Hình ảnh chú chó Shiba
Hình ảnh background, ống nước, bom, tên lửa
Hiệu ứng nổ
Âm thanh: (Nguồn : https://mixkit.co/free-sound-effects/)
Nhạc nền
Âm thanh khi nhảy
Âm thanh khi nổ
Thư viện:
SDL2
SDL_image
SDL_ttf
SDL_mixer
Tài liệu tham khảo:
Lazy Foo' Productions (Hướng dẫn SDL)
Grok3, Deepseek

