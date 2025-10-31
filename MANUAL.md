# Руководство пользователя SeekFS

## Базовое использование

### Основной синтаксис
```bash
SeekFS [ОПЦИИ]... [ПУТЬ]...
```

### Получение справки
```bash
# Основная справка
seekfs --help

# Справка по конкретной опции
seekfs --help | grep -A 5 "duplicates"

# Проверка версии
seekfs --version
```

## Поисковые паттерны

### Регулярные выражения для имен файлов

#### Базовые паттерны
```bash
# Все файлы с расширением .txt
seekfs -n ".*\.txt$"

# Файлы, начинающиеся с "report"
seekfs -n "^report.*"

# Файлы, заканчивающиеся на .log или .tmp
seekfs -n ".*\.(log|tmp)$"

# Файлы с датой в формате YYYY-MM-DD
seekfs -n ".*202[0-9]-[0-9]{2}-[0-9]{2}.*"

# Файлы с номером версии (v1.2.3)
seekfs -n ".*v[0-9]+\.[0-9]+\.[0-9]+.*"
```

#### Специализированные паттерны
```bash
# Файлы резервных копий
seekfs -n ".*(~|\.bak|\.backup)$"

# Временные файлы
seekfs -n "^(tmp|temp).*" --path /tmp

# Файлы конфигурации
seekfs -n ".*(\.conf|\.config|\.ini|\.cfg)$"
```

### Поиск по содержимому

#### Текстовые паттерны
```bash
# Поиск email адресов
seekfs -c "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}"

# Поиск комментариев TODO/FIXME
seekfs -c "TODO|FIXME|XXX|HACK" --type py,java,cpp,js

# Поиск IP адресов IPv4
seekfs -c "\b(?:[0-9]{1,3}\.){3}[0-9]{1,3}\b"

# Поиск URL
seekfs -c "https?://[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(?:/[a-zA-Z0-9._~:/?#@!$&'()*+,;=-]*)?"
```

#### Кодовые паттерны
```bash
# Поиск определений функций/классов
seekfs -c "^(def|class)\s+[a-zA-Z_][a-zA-Z0-9_]*" --type py

# Поиск SQL запросов
seekfs -c "SELECT.*FROM|INSERT INTO|UPDATE.*SET" --type sql,txt,py

# Поиск лог-сообщений с уровнем ERROR
seekfs -c "\[ERROR\]|ERROR:" --type log,txt
```

## Фильтрация результатов

### Фильтрация по типу файлов

```bash
# Только текстовые файлы
seekfs -c "TODO" --type txt,md,rst

# Только кодовая база
seekfs -n ".*" --type py,java,cpp,js,ts,go --path /project/src

# Только изображения
seekfs -d --type jpg,jpeg,png,gif,bmp --path ~/Pictures

# Исключение бинарных файлов
seekfs -c "pattern" --type !bin,!exe,!so,!dll
```

### Фильтрация по размеру

```bash
# Маленькие файлы (до 1 МБ)
seekfs -d --max-size 1 --type txt,md

# Средние файлы (1-10 МБ)
seekfs -c "error" --min-size 1 --max-size 10

# Большие файлы (свыше 100 МБ)
seekfs -d --min-size 100 --progress

# Точный диапазон размеров
seekfs -n ".*\.log$" --min-size 0.1 --max-size 5
```

### Дополнительные фильтры

```bash
# Игнорирование регистра
seekfs -c "TODO" -i --type txt

# Поиск только в скрытых файлах/директориях
seekfs -n "^\..*" --path ~

# Исключение node_modules и .git
seekfs -c "function" --exclude "node_modules|\.git" --progress
```

## Управление производительностью

### Оптимизация многопоточности

```bash
# Автоматическое определение количества ядер
seekfs -d -t $(nproc) --progress

# Ограничение потоков для слабых систем
seekfs -c "pattern" -t 2 --progress

# Максимальная производительность (осторожно!)
seekfs -d -t 16 --progress --max-size 500

# Баланс между производительностью и отзывчивостью системы
seekfs -n ".*" -t $(($(nproc) - 1)) --progress
```

### Управление памятью и вводом-выводом

```bash
# Ограничение размера для экономии памяти
seekfs -d --max-size 10 --type jpg,png

# Приоритет скорости над памятью
seekfs -c "important" --max-size 1000 --progress

# Оптимизация для SSD vs HDD
# Для SSD - больше параллелизма
seekfs -d -t 8 --progress
# Для HDD - меньше параллелизма, чтобы избежать trashing
seekfs -d -t 2 --progress
```

### Стратегии поиска для больших директорий

```bash
# Поэтапный поиск в очень больших директориях
seekfs -n ".*\.txt$" --path /very/large/dir --max-size 1 | head -100

# Поиск с ограничением глубины
seekfs -c "config" --max-depth 3 --path /etc

# Пакетная обработка результатов
seekfs -d --type jpg,jpeg --progress | while read file; do
    echo "Обработка: $file"
    # дальнейшая обработка
done
```

## Управление выводом

### Форматирование результатов

```bash
# Простой список файлов
seekfs -n ".*\.py$" --no-color

# Подробный вывод с путями
seekfs -c "class" --type py --verbose

# Вывод только имен файлов (без путей)
seekfs -n ".*\.txt$" --basename-only

# Статистика поиска
seekfs -d --type jpg,png --stats
```

