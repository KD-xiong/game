file_paths = [
    "Bar.h", "Boom.h", "render.cpp", "Plane.h", "object.h", "Bar.cpp",
    "HP.cpp", "Boom.cpp", "Menu.hpp", "render.hpp", "define.h",
    "Enemy.h", "Enemy.cpp", "main.cpp", "Bullet.cpp", "object.cpp",
    "Menu.cpp", "Plane.cpp", "Record.h", "HP.h", "Bullet.h", "Record.cpp"
]

total_lines = 0
for file_path in file_paths:
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
            total_lines += len(lines)
    except FileNotFoundError:
        print(f"文件 {file_path} 未找到。")
    except Exception as e:
        print(f"读取文件 {file_path} 时出现错误: {e}")

print(f"这些代码总共写了 {total_lines} 行。")