#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <Windows.h>
#include <stdexcept>
#include <algorithm>

using namespace std;

class TrackProgress {
public:
    float currentTime;

    // Метод для перемотки трека на 5 секунд вперед
    void jump_5sec_timeline(float* time) {
        if (time) {
            *time += 5.0f;
        }
    }
};

class PlaylistSettings {
private:
    bool shuffle; // Режим случайного воспроизведения
    bool repeat;  // Режим повтора

public:
    void inputSettings() {
        int input;

        cout << "Введите режим случайного воспроизведения (1 - включено, 0 - выключено): ";
        cin >> input;
        shuffle = (input == 1);

        cout << "Введите режим повторного воспроизведения (1 - включено, 0 - выключено): ";
        cin >> input;
        repeat = (input == 1);
    }

    void print() const {
        cout << "Случайное воспроизведение: " << (shuffle ? "включено" : "выключено")
            << "\nПовторное воспроизведение: " << (repeat ? "включено" : "выключено") << endl;
    }
};


class Device {
private:
    static constexpr const char* deviceType = "Audio Device"; // Тип устройства (статический)
    string deviceName; // Название устройства
    int maxVolume;     // Максимальная громкость
    int currentVolume; // Текущая громкость

public:
    void set(const string& name, int maxVol, int currVol) {
        deviceName = name;
        maxVolume = maxVol;
        currentVolume = currVol;
    }

    void print() const {
        cout << "Название устройства: " << deviceName
            << "\nМаксимальная громкость: " << maxVolume
            << "\nТекущая громкость: " << currentVolume << endl;
    }

    static void printDeviceType() {
        cout << "Тип устройства: " << deviceType << endl;
    }
};

class AudioSettings {
private:
    int volume;   // Уровень громкости
    int balance;  // Баланс каналов

public:
    void set(int vol, int bal) {
        volume = vol;
        balance = bal;
    }

    void print() const {
        cout << "Громкость: " << volume
            << "\nБаланс: " << balance << endl;
    }
};


class Content {
protected:
    string title;
    string artist;
    float duration;
    string format;

public:
    Content(const string& t = "", const string& a = "", float d = 0.0f, const string& f = "")
        : title(t), artist(a), duration(d), format(f) {
    }

    virtual ~Content() = default;

    virtual string getInfo() const {
        return "Track: " + title + " by " + artist +
            " (Duration: " + to_string(duration) + " sec, Format: " + format + ")";
    }

    void set(const string& t, const string& a, float d, const string& f) {
        title = t;
        artist = a;
        duration = d;
        format = f;
    }

    const string& getTitle() const { return title; }
    float getDuration() const { return duration; }
};


class PodcastContent : public Content {
private:
    string host;       // Ведущий подкаста
    int episodeCount;  // Количество эпизодов
    string description; // Краткое описание

public:
    // Конструктор с параметрами
    PodcastContent(const string& t, const string& a, float d, const string& f,
        const string& h, int eCount, const string& desc)
        : Content(t, a, d, f), host(h), episodeCount(eCount), description(desc) {
    }

    // Переопределение виртуальной функции
    string getInfo() const override {
        return "Podcast: " + title + " by " + artist +
            " (Duration: " + to_string(duration) + " sec, Format: " + format + ")" +
            "\nHost: " + host +
            "\nEpisodes: " + to_string(episodeCount) +
            "\nDescription: " + description;
    }
};

// Абстрактный класс MediaContent
class MediaLibrary {
private:
    vector<Content*> library;

public:
    ~MediaLibrary() {
        for (auto content : library) {
            delete content;
        }
    }

    void addContent(Content* content) {
        library.push_back(content);
    }

    void sortLibraryByDuration() {
        sort(library.begin(), library.end(), [](Content* a, Content* b) {
            return a->getDuration() < b->getDuration();
            });
    }

