## Установка и запуск

1. Скачайте и установите SFML 2.5.1 для Windows:
   - Перейдите на официальный сайт SFML
   - Скачайте SFML 2.6.2 для Windows 
   - Распакуйте архив в удобное место

2. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/ваш-username/GEMS.git
   ```

3. Откройте проект в Visual Studio:
   - Откройте файл `GEMS.sln`
   - В свойствах проекта (Project Properties) проверьте пути к SFML:
     - C/C++ -> General -> Additional Include Directories: путь к include папке SFML
     - Linker -> General -> Additional Library Directories: путь к lib папке SFML
     - Linker -> Input -> Additional Dependencies: добавьте все .lib файлы из lib папки SFML

4. Соберите и запустите проект:
   - Выберите конфигурацию Release
   - Нажмите Build -> Build Solution
   - Скопируйте все .dll файлы из bin папки SFML в папку с .exe файлом
   - Запустите GEMS.exe

Андреев Ярослав Николаевич, 5030102/30002
