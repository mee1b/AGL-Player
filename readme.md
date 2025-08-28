# 🎮 AGL Player (Accessible Games Lab) — плеер текстовых игр

AGL Player — десктопный плеер текстовых игр с акцентом на доступность (поддержка скринридеров), системой плагинов и простым интерфейсом для запуска/обновления игр.

## 📌 Назначение проекта

Проект предназначен для запуска и управления текстовыми играми (плагинами), которые реализуют единый GameInterface. Основные цели:

- упрощённая загрузка/выгрузка плагинов (.dll);
- поддержка скринридеров (NVDA, JAWS, Narrator);

## ✨ Возможности (MVP)

- Загрузка плагинов из папки `plugins/`.
- Менеджер плагинов (GUI) — просмотр, запуск, удаление, обновление.
- Озвучивание через NVDAController / TalkWrap с fallback.
- Асинхронный потоковый логгер (INFO / WARNING / ERROR).
- Поддержка доступности: QAccessible, фокусы, горячие клавиши.

## ⚙️ Требования для сборки

- C++17 (рекомендовано)
- Qt 6.8.3 (Widgets, Core, Gui)
- CMake 3.16 или выше
- Компилятор: MSVC (Windows)

## 🧰 Быстрая сборка (CMake)

Пример для Windows + Qt (параметр `CMAKE_PREFIX_PATH` при необходимости):

```bash
mkdir build 
cd build 
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64/lib/cmake" 
cmake --build . --config Release
```

После сборки бинарник окажется в `build/bin/` (или аналогичной папке, в зависимости от CMakeLists).

## ▶️ Запуск

Поместите плагины (`.dll`) в папку `plugins/` рядом с бинарником.

Запустите исполняемый файл **AGL-Player**.

## 🧾 Логирование

Лог-файл: `log.txt` (в рабочей директории).

Уровни: `INFO`, `WARNING`, `ERROR`.

Рекомендации:

- `INFO` — начало/конец функций, успешные операции.
- `WARNING` — потенциальные проблемы, но не критичные.
- `ERROR` — ошибки, прерывающие выполнение операции.

Пример строки лога:

```txt
2025-08-27 15:54:09 [INFO] bool TopWindow::loadPlugin(): Плагин Баскетбол успешно загружен!
```

Рекомендованные макросы (пример использования в коде):

```cpp
#define LOG_FUNC_START() Logger::getInstance().logMessage(logLevel::INFO, QString("Начало метода %1").arg(Q_FUNC_INFO))
#define LOG_FUNC_END(msg) Logger::getInstance().logMessage(logLevel::INFO, QString("Метод %1 завершен успешно: %2").arg(Q_FUNC_INFO).arg(msg))
#define LOG_INFO(msg) Logger::getInstance().logMessage(logLevel::INFO, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg))
#define LOG_WARN(msg) Logger::getInstance().logMessage(logLevel::WARNING, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg))
#define LOG_ERR(msg) Logger::getInstance().logMessage(logLevel::ERROR, QString("%1: %2").arg(Q_FUNC_INFO).arg(msg))
```

## 🧰 Плагины

Формат: Qt-плагин, реализует `GameInterface`.

Ключевой контракт (интерфейс):

```cpp
class GameInterface {
public:
    virtual ~GameInterface() = default;
    virtual QString startMessage() const = 0;
    virtual QString gameInput(const QString& playerChoice) = 0;
    virtual bool isOver() const = 0;
};
```

Разместите `.dll` в `plugins/` и используйте "Обновить" в менеджере для динамической подхватки.

## 🛠 Частые проблемы / Troubleshooting

- DLL не удаляется на Windows — часто причина в живых объектах из плагина. При удалении:
  - отключите все сигналы/слоты у `instance()`;
  - вызовите `loader->unload();` 
  - если `unload()` возвращает false, проверьте таймеры/потоки/дочерние объекты внутри плагина;
  - как временная мера — переименовать файл в `.deleted.*` и удалить позже.

- Дублирование плагинов при обновлении — нужно полностью очищать внутренние структуры перед повторной загрузкой (включая `pluginsLoad`, `pluginsInterface`, `filePaths`, и UI).

## ✅ Проверенные платформы

- Windows 10/11 (MSVC, MinGW)

## 🧭 TODO / Планы (приоритеты)

### Высокий приоритет
- Вынести работу с плагинами в `PluginManager`:  
  `loadAll()`, `update()`, `deletePlugin()`, `clearTombstone()`.
- `TopWindow` должен обращаться к `PluginManager` через API, а не напрямую к `pluginsLoad/pluginsInterface/filePaths`.
- Исправить корректную безопасную очистку всех плагинов (поддержка удаления всех с подтверждением).
- Устранить дублирование при `updatePlug()` (очистка старого состояния перед новой загрузкой).

### Средний приоритет
- Вынести игровой цикл в `GameController` (управление `gameInterface`, `run/again/reset`).
- Вынести озвучивание в `TalkManager` (централизованное управление NVDA / TalkWrap / fallback).
- Добавить unit- и интеграционные тесты для `PluginManager` / `GameController`.

### Низкий приоритет / улучшения
- Реализовать структуру `list` + `unordered_map` (LRU-like) для хранения плагинов:

```cpp
std::list<PluginEntry> plugins; // где PluginEntry { QPluginLoader*, QString absPath, GameInterface* iface }
std::unordered_map<QString, iterator> fastLookup; // быстрый поиск и безопасное удаление/перемещение
```

- Улучшить GUI: прогресс-статус при загрузке/удалении, undo-логика для менеджера.