    Content* searchByTitle(const string& title) const {
        auto it = find_if(library.begin(), library.end(), [&title](Content* content) {
            return content->getTitle() == title;
            });
        return (it != library.end()) ? *it : nullptr;
    }

    void printLibrary() const {
        for (const auto& content : library) {
            cout << content->getInfo() << endl;
        }
    }
};

// Производный класс для музыкального трека
class MusicTrack : public MediaContent {
private:
    string genre;

public:
    MusicTrack(const string& t, const string& c, float d, const string& g)
        : MediaContent(t, c, d), genre(g) {
    }

    string getInfo() const override {
        return "Music Track: " + title + " by " + creator +
            " (Duration: " + to_string(duration) + " sec, Genre: " + genre + ")";
    }
};

// Производный класс для аудиокниги
class Audiobook : public MediaContent {
private:
    string narrator;
    int chapters;

public:
    Audiobook(const string& t, const string& c, float d, const string& n, int ch)
        : MediaContent(t, c, d), narrator(n), chapters(ch) {
    }

    string getInfo() const override {
        return "Audiobook: " + title + " by " + creator +
            " (Duration: " + to_string(duration) + " sec)" +
            "\nNarrator: " + narrator + ", Chapters: " + to_string(chapters);
    }
};

class Playlist {
private:
    string name_playlist;
    Content* tracks;
    int trackCount;

    static int totalTrackCount; // Общее количество треков

public:
    Playlist() : tracks(nullptr), trackCount(0) {}

    Playlist(const Playlist& other) : name_playlist(other.name_playlist), trackCount(other.trackCount) {
        if (trackCount > 0) {
            tracks = new Content[trackCount];
            for (int i = 0; i < trackCount; i++) {
                tracks[i] = other.tracks[i];
            }
        }
        else {
            tracks = nullptr;
        }
        totalTrackCount += trackCount;
    }

    ~Playlist() {
        delete[] tracks;
        tracks = nullptr; // Убедимся, что указатель обнулен
        totalTrackCount -= trackCount;
    }

    Playlist& operator=(const Playlist& other) {
        if (this == &other)
            return *this;

        delete[] tracks;

        name_playlist = other.name_playlist;
        trackCount = other.trackCount;

        if (trackCount > 0) {
            tracks = new Content[trackCount];
            for (int i = 0; i < trackCount; i++) {
                tracks[i] = other.tracks[i];
            }
        }
        else {
            tracks = nullptr;
        }

        return *this;
    }

    void setName(const string& name) {
        name_playlist = name;
    }

    void add_tracks_to_playlist(int count) {
        delete[] tracks;
        tracks = new Content[count];
        trackCount = count;

        cin.ignore(); // Очистка буфера ввода
        for (int i = 0; i < count; i++) {
            string title, artist, format;
            float duration;

            cout << "Введите название трека: ";
            getline(cin, title);
            cout << "Введите исполнителя: ";
            getline(cin, artist);
            cout << "Продолжительность в секундах: ";
            cin >> duration;
            cin.ignore(); // Очистка буфера ввода
            cout << "Введите формат трека: ";
            getline(cin, format);

            tracks[i].set(title, artist, duration, format);
        }

        totalTrackCount += count;
    }

    void print_playlist_info() const {
        cout << "Playlist Name: " << name_playlist << endl;
        for (int i = 0; i < trackCount; i++) {
            cout << "Track " << i + 1 << ": ";
            tracks[i].print();
        }
    }

    const Content* getTracks() const {
        return tracks;
    }

    int getTrackCount() const {
        return trackCount;
    }

    static int getTotalTrackCount() {
        return totalTrackCount;
    }
};

int Playlist::totalTrackCount = 0;


