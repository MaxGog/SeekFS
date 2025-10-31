# Установка SeekFS

## Предварительные требования

### Системные требования
- **Linux**: GLIBC 2.28+, 64-битная система
- **macOS**: 10.15 (Catalina) или новее
- **Windows**: Windows 10 или новее
- **Память**: минимум 512 МБ свободной оперативной памяти
- **Дисковое пространство**: 10 МБ для установки

### Зависимости для сборки из исходного кода
- **Компилятор**: GCC 9+, Clang 10+, MSVC 2019+ или Apple Clang 12+
- **CMake**: версия 3.12 или выше
- **Git**: для клонирования репозитория
- **Make** или **Ninja**: система сборки

## Способ установки

### 🚀 Быстрая установка (Linux/macOS)

```bash
# Автоматическая установка последней версии
curl -fsSL https://raw.githubusercontent.com/maxgog/SeekFS/main/scripts/install.sh | sh

# Или с явным указанием версии
curl -fsSL https://raw.githubusercontent.com/maxgog/SeekFS/main/scripts/install.sh | sh -s -- v1.0.0
```

**Что делает скрипт:**
- Определяет вашу ОС и архитектуру
- Скачивает соответствующий бинарный файл
- Проверяет контрольную сумму SHA256
- Устанавливает в `/usr/local/bin/` (требует sudo)
- Проверяет корректность установки

### 📦 Установка из исходного кода

#### Стандартная сборка
```bash
# Клонирование репозитория
git clone https://github.com/maxgog/SeekFS.git
cd SeekFS

# Создание директории для сборки
mkdir build && cd build

# Конфигурация проекта
cmake .. -DCMAKE_BUILD_TYPE=Release

# Сборка (можно указать количество ядер)
make -j$(nproc)

# Тестирование (опционально)
make test

# Установка
sudo make install
```

#### Расширенные опции сборки
```bash
# Сборка с отладочной информацией
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Установка в пользовательскую директорию
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local

# Сборка с использованием Ninja
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja
```

### 📟 Пакетные менеджеры

#### Ubuntu/Debian
```bash
# Установка из .deb пакета
wget https://github.com/maxgog/SeekFS/releases/latest/download/seekfs_1.0.0_amd64.deb
sudo dpkg -i seekfs_1.0.0_amd64.deb

# Если есть ошибки зависимостей
sudo apt-get install -f
```

#### Homebrew (macOS)
```bash
# Добавление репозитория и установка
brew tap maxgog/seekfs
brew install seekfs

# Обновление до новой версии
brew update && brew upgrade seekfs
```

#### Arch Linux (AUR)
```bash
# Использование yay (или другого AUR-хелпера)
yay -S seekfs-git
```

#### Windows
```bash
# Использование Chocolatey
choco install seekfs

# Или ручная установка:
# 1. Скачайте ZIP-архив из раздела релизов
# 2. Распакуйте в C:\Program Files\SeekFS\
# 3. Добавьте C:\Program Files\SeekFS\bin в системную переменную PATH
# 4. Перезапустите терминал/командную строку
```

### 🔧 Установка для разработчиков

```bash
git clone https://github.com/maxgog/SeekFS.git
cd SeekFS

# Инициализация подмодулей (если есть)
git submodule update --init --recursive

# Сборка с санитайзерами для отладки
mkdir build-debug && cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
make

# Запуск тестов
./tests/seekfs_tests
```

## Проверка установки

После установки проверьте корректность работы:

```bash
seekfs --version
seekfs --help
```

Ожидаемый вывод:
```
SeekFS v1.0.0
Утилита для продвинутого поиска файлов

Использование: SeekFS [ОПЦИИ]... [ПУТЬ]...
...
```

## Устранение проблем

### Распространенные проблемы

**Проблема**: "Команда не найдена"
```bash
# Решение: добавьте директорию установки в PATH
export PATH=$PATH:/usr/local/bin
# Для постоянного решения добавьте в ~/.bashrc или ~/.profile
```

**Проблема**: Ошибки при сборке
```bash
# Обновите CMake
sudo apt update && sudo apt upgrade cmake  # Ubuntu/Debian
brew upgrade cmake  # macOS

# Убедитесь, что компилятор поддерживает C++17
g++ --version
```

**Проблема**: Отсутствуют зависимости
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake git

# macOS
xcode-select --install
brew install cmake git

# CentOS/RHEL
sudo yum groupinstall 'Development Tools'
sudo yum install cmake git
```

### Получение помощи

Если у вас возникли проблемы с установкой:
1. Проверьте [Issues на GitHub](https://github.com/maxgog/SeekFS/issues)
2. Убедитесь, что ваша система соответствует требованиям
3. Проверьте версии компилятора и CMake
4. Создайте issue с подробным описанием проблемы

## Обновление

### Обновление существующей установки

```bash
# Для установки через скрипт
curl -fsSL https://raw.githubusercontent.com/maxgog/SeekFS/main/scripts/install.sh | sh

# Для Homebrew
brew update && brew upgrade seekfs

# Для сборки из исходного кода
cd SeekFS
git pull origin main
cd build && make && sudo make install
```

## Удаление

```bash
# Для установки через make
sudo make uninstall  # из папки build

# Для Homebrew
brew uninstall seekfs
brew untap maxgog/seekfs

# Для .deb пакета
sudo dpkg -r seekfs

# Ручное удаление
sudo rm -f /usr/local/bin/seekfs
```

---

**Примечание**: SeekFS находится в активной разработке. Рекомендуется регулярно обновляться для получения новых функций и исправлений ошибок.