### Сохранение и обработка результатов

```bash
# Сохранение в файл
seekfs -c "password" --type py,js,json > suspicious_files.txt

# Добавление в существующий файл
seekfs -n ".*\.log$" --progress >> all_logs.txt

# Параллельная обработка результатов
seekfs -d --type mp4,avi,mkv | xargs -P 4 -I {} ffmpeg -i "{}" "{}.compressed.mp4"

# Фильтрация и сортировка результатов
seekfs -n ".*" --type txt | sort | uniq > sorted_files.txt
```

### Интеграция с другими утилитами

```bash
# Поиск и открытие в редакторе
seekfs -c "TODO" --type py | xargs vim

# Копирование найденных файлов
seekfs -n ".*\.jpg$" --path ~/Photos | xargs -I {} cp {} ~/backup/

# Подсчет строк кода
seekfs -n ".*\.(py|js|java)$" --path /project | xargs wc -l

# Поиск и архивирование
seekfs -n ".*\.log$" --min-size 1 | tar -czf large_logs.tar.gz -T -
```

## Примеры использования

### Повседневные задачи

```bash
# Поиск временных файлов для очистки
seekfs -n ".*(\.tmp|~|\.swp)$" --progress

# Поиск больших файлов для освобождения места
seekfs -n ".*" --min-size 100 --path ~ --progress

# Поиск дубликатов фотографий
seekfs -d --type jpg,jpeg,png --path ~/Pictures --progress -t 4

# Поиск конфигурационных файлов
seekfs -n ".*(\.conf|\.config|\.ini)$" --path /etc --progress
```

### Задачи разработчика

```bash
# Поиск неиспользуемых импортов
seekfs -c "import.*not_used" --type py --ignore-case

# Поиск хардкоденных паролей
seekfs -c "password\s*=\s*['\"][^'\"]+['\"]" --type py,js,java

# Поиск устаревших API вызовов
seekfs -c "old_api_function|deprecated_method" --type py,js

# Поиск файлов с длинными строками
seekfs -c "^.{100,}$" --type py,js,css --progress
```

### Административные задачи

```bash
# Поиск файлов с определенными правами доступа
seekfs -n ".*" --perm 777 --path /var/www

# Поиск лог-файлов с ошибками
seekfs -c "ERROR|CRITICAL|FATAL" --type log --path /var/log

# Поиск измененных файлов за последние 24 часа
find /path -mtime -1 | xargs seekfs -c "suspicious_pattern"

# Мониторинг дубликатов в системе
seekfs -d --path /home --max-size 100 --progress | tee duplicates_report.txt
```

## Обработка ошибок и отладка

### Диагностика проблем

```bash
# Подробный вывод для отладки
seekfs -n ".*" --verbose --debug

# Проверка прав доступа
seekfs -c "pattern" --path /root 2>&1 | grep -i "permission"

# Тестирование производительности
time seekfs -d --path ~/Documents --progress

# Проверка корректности регулярных выражений
seekfs -n ".*\.txt$" --dry-run
```

### Обработка частых ошибок

```bash
# Если не хватает памяти
seekfs -d --max-size 10 -t 2 --progress

# Если слишком много открытых файлов
ulimit -n 10000 && seekfs -c "pattern" --progress

# Если поиск занимает слишком долго
seekfs -n ".*" --path /large_dir | head -1000

# Для проблемных файловых систем
seekfs -c "content" --skip-errors --progress
```

### Логирование и мониторинг

```bash
# Логирование сессии поиска
seekfs -d --progress 2>&1 | tee search_session.log

# Поиск с метриками производительности
/usr/bin/time -v seekfs -c "complex_pattern" --progress

# Периодический мониторинг
while true; do
    seekfs -c "ERROR" --type log --path /var/log --newer-than 1h
    sleep 300
done
```

## Советы и лучшие практики

### Оптимизация скорости поиска

1. **Используйте фильтры по размеру** - исключайте большие файлы, когда это возможно
2. **Ограничивайте глубину поиска** - используйте `--max-depth` для вложенных структур
3. **Кэшируйте результаты** - сохраняйте частые запросы для повторного использования
4. **Используйте инкрементальный поиск** - начинайте с широких шаблонов, затем сужайте

### Безопасность и конфиденциальность

```bash
# Не ищите конфиденциальные данные в логах
seekfs -c "password|token|key" --path /var/log > security_audit.txt

# Осторожно с правами доступа
seekfs -n ".*" --path /etc/sudoers.d 2>/dev/null || echo "Access denied"

# Шифрование результатов поиска
seekfs -c "confidential" | gpg --encrypt -r recipient@email.com > results.gpg
```

### Интеграция в скрипты

```bash
#!/bin/bash
# Пример скрипта для регулярной очистки дубликатов

LOG_FILE="/var/log/seekfs_cleanup.log"
echo "$(date): Starting duplicate cleanup" >> "$LOG_FILE"

seekfs -d --path /home --max-size 50 --type jpg,jpeg,png --progress | \
while read -r file; do
    if [[ -f "$file" ]]; then
        backup_dir="/backup/duplicates/$(date +%Y-%m)"
        mkdir -p "$backup_dir"
        mv "$file" "$backup_dir/"
        echo "Moved: $file to $backup_dir" >> "$LOG_FILE"
    fi
done

echo "$(date): Cleanup completed" >> "$LOG_FILE"
```
