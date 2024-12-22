#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <Windows.h>
#include <stdexcept>
#include <algorithm> // Для сортировки и поиска

using namespace std;

class TrackProgress {
public:
    float currentTime;

    void jump_5sec_timeline(float* time) {
        if (time) {
            *time += 5.0f;
        }
    }
};

class PlaylistSettings {
private:
    bool shuffle;
    bool repeat;

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
    static constexpr const char* deviceType = "Audio Device";
    string deviceName;
    int maxVolume;
    int currentVolume;

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
    int volume;
    int balance;

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
    string host;
    int episodeCount;
    string description;

public:
    PodcastContent(const string& t, const string& a, float d, const string& f,
        const string& h, int eCount, const string& desc)
        : Content(t, a, d, f), host(h), episodeCount(eCount), description(desc) {
    }

    string getInfo() const override {
        return "Podcast: " + title + " by " + artist +
            " (Duration: " + to_string(duration) + " sec, Format: " + format + ")" +
            "\nHost: " + host +
            "\nEpisodes: " + to_string(episodeCount) +
            "\nDescription: " + description;
    }
};

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

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    MediaLibrary mediaLibrary;

    mediaLibrary.addContent(new Content("Song A", "Artist 1", 210.0f, "MP3"));
    mediaLibrary.addContent(new PodcastContent("Science Podcast", "Host 1", 3600.0f, "MP3", "Dr. Smith", 10, "Interesting science facts"));

    cout << "Медиатека до сортировки:" << endl;
    mediaLibrary.printLibrary();

    mediaLibrary.sortLibraryByDuration();

    cout << "\nМедиатека после сортировки по продолжительности:" << endl;
    mediaLibrary.printLibrary();

    string searchTitle;
    cout << "\nВведите название для поиска: ";
    getline(cin, searchTitle);

    Content* foundContent = mediaLibrary.searchByTitle(searchTitle);
    if (foundContent) {
        cout << "\nНайденный контент:" << endl;
        cout << foundContent->getInfo() << endl;
    }
    else {
        cout << "\nКонтент с таким названием не найден." << endl;
    }

    return 0;
}