class User {
private:
    vector<Playlist> userPlaylists;
public:
    void add_playlist_user() {
        Playlist p;
        cout << "Введите название нового плейлиста: ";
        string name;
        cin.ignore(); // Очистка буфера ввода
        getline(cin, name);
        p.setName(name);

        int trackCount;
        cout << "Введите количество треков: ";
        cin >> trackCount;
        p.add_tracks_to_playlist(trackCount);

        userPlaylists.push_back(p);
    }

    void print_user_playlists() const {
        for (size_t i = 0; i < userPlaylists.size(); ++i) {
            cout << "\nПлейлист " << i + 1 << ":\n";
            userPlaylists[i].print_playlist_info();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Работа с Device
    Device device;
    device.set("Мультимедиа система", 100, 50);
    device.print();
    Device::printDeviceType();

    // Работа с AudioSettings
    AudioSettings audioSettings;
    audioSettings.set(70, 0); // Громкость 70, баланс 0
    audioSettings.print();

    // Работа с PlaylistSettings
    PlaylistSettings playlistSettings;
    playlistSettings.inputSettings();
    playlistSettings.print();

    // Работа с пользователями и плейлистами
    int numUsers;
    cout << "Введите количество пользователей: ";
    cin >> numUsers;

    vector<User> usersArray(numUsers);

    for (int i = 0; i < numUsers; ++i) {
        char addMore;
        cout << "\nДобавление плейлистов для пользователя " << i + 1 << ":\n";
        do {
            usersArray[i].add_playlist_user();
            cout << "Хотите добавить еще один плейлист? (y/n): ";
            cin >> addMore;
        } while (addMore == 'y' || addMore == 'Y');
    }

    for (int i = 0; i < numUsers; ++i) {
        cout << "\nПлейлисты пользователя " << i + 1 << ":\n";
        usersArray[i].print_user_playlists();
    }

    // Демонстрация работы TrackProgress
    char question;
    TrackProgress trackProgress;
    trackProgress.currentTime = 0;
    cout << "Хотите перемотать трек на 5 сек вперед? (y/n): ";
    cin >> question;
    if (question == 'y' || question == 'Y') {
        trackProgress.jump_5sec_timeline(&trackProgress.currentTime);
    }
    cout << "Текущий прогресс трека: " << trackProgress.currentTime << " секунд\n";

    // Создаем объект базового класса
    Content baseContent("Музыкальный трек", "Artist A", 180.0f, "MP3");

    // Создаем объект производного класса
    PodcastContent podcast("Научный подкаст", "Science Weekly", 3600.0f, "MP3",
        "Дмитрий Иванов", 24, "Интересные факты о науке");

    // Демонстрация вызова через невиртуальную функцию
    cout << "Вызов через невиртуальную функцию базового класса:\n";
    baseContent.printInfo();
    podcast.printInfo();

    // Использование указателей на базовый и производный классы
    cout << "\nВызов через указатели:\n";
    Content* ptrBase = &baseContent;
    Content* ptrPodcast = &podcast;

    ptrBase->printInfo();  // Вызов базовой версии
    ptrPodcast->printInfo();  // Вызов переопределенной версии

    // Демонстрация изменения работы программы, если функция не виртуальная
    cout << "\nЕсли функция getInfo() не виртуальная:\n";
    ptrBase->printInfo();  // Всегда вызывает базовую версию
    ptrPodcast->printInfo();  // Также вызывает базовую версию

    // Создаем список указателей на MediaContent
    vector<MediaContent*> mediaLibrary;

    // Добавляем различные типы медиа-контента
    mediaLibrary.push_back(new MusicTrack("Song A", "Artist 1", 210.0f, "Pop"));
    mediaLibrary.push_back(new Audiobook("Book B", "Author 2", 7200.0f, "Narrator X", 15));

    // Перебираем и выводим информацию о каждом контенте
    for (const auto& content : mediaLibrary) {
        cout << content->getInfo() << endl;
        content->play();
        cout << endl;
    }

    // Удаляем объекты из памяти
    for (auto& content : mediaLibrary) {
        delete content;
    }
}
