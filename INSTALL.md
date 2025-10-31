# Установка SeekFS

## Быстрая установка (Linux/macOS)
```bash
curl -fsSL https://raw.githubusercontent.com/yourusername/SeekFS/main/scripts/install.sh | sh
```

## Установка из исходного кода

```bash
git clone https://github.com/yourusername/SeekFS.git
cd SeekFS
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
sudo make install
```

# Пакетные менеджеры

## Ubuntu/Debian

```bash
# Скачайте .deb пакет из релизов и установите:
sudo dpkg -i seekfs_1.0.0_amd64.deb
```

## Homebrew (macOS)

```bash
brew tap yourusername/seekfs
brew install seekfs
```

## Windows
1. Скачайте ZIP архив из релизов
2. Распакуйте в C:\Program Files\SeekFS\
3. Добавьте путь в системную переменную PATH
